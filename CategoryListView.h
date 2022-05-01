#ifndef CATEGORYLISTVIEW_H
#define CATEGORYLISTVIEW_H

#include <QListView>
#include <QScrollBar>

class CategoryListView : public QListView
{
public:
    CategoryListView();

    QSize sizeHint() const final
    {
        int width = sizeHintForColumn(0) + frameWidth() * 2 + 5;
        width += verticalScrollBar()->sizeHint().width();
        return QSize(width, 100);
    }
};

#endif // CATEGORYLISTVIEW_H
