#include "LineNumberArea.h"

LineNumberArea::LineNumberArea(CodeEdit *editor)
        : QWidget(editor), codeEdit(editor) {
}

/**
 * @brief LineNumberArea::sizeHint
 * @return
 */
QSize LineNumberArea::sizeHint() const
{
    return QSize(codeEdit->lineNumberAreaWidth(), 0);
}

/**
 * @brief LineNumberArea::paintEvent
 * @param event
 */
void LineNumberArea::paintEvent(QPaintEvent *event)
{
    codeEdit->lineNumberAreaPaintEvent(event);
}
