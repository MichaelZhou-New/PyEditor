#include <QFileDialog>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QMessageBox>
#include <QDialogButtonBox>
#include <QDebug>

#include "TabManager.h"
#include "CodeEdit.h"

TabManager::TabManager(QWidget *parent)
    : QTabWidget(parent)
{
    this->setFont(QFont("Microsoft YaHei", 10, QFont::Black));
    this->setMovable(true);
    this->setTabsClosable(true);

    connect(this, &TabManager::tabCloseRequested, this, &TabManager::onCloseTabRequest);
}

void TabManager::openFile(const QString &fileAbsolutePath)
{
    QFile file(fileAbsolutePath);
    QFileInfo fileInfo(fileAbsolutePath);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot open file: " + file.errorString());
        return;
    }

    QTextStream in(&file);

    // add new tab
    CodeEdit *currentCodeEdit = new CodeEdit(this, fileAbsolutePath);

    this->addTab(currentCodeEdit, fileInfo.fileName());
    this->setCurrentWidget(currentCodeEdit);
    currentCodeEdit->setPlainText(in.readAll());

    file.close();
}

void TabManager::openFiles(const QStringList &fileAbsolutePaths)
{
    for (QStringList::size_type i = 0; i < fileAbsolutePaths.size(); i++) {
        const QString &filePath = fileAbsolutePaths[i];
        QFile file(filePath);
        QFileInfo fileInfo(filePath);
        if (!file.open(QFile::ReadOnly | QFile::Text)) {
            QMessageBox::warning(this, "Warning", "Cannot open file: " + file.errorString());
            continue;
        }

        QTextStream in(&file);

        // add new tab
        CodeEdit *currentCodeEdit = new CodeEdit(this, filePath);

        this->addTab(currentCodeEdit, fileInfo.fileName());
        this->setCurrentWidget(currentCodeEdit);
        currentCodeEdit->setPlainText(in.readAll());

        file.close();
    }
}

/**
 * handle closeTabRequest signal
 * @brief TabManager::onCloseTabRequest
 * @param tabIndex
 */
void TabManager::onCloseTabRequest(int tabIndex)
{
    // get current tab from tabManager
    CodeEdit *currentCodeEdit = static_cast<CodeEdit*>(this->widget(tabIndex));

    QMessageBox messageBox;
    messageBox.setText(tr("是否要保存对 ") + currentCodeEdit->openedFileInfo.fileName() + tr(" 的更改？"));
    messageBox.setInformativeText(tr("如果不保存，你的更改将丢失。"));
    messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
    messageBox.setIcon(QMessageBox::Warning);
    messageBox.exec();

    if (!(messageBox.clickedButton() == messageBox.button(QMessageBox::Cancel))) {
        if (messageBox.clickedButton() == messageBox.button(QMessageBox::Yes)) {
            this->onSaveFileActionTriggered();
        }
        this->removeTab(tabIndex);
        delete this->widget(tabIndex);
    }
}

/*
 * add new file
 */
void TabManager::onNewFileActionTriggered()
{
    CodeEdit *currentCodeEdit = new CodeEdit(this);
    this->addTab(currentCodeEdit, tr("untitled"));
    this->setCurrentWidget(currentCodeEdit);
}

/*
 *  open file from disk, create a new file tab,
 *  read text and then put text in codeedit
 */
void TabManager::onOpenFileActionTriggered()
{
    QFileDialog fileDialog;
    fileDialog.setFileMode(QFileDialog::ExistingFiles);

    QStringList fileAbsolutePaths(fileDialog.getOpenFileNames());

    this->openFiles(fileAbsolutePaths);

//    for (QStringList::size_type i = 0; i < fileAbsolutePaths.size(); i++) {
//        const QString &filePath = fileAbsolutePaths[i];
//        QFile file(filePath);
//        QFileInfo fileInfo(filePath);
//        if (!file.open(QFile::ReadOnly | QFile::Text)) {
//            QMessageBox::warning(this, "Warning", "Cannot open file: " + file.errorString());
//            continue;
//        }

//        QTextStream in(&file);

//        // add new tab
//        CodeEdit *currentCodeEdit = new CodeEdit(this, filePath);

//        this->addTab(currentCodeEdit, fileInfo.fileName());
//        this->setCurrentWidget(currentCodeEdit);
//        currentCodeEdit->setPlainText(in.readAll());

//        file.close();
//    }
}

/*
 * save file
 **/
void TabManager::onSaveFileActionTriggered()
{
    CodeEdit *currentCodeEdit = static_cast<CodeEdit*>(this->currentWidget());

    if (currentCodeEdit->openedFileInfo.size() == 0) {
        this->onSaveFileAsActionTriggered();
        return;
    }

    QFile file(currentCodeEdit->openedFileInfo.absoluteFilePath(), this);

    if (!file.open(QFile::WriteOnly)) {
        QMessageBox::warning(this, "Warning", "Cannot save file: " + file.errorString());
        return;
    }

    file.write(currentCodeEdit->toPlainText().toLocal8Bit());
    file.close();
}

/*
 * save file as other file
 * */
void TabManager::onSaveFileAsActionTriggered()
{

    QFileDialog fileDialog;
    QString fileAbsolutePath(fileDialog.getSaveFileName(this));

    QFile file(fileAbsolutePath);
    QFileInfo fileInfo(fileAbsolutePath);

    if (fileInfo.fileName().length() > 0) {
        if (!file.open(QFile::WriteOnly | QFile::Text)) {
            QMessageBox::warning(this, "Warning", "Cannot save file: " + file.errorString());
            return;
        }

        CodeEdit *currentCodeEdit = static_cast<CodeEdit*>(this->currentWidget());

        QTextStream out(&file);

        out << currentCodeEdit->toPlainText();
        this->setTabText(this->currentIndex(), fileInfo.fileName());

        file.close();
    }
}
