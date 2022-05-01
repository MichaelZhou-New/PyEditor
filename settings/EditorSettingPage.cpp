#include "EditorSettingPage.h"

#include <QDebug>

#include <QVBoxLayout>
#include <QStringList>

#include <QSettings>
#include <QVariant>

EditorSettingPage::EditorSettingPage(QWidget *parent)
    : SettingPage(parent),
      m_settings(new QSettings(QSettings::IniFormat, QSettings::UserScope, "dylan", "PyEditor")),
      m_font(new QFont("Arial", 9))
{
    this->setupUi();
    this->readSettings();
}

void EditorSettingPage::setupUi()
{
    m_fontGroupBox = new QGroupBox(tr("字体"), this);
    m_fontTypeLabel = new QLabel(tr("字型："), this) ;
    m_fontComboBox = new QFontComboBox(this);
    m_fontSizeLabel = new QLabel(tr("字号:"));
    m_fontSizeComboBox = new QComboBox(this);

    m_fontComboBox->setCurrentFont(*m_font);
    m_fontComboBox->setSizeAdjustPolicy(QFontComboBox::AdjustToContents);
    m_fontSizeComboBox->addItems([](){
       QStringList fontSizeList;
       for (int size = 2; size < 34; size++) {
           fontSizeList.append(QString::number(size));
       }
       return fontSizeList;
    }());
    m_fontSizeComboBox->setCurrentText(QString::number(m_font->pointSize()));

    // tab1
    QWidget *tab1RootWidget = new QWidget(this);
    this->addTab(tab1RootWidget, tr("通用"));
    QVBoxLayout *vBoxLayout = new QVBoxLayout;
    tab1RootWidget->setLayout(vBoxLayout);

    vBoxLayout->addWidget(m_fontGroupBox);
    vBoxLayout->addSpacerItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding));

    QHBoxLayout *fontGroupHBoxLayout = new QHBoxLayout(m_fontGroupBox);
    fontGroupHBoxLayout->addWidget(m_fontTypeLabel);
    fontGroupHBoxLayout->addWidget(m_fontComboBox);
    fontGroupHBoxLayout->addWidget(m_fontSizeLabel);
    fontGroupHBoxLayout->addWidget(m_fontSizeComboBox);
    fontGroupHBoxLayout->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
}

void EditorSettingPage::readSettings()
{
    QVariant fontFamilyVariant = m_settings->value("editor/fontFamily");
    QVariant fontSizeVariant = m_settings->value("editor/fontSize");
    if (fontFamilyVariant.isNull() == false) {
        m_font->setFamily(fontFamilyVariant.value<QFont>().family());
    }
    if (fontSizeVariant.isNull() == false) {
        m_font->setPointSize(fontSizeVariant.toInt());
        m_fontSizeComboBox->setCurrentText(fontSizeVariant.toString());
    }
    m_fontComboBox->setCurrentFont(*m_font);
}

void EditorSettingPage::apply()
{
    m_settings->setValue("editor/fontFamily", m_fontComboBox->currentFont().family());
    m_settings->setValue("editor/fontSize", m_fontSizeComboBox->currentText());

    qDebug() << "保存编辑器字体配置:";
    qDebug() << "fontFamliy: " << m_fontComboBox->currentFont().family();
    qDebug() << "fontPointSize: " << m_fontSizeComboBox->currentText();
}
