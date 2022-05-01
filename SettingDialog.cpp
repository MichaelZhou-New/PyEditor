#include "SettingDialog.h"

#include <QHBoxLayout>
#include <QApplication>
#include <QDialogButtonBox>
#include <QSizePolicy>

#include <QVBoxLayout>

#include <QPlainTextEdit>

#include "settings/KitsSettingPage.h"
#include "settings/AppearanceSettingPage.h"
#include "settings/EditorSettingPage.h"

SettingDialog::SettingDialog(QWidget *parent)
    : QDialog(parent),
      m_headerLabel(new QLabel),
      m_categoryList(new CategoryListView),
      m_settingList(new QListWidget),
      m_stackedWidget(new QStackedWidget(this))
{
    this->setupUi();
}

void SettingDialog::setupUi()
{
    m_headerLabel->setText("设置");
    this->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    this->setMinimumSize(470, 390);
    this->resize(750, 450);

    QFont headerLabelFont = m_headerLabel->font();
    headerLabelFont.setBold(true);

    const int pointSize = headerLabelFont.pointSize();
    if (pointSize > 0) {
        headerLabelFont.setPointSize(pointSize + 2);
    }
    m_headerLabel->setFont(headerLabelFont);

    QHBoxLayout *headerHLayout = new QHBoxLayout;
    const int leftMargin = QApplication::style()->pixelMetric(QStyle::PM_LayoutLeftMargin);
    headerHLayout->addSpacerItem(new QSpacerItem(leftMargin, 0, QSizePolicy::Fixed, QSizePolicy::Ignored));
    headerHLayout->addWidget(m_headerLabel);

    m_settingList->setMaximumWidth(195);
    m_settingList->setMinimumWidth(195);
    m_settingList->addItem(tr("解释器"));
    m_settingList->addItem(tr("外观"));
    m_settingList->addItem(tr("编辑器"));

    m_stackedWidget->addWidget(new KitsSettingPage);
    m_stackedWidget->addWidget(new AppearanceSettingPage);
    m_stackedWidget->addWidget(new EditorSettingPage);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                                    | QDialogButtonBox::Apply
                                                    | QDialogButtonBox::Cancel);
    QGridLayout *mainGridLayout = new QGridLayout;

    mainGridLayout->addLayout(headerHLayout, 0, 1, 1, 1);
    mainGridLayout->addWidget(m_settingList, 1, 0, 1, 1);
    mainGridLayout->addWidget(m_stackedWidget, 1, 1, 1, 1);
    mainGridLayout->addWidget(buttonBox, 2, 0, 1, 2);

    mainGridLayout->setColumnStretch(1, 4);
    mainGridLayout->setSizeConstraint(QLayout::SetMinimumSize);

    this->setLayout(mainGridLayout);

    // setting connection
    connect(buttonBox->button(QDialogButtonBox::Apply), &QAbstractButton::clicked,
            this, &SettingDialog::apply);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &SettingDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &SettingDialog::reject);

    connect(m_settingList, &QListWidget::currentRowChanged, this, &SettingDialog::onSettingListCurrentRowChanged);
}

void SettingDialog::onSettingListCurrentRowChanged(int row)
{
    m_headerLabel->setText(m_settingList->currentIndex().data().toString());
    m_stackedWidget->setCurrentIndex(row);
}

void SettingDialog::apply()
{
    for (int i = 0; i < m_stackedWidget->count(); i++) {
        SettingPage *settingPage = static_cast<SettingPage*>(m_stackedWidget->widget(i));
        settingPage->apply();
    }
}

void SettingDialog::accept()
{
    this->apply();
    done(QDialog::Accepted);
}

void SettingDialog::reject()
{
    done(QDialog::Rejected);
}
