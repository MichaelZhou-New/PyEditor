#ifndef CATEGORYLISTVIEWDELEGATE_H
#define CATEGORYLISTVIEWDELEGATE_H

#include <QStyledItemDelegate>

class CategoryListViewDelegate : public QStyledItemDelegate
{
public:
    explicit CategoryListViewDelegate(QObject *parent = nullptr);

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override
    {
        QSize size = QStyledItemDelegate::sizeHint(option, index);
        size.setHeight(qMax(size.height(), 32));
        return size;
    }
};

#endif // CATEGORYLISTVIEWDELEGATE_H
