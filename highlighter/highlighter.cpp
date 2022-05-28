#include <QSyntaxHighlighter>

#include "highlighter.h"

#include "utils/JsonUtil.h"

Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{



    HighlightingRule rule;
    keywordFormat.setForeground(Qt::darkBlue);
    keywordFormat.setFontWeight(QFont::Bold);
    QStringList keywordPatterns;
//    keywordPatterns << "\\bchar\\b" << "\\bclass\\b" << "\\bconst\\b"
//                    << "\\bdouble\\b" << "\\benum\\b" << "\\bexplicit\\b"
//                    << "\\bfriend\\b" << "\\binline\\b" << "\\bint\\b"
//                    << "\\blong\\b" << "\\bnamespace\\b" << "\\boperator\\b"
//                    << "\\bprivate\\b" << "\\bprotected\\b" << "\\bpublic\\b"
//                    << "\\bshort\\b" << "\\bsignals\\b" << "\\bsigned\\b"
//                    << "\\bslots\\b" << "\\bstatic\\b" << "\\bstruct\\b"
//                    << "\\btemplate\\b" << "\\btypedef\\b" << "\\btypename\\b"
//                    << "\\bunion\\b" << "\\bunsigned\\b" << "\\bvirtual\\b"
//                    << "\\bvoid\\b" << "\\bvolatile\\b" << "\\bbool\\b";
    keywordPatterns << "\\bFalse\\b" << "\\bawait\\b" << "\\beles\\b"
                    << "\\bimport\\b" << "\\bpass\\b" << "\\bNone\\b"
                    << "\\bbreak\\b" << "\\bexcept\\b" << "\\bin\\b"
                    << "\\braise\\b" << "\\bTrue\\b" << "\\bclass\\b"
                    << "\\bfinally\\b" << "\\bis\\b" << "\\breturn\\b"
                    << "\\band\\b" << "\\bdef\\b" << "\\bfrom\\b"
                    << "\\bnonlocal\\b" << "\\bwhile\\b" << "\\bassert\\b"
                    << "\\bdel\\b" << "\\bglobal\\b" << "\\bnot\\b"
                    << "\\bwith\\b" << "\\basync\\b" << "\\belif\\b"
                    << "\\bif\\b" << "\\bor\\b" << "\\byiel\\b";
    foreach (const QString &pattern, keywordPatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    // literal
    classFormat.setFontWeight(QFont::Bold);
    classFormat.setForeground(Qt::darkMagenta);
    rule.pattern = QRegularExpression("\\bQ[A-Za-z]+\\b");
    rule.format = classFormat;
    highlightingRules.append(rule);

    singleLineCommentFormat.setForeground(Qt::red);
    rule.pattern = QRegularExpression("//[^\n]*");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    multiLineCommentFormat.setForeground(Qt::red);

    quotationFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegularExpression("\".*\"");
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    functionFormat.setFontItalic(true);
    functionFormat.setForeground(Qt::blue);
    rule.pattern = QRegularExpression("\\b[A-Za-z0-9_]+(?=\\()");
    rule.format = functionFormat;
    highlightingRules.append(rule);

//    commentStartExpression = QRegularExpression("/\\*");
//    commentEndExpression = QRegularExpression("\\*/");
    commentStartExpression = QRegularExpression("'''");
    commentEndExpression = QRegularExpression("'''");
}

void Highlighter::setColorInformations(const QJsonArray &colorInfor)
{
    qDebug() << "setColorInformations";
    this->colorInformations = colorInfor;
//    this->highlightBlock(this->document()->toPlainText());
}

void Highlighter::highlightByPos(int startLine, int startOffset, int endLine, int endOffset)
{
    for (int i = startLine; i <= endLine; i++) {
        QTextBlock startTextBlock = this->document()->findBlockByLineNumber(startLine);
        int startPosition = startTextBlock.position() + startOffset;
        QTextBlock endTextBlock = this->document()->findBlockByLineNumber(endLine);
        int endPosition = endTextBlock.position() + endOffset;

//        this->setFormat(startPosition, endPosition - startPosition, QColor)
    }
}

void Highlighter::highlightBlock(const QString &text)
{
//    int currentLineNumber = currentBlock().blockNumber();
//    qDebug() << "highlightBlock: ";
//    qDebug() << text;
//    QTextCharFormat textCharFormat;
//    QVariantList colorInforList = colorInformations.toVariantList();
//    for (auto iter = colorInforList.begin(); iter != colorInforList.end(); ++iter) {
//        QJsonObject jsonObject = iter->toJsonObject();

//        int startLine   = JsonUtil::getValue(jsonObject, "range/start/line").toInt();
//        int startOffset = JsonUtil::getValue(jsonObject, "range/start/character").toInt();
//        int endLine     = JsonUtil::getValue(jsonObject, "range/end/line").toInt();
//        int endOffset   = JsonUtil::getValue(jsonObject, "range/end/character").toInt();
//        int red         = JsonUtil::getValue(jsonObject, "color/red").toInt();
//        int green       = JsonUtil::getValue(jsonObject, "color/green").toInt();
//        int blue        = JsonUtil::getValue(jsonObject, "color/blue").toInt();

////        qDebug() << "startLine: " << startLine;
////        qDebug() << "startOffset: " << startOffset;
////        qDebug() << "endLine: " << endLine;
////        qDebug() << "endOffset: " << endOffset;
////        qDebug() << "red: " << red;
////        qDebug() << "green: " << green;
////        qDebug() << "blue: " << blue;

//        QTextBlock startTextBlock = this->document()->findBlockByLineNumber(startLine);
//        int startPosition = startTextBlock.position() + startOffset;
//        QTextBlock endTextBlock = this->document()->findBlockByLineNumber(endLine);
//        int endPosition = endTextBlock.position() + endOffset;

////        qDebug() << "setFormat: ";
////        qDebug() << "startPos: " << startPosition << " endPos: " << endPosition;
////        qDebug() << "len: " << endPosition - startPosition;
////        this->setFormat(startPosition, endPosition - startPosition, QColor(red, green, blue, 255));
//        textCharFormat.setForeground(QColor(red, green, blue, 255));
//        this->setFormat(startPosition, endPosition - startPosition + 1, textCharFormat);
//        this->setFormat(0, text.length(), Qt::red);
////        this->setFormat(startPosition, endPosition - startPosition, textCharFormat);
//    }



//    foreach (const HighlightingRule &rule, highlightingRules) {
//        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
//        while (matchIterator.hasNext()) {
//            QRegularExpressionMatch match = matchIterator.next();
//            this->setFormat(match.capturedStart(), match.capturedLength(), rule.format);
//        }
//    }
//    this->setCurrentBlockState(0);

//    int startIndex = 0;
//    if (previousBlockState() != 1)
//        startIndex = text.indexOf(commentStartExpression);

//    while (startIndex >= 0) {
//        QRegularExpressionMatch match = commentEndExpression.match(text, startIndex);
//        int endIndex = match.capturedStart();
//        int commentLength = 0;
//        if (endIndex == -1) {
//            setCurrentBlockState(1);
//            commentLength = text.length() - startIndex;
//        } else {
//            commentLength = endIndex - startIndex
//                            + match.capturedLength();
//        }
//        this->setFormat(startIndex, commentLength, multiLineCommentFormat);
//        startIndex = text.indexOf(commentStartExpression, startIndex + commentLength);
//    }
}
