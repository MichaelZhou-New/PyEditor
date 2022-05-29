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
#include <QTextCursor>
#include <QSpacerItem>
#include <QToolBar>
#include <QProcess>
#include <QSettings>
#include <QLineEdit>
#include <QAction>
#include <cstdlib>

#include <QDebug>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "editor/TabManager.h"
#include "editor/CodeEdit.h"
#include "fileBrowser/FileBrowserSortFilterProxyModel.h"
#include "SettingDialog.h"
#include "statusBar/CursorInfoWidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      splitter(new QSplitter(this)),
      fileBrowser(new QTreeView),
      fileBrowserSortFilterProxyModel(new FileBrowserSortFilterProxyModel),
      fileSystemModel(new QFileSystemModel),
      tabManager(new TabManager(this)),
      cursorInfoWidget(new CursorInfoWidget(this)),
      m_searchLineEdit(new QLineEdit(this))
{
    ui->setupUi(this);
    this->setupToolBars();
    this->setupUi();

    QDir::setCurrent(QDir::homePath());

    // set connections
    connect(ui->newFileAction, &QAction::triggered, this->tabManager, &TabManager::onNewFileActionTriggered);
    connect(ui->openFileAction, &QAction::triggered, this->tabManager, &TabManager::onOpenFileActionTriggered);
    connect(ui->openFolderAction, &QAction::triggered, this, &MainWindow::onOpenFolderTriggered);
    connect(ui->saveFileAction, &QAction::triggered, this->tabManager, &TabManager::onSaveFileActionTriggered);
    connect(ui->saveFileAsAction, &QAction::triggered, this->tabManager, &TabManager::onSaveFileAsActionTriggered);

    connect(ui->settingsAction, &QAction::triggered, this, &MainWindow::onSettingsTriggered);
    connect(this->fileBrowser, &QTreeView::doubleClicked, this, &MainWindow::onTreeViewDoubleClicked);

    tabManager->onNewFileActionTriggered();
}

MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief MainWindow::setupToolBars
 */
void MainWindow::setupToolBars()
{
    QToolBar *editToolBar = addToolBar("Edit");
    editToolBar->addAction(ui->undoAction);
    editToolBar->addAction(ui->redoAction);

    QToolBar *kitsToolBar = addToolBar("Kits");
    kitsToolBar->addAction(tr("&执行"), this, &MainWindow::onExecuteActionTriggered);
    kitsToolBar->addAction(tr("&控制台"), this, &MainWindow::onOpenConsoleActionTriggered);

    QToolBar *searchToolBar = addToolBar("search");
    m_searchLineEdit->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    searchToolBar->addWidget(m_searchLineEdit);
//    QAction *prevResultAction = new QAction(tr("上一个"), this);
//    QAction *nextResultAction = new QAction(tr("上一个"), this);
//    searchToolBar->addAction(prevResultAction);
//    searchToolBar->addAction(nextResultAction);
    searchToolBar->addAction(tr("&上一个"), this, &MainWindow::onPrevResultActionTriggered);
    searchToolBar->addAction(tr("&下一个"), this, &MainWindow::onNextResultActionTriggered);
}

/**
 * @brief MainWindow::setupUi
 */
void MainWindow::setupUi()
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
    splitter->setStretchFactor(0, 0);
    splitter->setStretchFactor(1, 1);

    fileSystemModel->setRootPath("");
    fileSystemModel->sort(0, Qt::DescendingOrder);

    fileBrowserSortFilterProxyModel->setSourceModel(fileSystemModel);
    fileBrowser->setModel(fileBrowserSortFilterProxyModel);

    const QModelIndex rootIndex = fileSystemModel->index(QDir::homePath());
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
    fileBrowser->setMinimumWidth(180);
    fileBrowser->resize(180, fileBrowser->height());
    fileBrowser->show();

    // setting tabManager
    tabManager->setMinimumWidth(180);
    tabManager->setMinimumHeight(250);
    tabManager->resize(730, tabManager->height());

    this->setCentralWidget(this->splitter);
    this->setWindowTitle(tr("PyEditor"));

    // setting cursorInfoWidget
    statusBar()->addPermanentWidget(cursorInfoWidget);
    connect(tabManager, &TabManager::codeEditCursorChanged, this, &MainWindow::onTabManagerCursorPositionChanged);
}

/**
 * @brief MainWindow::onOpenFolderTriggered
 */
void MainWindow::onOpenFolderTriggered()
{
    QFileDialog dirDialog;
    dirDialog.setFileMode(QFileDialog::DirectoryOnly);

    QString absoluteDir(dirDialog.getExistingDirectory(this));

    if (absoluteDir.length() != 0) {
        QDir::setCurrent(absoluteDir);
        qDebug() << QDir::currentPath();
        const QModelIndex rootIndex = fileSystemModel->index(absoluteDir);
        if (rootIndex.isValid()) {
            fileBrowser->setRootIndex(fileBrowserSortFilterProxyModel->mapFromSource(rootIndex));
        }
    }
}

