#include <QFileDialog>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QMessageBox>
#include <QDialogButtonBox>
#include <QDebug>

#include "TabManager.h"
#include "CodeEdit.h"

/**
 * @brief TabManager::TabManager
 * @param parent
 */
TabManager::TabManager(QWidget *parent)
    : QTabWidget(parent)
{
    this->setFont(QFont("Microsoft YaHei", 10, QFont::Black));
    this->setMovable(true);
    this->setTabsClosable(true);

    connect(this, &TabManager::tabCloseRequested, this, &TabManager::onCloseTabRequest);
    connect(this, &TabManager::currentChanged, this, &TabManager::onCodeEditCursorPositionChanged);

    qDebug() << "tabManager: " << Qt::hex << this;
}

/**
 * @brief TabManager::openFile
 * @param fileAbsolutePath
 */
void TabManager::openFile(const QString &fileAbsolutePath)
{
    QFile file(fileAbsolutePath);
    QFileInfo fileInfo(fileAbsolutePath);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot open file: " + file.errorString());
        return;
    }

    QTextStream in(&file);

    // add new tab
    CodeEdit *currentCodeEdit = new CodeEdit(this, fileAbsolutePath);

    this->addTab(currentCodeEdit, fileInfo.fileName());
    this->setCurrentWidget(currentCodeEdit);
    currentCodeEdit->setPlainText(in.readAll());

    file.close();

    connect(currentCodeEdit, &CodeEdit::modificationChanged, this, &TabManager::onCodeEditChanged);
    connect(currentCodeEdit, &CodeEdit::cursorPositionChanged, this, &TabManager::onCodeEditCursorPositionChanged);
}

/**
 * @brief TabManager::openFiles
 * @param fileAbsolutePaths
 */
void TabManager::openFiles(const QStringList &fileAbsolutePaths)
{
    for (QStringList::size_type i = 0; i < fileAbsolutePaths.size(); i++) {
        const QString &filePath = fileAbsolutePaths[i];
        QFile file(filePath);
        QFileInfo fileInfo(filePath);
        if (!file.open(QFile::ReadOnly | QFile::Text)) {
            QMessageBox::warning(this, "Warning", "Cannot open file: " + file.errorString());
            continue;
        }

        QTextStream in(&file);

        // add new tab
        CodeEdit *currentCodeEdit = new CodeEdit(this, filePath);

        this->addTab(currentCodeEdit, fileInfo.fileName());
        this->setCurrentWidget(currentCodeEdit);
        currentCodeEdit->setPlainText(in.readAll());

        file.close();

        connect(currentCodeEdit, &CodeEdit::modificationChanged, this, &TabManager::onCodeEditChanged, Qt::UniqueConnection);
        connect(currentCodeEdit, &CodeEdit::cursorPositionChanged, this, &TabManager::onCodeEditCursorPositionChanged);
    }
}

/**
 * handle closeTabRequest signal
 * @brief TabManager::onCloseTabRequest
 * @param tabIndex
 */
void TabManager::onCloseTabRequest(int tabIndex)
{
    // get current tab from tabManager
    CodeEdit *currentCodeEdit = static_cast<CodeEdit*>(this->widget(tabIndex));

    if (currentCodeEdit->textHasChanged()) {
        QMessageBox messageBox;
        messageBox.setText(tr("是否要保存对 ") + currentCodeEdit->openedFileInfo.fileName() + tr(" 的更改？"));
        messageBox.setInformativeText(tr("如果不保存，你的更改将丢失。"));
        messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        messageBox.setIcon(QMessageBox::Warning);
        messageBox.exec();

        if (!(messageBox.clickedButton() == messageBox.button(QMessageBox::Cancel))) {
            if (messageBox.clickedButton() == messageBox.button(QMessageBox::Yes)) {
                this->onSaveFileActionTriggered();
            }
            this->removeTab(tabIndex);
            delete this->widget(tabIndex);
        }
    } else {
        this->removeTab(tabIndex);
        delete this->widget(tabIndex);
    }
}

/**
 * @brief TabManager::onCodeEditModified
 * @param hasModified
 */
