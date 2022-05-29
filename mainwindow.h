#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSplitter>
#include <QTreeView>
#include <QFileSystemModel>
#include <QModelIndex>
#include <QLineEdit>
#include <QRegExp>

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

    void setupToolBars();
    void setupUi();

public slots:
    void onOpenFolderTriggered();
    void onSettingsTriggered();
    void onTreeViewDoubleClicked(const QModelIndex &index);
    void onTabManagerCursorPositionChanged(int line, int col);
    void onExecuteActionTriggered();
    void onOpenConsoleActionTriggered();
    void onPrevResultActionTriggered();
    void onNextResultActionTriggered();

private:
    Ui::MainWindow *ui;
    QSplitter *splitter;
    QTreeView *fileBrowser;
    FileBrowserSortFilterProxyModel *fileBrowserSortFilterProxyModel;
    QFileSystemModel *fileSystemModel;
    TabManager *tabManager;

    CursorInfoWidget *cursorInfoWidget;
    QLineEdit *m_searchLineEdit;
    QRegExp m_regExp;
};
#endif // MAINWINDOW_H