/**
 * @brief MainWindow::onTreeViewDoubleClicked
 * @param index
 */
void MainWindow::onTreeViewDoubleClicked(const QModelIndex &index)
{
    QModelIndex fileSystemModelIndex = fileBrowserSortFilterProxyModel->mapToSource(index);

    QString filePath = fileSystemModel->filePath(fileSystemModelIndex);
    this->tabManager->openFile(filePath);
}

/**
 * @brief MainWindow::onSettingsTriggered
 */
void MainWindow::onSettingsTriggered()
{
    SettingDialog *settingDialog = new SettingDialog(this);
    settingDialog->exec();

    for (int i = 0; i < tabManager->count(); i++) {
        CodeEdit *currentCodeEdit = static_cast<CodeEdit*>(tabManager->widget(i));
        currentCodeEdit->readSettings();
    }
}

/**
 * @brief MainWindow::onTabManagerCursorPositionChanged
 * @param line
 * @param col
 */
void MainWindow::onTabManagerCursorPositionChanged(int line, int col)
{
    cursorInfoWidget->setCursorPosition(line, col);
}

/**
 * @brief MainWindow::onExecuteActionTriggered
 */
void MainWindow::onExecuteActionTriggered()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "dylan", "PyEditor");
    QString interpreterPath = settings.value("kits/interpreterPath").toString();
    if (interpreterPath.length() == 0) {
        QMessageBox::warning(this, "Warning", "未指定解释器或解释器路径有误！");
        return;
    }

    CodeEdit *currentCodeEdit = static_cast<CodeEdit*>(tabManager->currentWidget());
    if (!currentCodeEdit->openedFileInfo.isFile()) {
        QMessageBox::warning(this, "Warning", "请先保存文件！");
        return;
    }

    QString program;
    QStringList arguments;
#ifdef _WIN32
    program = "cmd.exe";
    arguments << "/K" << ( "\"" + interpreterPath + "\"") << currentCodeEdit->openedFileInfo.absoluteFilePath();
    qDebug() << program + " " + arguments.join(" ");
    std::system(("start " + program + " " + arguments.join(" ")).toStdString().c_str());
#elif __linux__
    QMessageBox::warning(this, "Warning", "暂不支持Linux");
#else
    QMessageBox::warning(this, "Warning", "该功能仅支持Windows");
#endif
}

/**
 * @brief MainWindow::onOpenConsoleActionTriggered
 */
void MainWindow::onOpenConsoleActionTriggered()
{
    QString program;
    QStringList arguments;
#ifdef _WIN32
    program = "cmd.exe";
    std::system(("start " + program + " " + arguments.join(" ")).toStdString().c_str());
#elif __linux__
    QMessageBox::warning(this, "Warning", "暂不支持Linux");
#else
    QMessageBox::warning(this, "Warning", "该功能仅支持Windows");
#endif
}

/**
 * @brief MainWindow::onPrevResultActionTriggered
 */
void MainWindow::onPrevResultActionTriggered()
{
    CodeEdit *currentCodeEdit = static_cast<CodeEdit*>(tabManager->currentWidget());
    QString text = currentCodeEdit->toPlainText();
    QTextCursor textCursor = currentCodeEdit->textCursor();
    int pos = textCursor.selectionStart();
    if (pos != 0) {
        if (m_searchLineEdit->text() != m_regExp.pattern()) {
            m_regExp.setPattern(m_searchLineEdit->text());
        }
        pos = m_regExp.lastIndexIn(text, pos - 1);
        if (pos != -1) {
            textCursor.setPosition(pos + m_regExp.matchedLength(), QTextCursor::MoveAnchor);
            textCursor.setPosition(pos, QTextCursor::KeepAnchor);
            currentCodeEdit->setTextCursor(textCursor);
        }
    }
}

/**
 * @brief MainWindow::onNextResultActionTriggered
 */
void MainWindow::onNextResultActionTriggered()
{
    CodeEdit *currentCodeEdit = static_cast<CodeEdit*>(tabManager->currentWidget());
    QString text = currentCodeEdit->toPlainText();
    QTextCursor textCursor = currentCodeEdit->textCursor();
    int pos = textCursor.selectionEnd();
    if (pos != text.length()) {
        if (m_searchLineEdit->text() != m_regExp.pattern()) {
            m_regExp.setPattern(m_searchLineEdit->text());
        }
        pos = m_regExp.indexIn(text, pos);
        if (pos != -1) {
            textCursor.setPosition(pos, QTextCursor::MoveAnchor);
            textCursor.setPosition(pos + m_regExp.matchedLength(), QTextCursor::KeepAnchor);
            currentCodeEdit->setTextCursor(textCursor);
        }
    }
}
