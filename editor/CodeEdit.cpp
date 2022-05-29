#include <QString>
#include <QPainter>
#include <QTextBlock>
#include <QColor>
#include <QDebug>
#include <QSettings>
#include <QTextCursor>
#include <QDir>

#include "CodeEdit.h"
#include "TabManager.h"

#include "lsp-cpp/include/LSPUri.hpp"
#include "utils/JsonUtil.h"

CodeEdit::CodeEdit(QWidget *parent, const QString &fileAbsolutePath)
    : QPlainTextEdit(parent),
      openedFileInfo(fileAbsolutePath),
      lineNumberArea(new LineNumberArea(this)),
      _textChanged(false),
      lspClient(new LSPClient("E:\\Personal\\Workspaces\\LearnQt\\PyEditor\\PyEditor\\lspserver.exe", { })),
      tempFile(QDir::tempPath() + openedFileInfo.fileName() + "XXXXXX.py")
{
    this->setTabStopWidth(40);
    this->setTabStopDistance(40.00);
    this->setWordWrapMode(QTextOption::NoWrap);
    this->setFrameStyle(QFrame::NoFrame);
    this->readSettings();

    this->updateLineNumberAreaWidth(0);
    this->highlightCurrentLine();

    QTextCursor textCursor = this->textCursor();
    textCursor.setPosition(0);
    this->setTextCursor(textCursor);

    // setting lspclient connection
    connect(this->document(), &QTextDocument::contentsChanged, this, &CodeEdit::onContentChanged);
    connect(lspClient, &LSPClient::onDocumentColorResponse, this, &CodeEdit::onLSPClientDocumentColorResponseReceived);

    connect(this, &CodeEdit::blockCountChanged, this, &CodeEdit::updateLineNumberAreaWidth);
    connect(this, &CodeEdit::updateRequest, this, &CodeEdit::updateLineNumberArea);
    connect(this, &CodeEdit::cursorPositionChanged, this, &CodeEdit::highlightCurrentLine);
    connect(this, &CodeEdit::textChanged, this, &CodeEdit::onTextChanged);
}

CodeEdit::~CodeEdit()
{
    lspClient->shutdown();
    delete lspClient;
}

/**
 * read settings from .ini
 * @brief readSettings
 */
void CodeEdit::readSettings()
{
    QString fontFamily = "Consolas";
    int fontSize = 12;
    int fontWeight = 10;

    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "dylan", "PyEditor");
    QVariant fontFamilyVariant = settings.value("editor/fontFamily");
    QVariant fontSizeVariant = settings.value("editor/fontSize");
    if (!fontFamilyVariant.isNull()) {
        fontFamily = fontFamilyVariant.value<QFont>().family();
    }
    if (!fontSizeVariant.isNull()) {
        fontSize = fontSizeVariant.value<int>();
    }

    this->setFont(QFont(fontFamily, fontSize, fontWeight));
}

/**
 * draw line number
 * @brief CodeEdit::lineNumberAreaPaintEvent
 * @param event
 */
void CodeEdit::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), Qt::lightGray);

    // a block per line in QPlainTextEdit
    // so a block means a line
    QTextBlock block = this->firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) this->blockBoundingRect(block).height();

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);
            painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
                             Qt::AlignCenter, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) this->blockBoundingRect(block).height();
        ++blockNumber;
    }
}

/**
 * calculate latest width of lineNumberArea
 * @brief CodeEdit::lineNumberAreaWidth
 * @return
 */
int CodeEdit::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    int space = 3 + 20 + this->fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;

    return space;
}

/**
 * override keyPressEvent, change behaviour of shift + enter(return)
 * @brief CodeEdit::keyPressEvent
 * @param event
 */
void CodeEdit::keyPressEvent(QKeyEvent *event)
{
    QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);

    // disable shift + enter(return)
    if (keyEvent->type() == QEvent::KeyPress && (keyEvent->modifiers() & Qt::ShiftModifier)
            && (keyEvent->key() == Qt::Key_Enter || keyEvent->key() == Qt::Key_Return)) {
        event->ignore();
        return;
    }
    QPlainTextEdit::keyPressEvent(event);
}
/**
 * override resizeEvent
 * @brief CodeEdit::resizeEvent
 * @param e
 */
