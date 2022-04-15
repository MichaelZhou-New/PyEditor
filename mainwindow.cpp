#include <QPlainTextEdit>
#include <QFont>
#include <QFileDialog>
#include <QMessageBox>
#include <QLayout>
#include <QSplitter>
#include <QTreeView>
#include <QModelIndex>
#include <QDir>
#include <QFileDialog>

#include <QDebug>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "editor/TabManager.h"
#include "editor/CodeEdit.h"
#include "fileBrowser/FileBrowserSortFilterProxyModel.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      splitter(new QSplitter(this)),
      fileBrowser(new QTreeView),
      fileBrowserSortFilterProxyModel(new FileBrowserSortFilterProxyModel),
      fileSystemModel(new QFileSystemModel),
      tabManager(new TabManager)
{
    // setting parents
    tabManager->setParent(splitter);
    fileBrowser->setParent(splitter);
    fileSystemModel->setParent(fileBrowser);

    // setting splitter
    splitter->addWidget(fileBrowser);
    splitter->addWidget(tabManager);
    splitter->setCollapsible(splitter->indexOf(tabManager), false);
    splitter->setHandleWidth(1);

    fileSystemModel->setRootPath("");
    fileSystemModel->sort(0, Qt::DescendingOrder);

    fileBrowserSortFilterProxyModel->setSourceModel(fileSystemModel);
    fileBrowser->setModel(fileBrowserSortFilterProxyModel);

    const QModelIndex rootIndex = fileSystemModel->index("E:\\Personal\\Workspaces\\CppPrimer\\SourceCode\\");
    if (rootIndex.isValid()) {
        fileBrowser->setRootIndex(fileBrowserSortFilterProxyModel->mapFromSource(rootIndex));
    }

    // setting fileBrowser
    for (int i = 1; i <= this->fileSystemModel->columnCount(); i++) {
        fileBrowser->hideColumn(i);
    }
    fileBrowser->setHeaderHidden(true);
    fileBrowser->setSortingEnabled(true);
    fileBrowser->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    fileBrowser->show();
    fileBrowser->resize(200, fileBrowser->height());

    // setting tabManager
    tabManager->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    tabManager->setMinimumWidth(430);
    tabManager->setMinimumHeight(250);
    tabManager->resize(710, tabManager->height());

    ui->setupUi(this);

    this->setCentralWidget(this->splitter);
    this->setWindowTitle(tr("PyEditor"));

    // set connection
    connect(ui->newFileAction, &QAction::triggered, this->tabManager, &TabManager::onNewFileActionTriggered);
    connect(ui->openFileAction, &QAction::triggered, this->tabManager, &TabManager::onOpenFileActionTriggered);
    connect(ui->openFolderAction, &QAction::triggered, this, &MainWindow::onOpenFolder);
    connect(ui->saveFileAction, &QAction::triggered, this->tabManager, &TabManager::onSaveFileActionTriggered);
    connect(ui->saveFileAsAction, &QAction::triggered, this->tabManager, &TabManager::onSaveFileAsActionTriggered);

    connect(this->fileBrowser, &QTreeView::doubleClicked, this, &MainWindow::onTreeViewDoubleClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onOpenFolder()
{
    QFileDialog dirDialog;
    dirDialog.setFileMode(QFileDialog::DirectoryOnly);

    QString absoluteDir(dirDialog.getExistingDirectory(this));

    if (absoluteDir.length() != 0) {
        const QModelIndex rootIndex = fileSystemModel->index(absoluteDir);
        if (rootIndex.isValid()) {
            fileBrowser->setRootIndex(fileBrowserSortFilterProxyModel->mapFromSource(rootIndex));
        }
    }
}

void MainWindow::onTreeViewDoubleClicked(const QModelIndex &index)
{
    QModelIndex fileSystemModelIndex = fileBrowserSortFilterProxyModel->mapToSource(index);

    QString filePath = fileSystemModel->filePath(fileSystemModelIndex);

    this->tabManager->openFile(filePath);
}
