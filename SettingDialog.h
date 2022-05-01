#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include <QStyledItemDelegate>

#include <QListView>
#include <QListWidget>
#include <QScrollBar>

#include <QDialog>
#include <QLabel>
#include <QListView>
#include <QStackedWidget>
#include <QCloseEvent>

class CategoryListViewDelegate : public QStyledItemDelegate
{
public:
    explicit CategoryListViewDelegate(QObject *parent)
        : QStyledItemDelegate(parent) { }

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override
    {
        QSize size = QStyledItemDelegate::sizeHint(option, index);
        size.setHeight(qMax(size.height(), 32));
        return size;
    }
};

class CategoryListView : public QListView
{
public:
    CategoryListView()
    {
        setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
        setItemDelegate(new CategoryListViewDelegate(this));
        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }

    QSize sizeHint() const final
    {
        int width = sizeHintForColumn(0) + frameWidth() * 2 + 5;
        width += verticalScrollBar()->sizeHint().width();
        return QSize(width, 100);
    }
};


class SettingDialog : public QDialog
{
public:
    explicit SettingDialog(QWidget *parent = nullptr);

    void setupUi();

    QSize sizeHint() const final {
        return this->minimumSize();
    }

    void apply();

    void accept() override final;
    void reject() override final;

public slots:
    void onSettingListItemClicked(QListWidgetItem *item);
    void onSettingListCurrentRowChanged(int row);

private:
    QLabel *m_headerLabel;
    CategoryListView *m_categoryList;
    QListWidget *m_settingList;
    QStackedWidget *m_stackedWidget;
};

#endif // SETTINGDIALOG_H