void CodeEdit::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

/**
 * update width of lineNumberArea
 * @brief CodeEdit::updateLineNumberAreaWidth
 */
void CodeEdit::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    this->setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

/**
 * @brief CodeEdit::updateLineNumberArea
 * @param rect
 * @param dy
 */
void CodeEdit::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy) {
        lineNumberArea->scroll(0, dy);
    } else {
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());
    }

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}

/**
 * highlight current line
 * @brief CodeEdit::highlightCurrentLine
 */
void CodeEdit::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor(Qt::yellow).lighter(160);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    this->setExtraSelections(extraSelections);
}

/**
 * @brief CodeEdit::textHasChanged
 * @return
 */
bool CodeEdit::textHasChanged() const
{
    return this->_textChanged;
}

/**
 * @brief CodeEdit::setTextChangedStatus
 * @param changed
 */
void CodeEdit::setTextChangedStatus(bool changed)
{
    this->_textChanged = changed;
}

/**
 * @brief CodeEdit::onTextChanged
 */
void CodeEdit::onTextChanged()
{
    emit codeEditTextChanged(true);
}

/**
 * @brief CodeEdit::onContentChanged
 */
void CodeEdit::onContentChanged()
{
    if (this->tempFile.open()) {
        this->tempFile.write(this->toPlainText().toLocal8Bit());
        lspClient->documentColor(DocumentUri(QFileInfo(tempFile).absoluteFilePath().toStdString()));
        this->tempFile.close();
    }
    //lspClient->documentColor(DocumentUri(openedFileInfo.absoluteFilePath().toStdString()));
}

/**
 * @brief CodeEdit::onLSPClientDocumentColorResponseReceived
 * @param id
 * @param response
 */
void CodeEdit::onLSPClientDocumentColorResponseReceived(QString id, QJsonArray response)
{
//    qDebug() << "received:";
//    qDebug() << "id: " << id;
//    qDebug() << "response: " << response;
//    this->highlighter->setColorInformations(response);

    disconnect(this->document(), &QTextDocument::contentsChanged, this, &CodeEdit::onContentChanged);
    qDebug() << "highlightBlock: ";
    QTextCharFormat textCharFormat;
    QVariantList colorInforList = response.toVariantList();
    for (auto iter = colorInforList.begin(); iter != colorInforList.end(); ++iter) {
        QJsonObject jsonObject = iter->toJsonObject();

        int startLine   = JsonUtil::getValue(jsonObject, "range/start/line").toInt();
        int startOffset = JsonUtil::getValue(jsonObject, "range/start/character").toInt();
        int endLine     = JsonUtil::getValue(jsonObject, "range/end/line").toInt();
        int endOffset   = JsonUtil::getValue(jsonObject, "range/end/character").toInt();
        int red         = JsonUtil::getValue(jsonObject, "color/red").toInt();
        int green       = JsonUtil::getValue(jsonObject, "color/green").toInt();
        int blue        = JsonUtil::getValue(jsonObject, "color/blue").toInt();

        QTextBlock startTextBlock = this->document()->findBlockByLineNumber(startLine);
        int startPosition = startTextBlock.position() + startOffset;
        QTextBlock endTextBlock = this->document()->findBlockByLineNumber(endLine);
        int endPosition = endTextBlock.position() + endOffset;

        textCharFormat.setForeground(QColor(red, green, blue, 255));

        QTextCursor textCursor = this->textCursor();
        textCursor.setPosition(startPosition, QTextCursor::MoveAnchor);
        textCursor.setPosition(endPosition, QTextCursor::KeepAnchor);
        textCursor.setCharFormat(textCharFormat);
        textCursor.clearSelection();
    }
    connect(this->document(), &QTextDocument::contentsChanged, this, &CodeEdit::onContentChanged);
}
