#ifndef KITSSETTINGPAGE_H
#define KITSSETTINGPAGE_H

#include "SettingPage.h"

#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include <QSettings>

class KitsSettingPage : public SettingPage
{
    Q_OBJECT
public:
    explicit KitsSettingPage(QWidget *parent = nullptr);

    void setupUi();
    void readSettings();
    void apply() override;

public slots:
    void onClickedPathButton();

signals:
    void interpreterPathChanged(const QString& path);

private:
    // UI member
    QGroupBox *m_kitsGroupBox;
    QLabel *m_pathLabel;
    QLineEdit *m_interpreterPathLineEdit;
    QPushButton *m_pathButton;

private:
    // non-UI member
    QString m_interpreterPath;
    QSettings *m_settings;
};

#endif // KITSSETTINGPAGE_H
