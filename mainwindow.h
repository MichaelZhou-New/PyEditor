#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSplitter>
#include <QTreeView>
#include <QFileSystemModel>
#include <QModelIndex>

#include "editor/TabManager.h"
#include "fileBrowser/FileBrowserSortFilterProxyModel.h"
#include "statusBar/CursorInfoWidget.h"

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
    void onTabManagerCursorPositionChanged(int line, int col);

private:
    Ui::MainWindow *ui;
    QSplitter *splitter;
    QTreeView *fileBrowser;
    FileBrowserSortFilterProxyModel *fileBrowserSortFilterProxyModel;
    QFileSystemModel *fileSystemModel;
    TabManager *tabManager;

    CursorInfoWidget *cursorInfoWidget;
};
#endif // MAINWINDOW_H
