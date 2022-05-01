#include "CategoryListView.h"

#include "CategoryListViewDelegate.h"

CategoryListView::CategoryListView()
{
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
    setItemDelegate(new CategoryListViewDelegate(this));
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}
