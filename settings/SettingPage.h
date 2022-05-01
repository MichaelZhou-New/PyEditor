#ifndef SETTINGPAGE_H
#define SETTINGPAGE_H

#include <QTabWidget>

class SettingPage : public QTabWidget
{
public:
    SettingPage(QWidget *parent = nullptr);

    virtual void apply() { };
};

#endif // SETTINGPAGE_H
