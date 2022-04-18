#ifndef TABMANAGER_H
#define TABMANAGER_H

#include <QTabWidget>

class TabManager : public QTabWidget
{
    Q_OBJECT
public:
    explicit TabManager(QWidget *parent = nullptr);

    void openFile(const QString &fileAbsolutePath);
    void openFiles(const QStringList &fileAbsolutePaths);

public slots:
    void onCloseTabRequest(int tabIndex);
    void onCodeEditChanged(bool changed = true);
    void onNewFileActionTriggered();
    void onOpenFileActionTriggered();
    void onSaveFileActionTriggered();
    void onSaveFileAsActionTriggered();
};

#endif // TABMANAGER_H
