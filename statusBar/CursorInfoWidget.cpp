#include "CursorInfoWidget.h"

#include <QHBoxLayout>

CursorInfoWidget::CursorInfoWidget(QWidget *parent)
    : QWidget{parent},
      m_line(0),
      m_column(0)
{
    this->setupUi();
}

void CursorInfoWidget::setupUi()
{
    m_lineTitleLabel = new QLabel(tr("line: "), this);
    m_lineNumberLabel = new QLabel(QString::number(m_line), this);
    m_colTitleLabel = new QLabel(tr("col: "), this);
    m_colNumberLabel = new QLabel(QString::number(m_column), this);

    QHBoxLayout *hBoxLayout = new QHBoxLayout;
    this->setLayout(hBoxLayout);

    hBoxLayout->addWidget(m_lineTitleLabel);
    hBoxLayout->addWidget(m_lineNumberLabel);
    hBoxLayout->addWidget(m_colTitleLabel);
    hBoxLayout->addWidget(m_colNumberLabel);
}

void CursorInfoWidget::setLine(int line)
{
    m_line = line;
    m_lineNumberLabel->setText(QString::number(m_line));
}

void CursorInfoWidget::setColumn(int col)
{
    m_column = col;
    m_colNumberLabel->setText(QString::number(m_column));
}

void CursorInfoWidget::setCursorPosition(int line, int col)
{
    m_line = line;
    m_column = col;

    m_lineNumberLabel->setText(QString::number(m_line));
    m_colNumberLabel->setText(QString::number(m_column));
}

void CursorInfoWidget::onCursorChanged(int line, int col)
{
    m_line = line;
    m_column = col;

    m_lineNumberLabel->setText(QString::number(m_line));
    m_colNumberLabel->setText(QString::number(m_column));
}
