#ifndef APPEARANCESETTINGPAGE_H
#define APPEARANCESETTINGPAGE_H

#include "SettingPage.h"

#include <QSettings>
#include <QGroupBox>
#include <QLabel>
#include <QFontComboBox>

class AppearanceSettingPage : public SettingPage
{
public:
    explicit AppearanceSettingPage(QWidget *parent = nullptr);

    void setupUi();
    void readSettings();
    void apply() override;

private:
    // UI members

private:
    // non-UI members

};

#endif // APPEARANCESETTINGPAGE_H
