#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QRegularExpression>
#include <QTextDocument>
#include <QObject>
#include <QJsonArray>

#include "lsp-cpp/include/LSP.hpp"

class Highlighter : public QSyntaxHighlighter
{
public:
    explicit Highlighter(QTextDocument *parent = nullptr);
    void setColorInformations(const QJsonArray& colorInfor);

protected:
    void highlightBlock(const QString &text) override;

private:
    void highlightByPos(int startLine, int startOffset, int endLine, int endOffset);

private:
    QJsonArray colorInformations;

    struct HighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;

    QRegularExpression commentStartExpression;
    QRegularExpression commentEndExpression;

    QTextCharFormat keywordFormat;
    QTextCharFormat classFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat multiLineCommentFormat;
    QTextCharFormat quotationFormat;
    QTextCharFormat functionFormat;

    // highlight format
    QTextCharFormat m_commentFormat;
    QTextCharFormat m_errorFormat;
    QTextCharFormat m_keywordFormat;
    QTextCharFormat m_operatorFormat;

    QTextCharFormat m_commentHashbangFormat;
    QTextCharFormat m_commentMultilineFormat;
    QTextCharFormat m_commentPreprocFormat;
    QTextCharFormat m_commentPreprocFileFormat;
    QTextCharFormat m_commentSingleFormat;
    QTextCharFormat m_commentSpecialFormat;

    QTextCharFormat m_genericDeletedFormat;
    QTextCharFormat m_genericEmphFormat;
    QTextCharFormat m_genericErrorFormat;
    QTextCharFormat m_genericHeadingFormat;
    QTextCharFormat m_genericInsertedFormat;
    QTextCharFormat m_genericOutputFormat;
    QTextCharFormat m_genericFormat;
    QTextCharFormat m_genericPromptFormat;
    QTextCharFormat m_genericStrongFormat;
    QTextCharFormat m_genericSubheadingFormat;
    QTextCharFormat m_genericTracebackFormat;

    QTextCharFormat m_keywordConstantFormat;
    QTextCharFormat m_keywordDeclarationFormat;
    QTextCharFormat m_keywordNamespaceFormat;
    QTextCharFormat m_keywordPseudoFormat;
    QTextCharFormat m_keywordReservedFormat;
    QTextCharFormat m_keywordTypeFormat;

    QTextCharFormat m_literalNumberFormat;
    QTextCharFormat m_literalStringFormat;

    QTextCharFormat m_nameAttributeFormat;
    QTextCharFormat m_nameBuiltinFormat;
    QTextCharFormat m_nameClassFormat;
    QTextCharFormat m_nameConstantFormat;
    QTextCharFormat m_nameDecoratorFormat;
    QTextCharFormat m_nameEntityFormat;
    QTextCharFormat m_nameExceptionFormat;
    QTextCharFormat m_nameFunctionFormat;
    QTextCharFormat m_Format;
//    QTextCharFormat m_Format;
//    QTextCharFormat m_Format;
//    QTextCharFormat m_Format;
//    QTextCharFormat m_Format;
//    QTextCharFormat m_Format;
//    QTextCharFormat m_Format;
//    QTextCharFormat m_Format;
//    QTextCharFormat m_Format;
//    QTextCharFormat m_Format;

};

#endif // HIGHLIGHTER_H
