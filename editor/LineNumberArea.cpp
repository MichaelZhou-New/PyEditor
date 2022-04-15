#include "LineNumberArea.h"

LineNumberArea::LineNumberArea(CodeEdit *editor)
        : QWidget(editor), codeEdit(editor) {
}

QSize LineNumberArea::sizeHint() const
{
    return QSize(codeEdit->lineNumberAreaWidth(), 0);
}

void LineNumberArea::paintEvent(QPaintEvent *event)
{
    codeEdit->lineNumberAreaPaintEvent(event);
}