void TabManager::onCodeEditChanged(bool changed)
{
    CodeEdit *currentCodeEdit = static_cast<CodeEdit*>(this->currentWidget());
    int tabIndex = this->indexOf(currentCodeEdit);

    bool oldChanged = currentCodeEdit->textHasChanged();

    if (tabIndex != -1) {
        if (oldChanged== true && changed == false) {
            // change file name
            this->setTabText(tabIndex, currentCodeEdit->openedFileInfo.fileName());
            connect(currentCodeEdit, &CodeEdit::codeEditTextChanged, this, &TabManager::onCodeEditChanged, Qt::UniqueConnection);
        } else if (oldChanged== false && changed == true) {
            // change file name
            this->setTabText(tabIndex, currentCodeEdit->openedFileInfo.fileName() + " *");
            disconnect(currentCodeEdit, &CodeEdit::codeEditTextChanged, this, &TabManager::onCodeEditChanged);
        }
        currentCodeEdit->setTextChangedStatus(changed);
    }
}

/**
 * add new file
 * @brief TabManager::onNewFileActionTriggered
 */
void TabManager::onNewFileActionTriggered()
{
    CodeEdit *currentCodeEdit = new CodeEdit(this, "untitled");
    this->addTab(currentCodeEdit, tr("untitled"));
    this->setCurrentWidget(currentCodeEdit);

    connect(currentCodeEdit, &CodeEdit::codeEditTextChanged, this, &TabManager::onCodeEditChanged, Qt::UniqueConnection);
    connect(currentCodeEdit, &CodeEdit::cursorPositionChanged, this, &TabManager::onCodeEditCursorPositionChanged);
}

/**
 *  open file from disk, create a new file tab,
 *  read text and then put text in codeedit
 * @brief TabManager::onOpenFileActionTriggered
 */
void TabManager::onOpenFileActionTriggered()
{
    QFileDialog fileDialog;
    fileDialog.setFileMode(QFileDialog::ExistingFiles);

    QStringList fileAbsolutePaths(fileDialog.getOpenFileNames());

    this->openFiles(fileAbsolutePaths);
}

/**
 * save file
 * @brief TabManager::onSaveFileActionTriggered
 */
void TabManager::onSaveFileActionTriggered()
{
    CodeEdit *currentCodeEdit = static_cast<CodeEdit*>(this->currentWidget());

    if (currentCodeEdit->openedFileInfo.isFile() == false) {
        this->onSaveFileAsActionTriggered();
        return;
    }

    QFile file(currentCodeEdit->openedFileInfo.absoluteFilePath(), this);

    if (!file.open(QFile::WriteOnly)) {
        QMessageBox::warning(this, "Warning", "Cannot save file: " + file.errorString());
        return;
    }

    file.write(currentCodeEdit->toPlainText().toLocal8Bit());
    file.close();

    this->onCodeEditChanged(false);
}

/**
 * save file as other file
 * @brief TabManager::onSaveFileAsActionTriggered
 */
void TabManager::onSaveFileAsActionTriggered()
{

    QFileDialog fileDialog;
    QString fileAbsolutePath(fileDialog.getSaveFileName(this));

    QFile file(fileAbsolutePath);
    QFileInfo fileInfo(fileAbsolutePath);

    if (fileInfo.fileName().length() > 0) {
        if (!file.open(QFile::WriteOnly | QFile::Text)) {
            QMessageBox::warning(this, "Warning", "Cannot save file: " + file.errorString());
            return;
        }

        CodeEdit *currentCodeEdit = static_cast<CodeEdit*>(this->currentWidget());

        QTextStream out(&file);

        out << currentCodeEdit->toPlainText();
        currentCodeEdit->openedFileInfo = QFileInfo(fileAbsolutePath);
        this->setTabText(this->currentIndex(), fileInfo.fileName());

        file.close();

        this->onCodeEditChanged(false);
    }

}

/**
 * @brief TabManager::onCodeEditCursorPositionChanged
 */
void TabManager::onCodeEditCursorPositionChanged()
{
    if (this->count() > 0) {
        CodeEdit *currentCodeEdit = static_cast<CodeEdit*>(this->currentWidget());

        QTextCursor textCursor = currentCodeEdit->textCursor();
        int line = textCursor.blockNumber();
        int col = textCursor.columnNumber();

        qDebug() << "line: " << line << " col: " << col;
        emit this->codeEditCursorChanged(line, col);
    }
}
