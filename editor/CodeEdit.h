#ifndef CODEEDIT_H
#define CODEEDIT_H

#include <QPlainTextEdit>
#include <QFileInfo>

#include "LineNumberArea.h"
#include "../highlighter/highlighter.h"

class LineNumberArea;

class CodeEdit : public QPlainTextEdit
{
    Q_OBJECT
public:
    CodeEdit(QWidget *parent = nullptr, const QString &fileAbsolutePath = "");

    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();
    bool textHasChanged() const;

signals:
    void codeEditTextChanged(bool changed);

public slots:
    void setTextChangedStatus(bool hasModified = true);
    void onTextChanged();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void updateLineNumberArea(const QRect &, int);
    void highlightCurrentLine();

public:
    QFileInfo openedFileInfo;

private:
    LineNumberArea *lineNumberArea;
    Highlighter *highlighter;
    bool _textChanged;
};

#endif // CODEEDIT_H
