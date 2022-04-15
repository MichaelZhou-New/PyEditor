#ifndef LINENUMBERAREA_H
#define LINENUMBERAREA_H

#include <QWidget>
#include <QSize>

#include "CodeEdit.h"

class CodeEdit;

class LineNumberArea : public QWidget
{
    Q_OBJECT
public:
    explicit LineNumberArea(CodeEdit *editor = nullptr);

    QSize sizeHint() const override;

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    CodeEdit *codeEdit;
};

#endif // LINENUMBERAREA_H
