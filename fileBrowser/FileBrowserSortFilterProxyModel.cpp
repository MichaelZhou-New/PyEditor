#include "FileBrowserSortFilterProxyModel.h"

#include <QFileSystemModel>
#include <QFileInfo>

FileBrowserSortFilterProxyModel::FileBrowserSortFilterProxyModel(QObject *parent) : QSortFilterProxyModel(parent)
{

}

/**
 * @brief FileBrowserSortFilterProxyModel::lessThan
 * @param left
 * @param right
 * @return
 */
bool FileBrowserSortFilterProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    // If sorting by file names column
    if (sortColumn() == 0) {
        QFileSystemModel *fsm = qobject_cast<QFileSystemModel*>(sourceModel());
        bool asc = sortOrder() == Qt::AscendingOrder ? true : false;

        QFileInfo leftFileInfo  = fsm->fileInfo(left);
        QFileInfo rightFileInfo = fsm->fileInfo(right);

        // If DotAndDot move in the beginning
        if (sourceModel()->data(left).toString() == "..") {
            return asc;
        }
        if (sourceModel()->data(right).toString() == "..") {
            return !asc;
        }

        // Move dirs upper
        if (!leftFileInfo.isDir() && rightFileInfo.isDir()) {
            return !asc;
        } else if (leftFileInfo.isDir() && !rightFileInfo.isDir()) {
            return asc;
        } else {
            QString leftFileName = leftFileInfo.fileName();
            QString rightFileName = rightFileInfo.fileName();
            if (leftFileName[0] < rightFileName[0]) {
                return asc;
            } else {
                return !asc;
            }
        }
    }

    return QSortFilterProxyModel::lessThan(left, right);
}
