#ifndef CURSORINFOWIDGET_H
#define CURSORINFOWIDGET_H

#include <QWidget>
#include <QLabel>

class CursorInfoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CursorInfoWidget(QWidget *parent = nullptr);

    void setupUi();
    void setLine(int line);
    void setColumn(int col);
    void setCursorPosition(int line, int col);

signals:

public slots:
    void onCursorChanged(int line, int col);

private:
    // UI members
    QLabel *m_lineTitleLabel;
    QLabel *m_lineNumberLabel;
    QLabel *m_colTitleLabel;
    QLabel *m_colNumberLabel;

private:
    // non-UI members
    int m_line;
    int m_column;
};

#endif // CURSORINFOWIDGET_H
