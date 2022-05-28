#ifndef CODEEDIT_H
#define CODEEDIT_H

#include <QPlainTextEdit>
#include <QFileInfo>
#include <QJsonArray>
#include <QTemporaryFile>

#include "LineNumberArea.h"
#include "lspClient/LSPClient.hpp"

class LineNumberArea;

class CodeEdit : public QPlainTextEdit
{
    Q_OBJECT
public:
    CodeEdit(QWidget *parent = nullptr, const QString &fileAbsolutePath = "");
    ~CodeEdit();

    void readSettings();
    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();
    bool textHasChanged() const;

signals:
    void codeEditTextChanged(bool changed);

public slots:
    void setTextChangedStatus(bool hasModified = true);
    void onTextChanged();
    void onContentChanged();
    void onLSPClientDocumentColorResponseReceived(QString id, QJsonArray response);

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
    bool _textChanged;

    LSPClient *lspClient;
    QTemporaryFile tempFile;
};

#endif // CODEEDIT_H
