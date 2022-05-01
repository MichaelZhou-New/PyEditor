#ifndef EDITORSETTINGPAGE_H
#define EDITORSETTINGPAGE_H

#include "SettingPage.h"

#include <QSettings>
#include <QGroupBox>
#include <QLabel>
#include <QFontComboBox>

class EditorSettingPage : public SettingPage
{
public:
    explicit EditorSettingPage(QWidget *parent = nullptr);

    void setupUi();
    void readSettings();
    void apply() override;

private:
    // UI members
    QGroupBox *m_fontGroupBox;
    QLabel *m_fontTypeLabel;
    QFontComboBox *m_fontComboBox;
    QLabel *m_fontSizeLabel;
    QComboBox *m_fontSizeComboBox;

private:
    // non-UI members
    QSettings *m_settings;
    QFont *m_font;
};

#endif // EDITORSETTINGPAGE_H
