#ifndef MYSETTINGDIALOG_H
#define MYSETTINGDIALOG_H

#include <QDialog>

namespace Ui {
class MySettingDialog;
}

class MySettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MySettingDialog(QWidget *parent = nullptr);
    ~MySettingDialog();

private:
    Ui::MySettingDialog *ui;
};

#endif // MYSETTINGDIALOG_H
