#include "KitsSettingPage.h"

#include <QDebug>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>

#include <QFileDialog>
#include <QString>
#include <QSettings>

KitsSettingPage::KitsSettingPage(QWidget *parent)
    : SettingPage(parent),
      m_settings(new QSettings(QSettings::IniFormat, QSettings::UserScope, "dylan", "PyEditor", this))
{
    this->setupUI();

    this->readSettings();

    // setting connection
    connect(m_pathButton, &QPushButton::pressed, this, &KitsSettingPage::onClickedPathButton);
    connect(this, &KitsSettingPage::interpreterPathChanged, m_interpreterPathLineEdit, &QLineEdit::setText);
}

void KitsSettingPage::readSettings()
{
    QString interpreterPath = m_settings->value("kits/interpreterPath").toString();
    qDebug() << "读取解释器路径：" << interpreterPath;
    if (interpreterPath.length() != 0) {
        m_interpreterPath = interpreterPath;
        m_interpreterPathLineEdit->setText(m_interpreterPath);
    }
}

void KitsSettingPage::setupUI()
{
    m_kitsGroupBox = new QGroupBox(tr("解释器配置"), this);
    m_pathLabel = new QLabel(tr("路径："), this) ;
    m_interpreterPathLineEdit = new QLineEdit;
    m_interpreterPathLineEdit->setReadOnly(true);
    m_pathButton = new QPushButton(tr("浏览..."));

    // tab1
    QWidget *tab1RootWidget = new QWidget(this);
    this->addTab(tab1RootWidget, tr("解释器"));
    QVBoxLayout *vBoxLayout = new QVBoxLayout;
    tab1RootWidget->setLayout(vBoxLayout);

    vBoxLayout->addWidget(m_kitsGroupBox);
    vBoxLayout->addSpacerItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding));

    QHBoxLayout *kitsGroupHBoxLayout = new QHBoxLayout(m_kitsGroupBox);
    kitsGroupHBoxLayout->addWidget(m_pathLabel);
    kitsGroupHBoxLayout->addWidget(m_interpreterPathLineEdit);
    kitsGroupHBoxLayout->addWidget(m_pathButton);
    kitsGroupHBoxLayout->addSpacerItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

    qDebug() << m_pathButton->height() << " " << m_pathButton->sizeHint();
}

void KitsSettingPage::apply()
{
    m_settings->setValue("kits/interpreterPath", m_interpreterPath);

    qDebug() << "保存成功";
    qDebug() << m_settings->fileName();
}

void KitsSettingPage::onClickedPathButton()
{
    QFileDialog fileDialog;
    fileDialog.setFileMode(QFileDialog::ExistingFile);

    QString path = QString(fileDialog.getOpenFileName());

    if (path.length() > 0) {
        m_interpreterPath = path;
        emit interpreterPathChanged(m_interpreterPath);
    }
}
