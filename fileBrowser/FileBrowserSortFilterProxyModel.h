#ifndef FILEBROWSERSORTFILTERPROXYMODEL_H
#define FILEBROWSERSORTFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>

class FileBrowserSortFilterProxyModel : public QSortFilterProxyModel
{
public:
    explicit FileBrowserSortFilterProxyModel(QObject *parent = nullptr);

    // reimplement lessThan()
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const;
};

#endif // FILEBROWSERSORTFILTERPROXYMODEL_H
