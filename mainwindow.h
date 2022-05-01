#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSplitter>
#include <QTreeView>
#include <QFileSystemModel>
#include <QModelIndex>

#include "editor/TabManager.h"
#include "fileBrowser/FileBrowserSortFilterProxyModel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void onOpenFolderTriggered();
    void onSettingsTriggered();
    void onTreeViewDoubleClicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;
    QSplitter *splitter;
    QTreeView *fileBrowser;
    FileBrowserSortFilterProxyModel *fileBrowserSortFilterProxyModel;
    QFileSystemModel *fileSystemModel;
    TabManager *tabManager;
};
#endif // MAINWINDOW_H
