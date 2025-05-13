#include "registerwindow.h"
#include "ui_registerwindow.h"

Registerwindow::Registerwindow(QDialog *parent)
    : QDialog(parent)
    , ui(new Ui::Registerwindow)
    , parentWindow(parent)
{
    ui->setupUi(this);
    QObject::connect(this->ui->registerButton, &QPushButton::clicked, this, &Registerwindow::onRegisterButtonClicked);
    QObject::connect(this->ui->exitButton, &QPushButton::clicked, this, &Registerwindow::onExitButtonClicked);
}

Registerwindow::~Registerwindow()
{
    delete ui;
}
void Registerwindow::onRegisterButtonClicked()
{
    QMessageBox::information(this, "账号注册成功", this->ui->userNameEdit->text());
}
void Registerwindow::onExitButtonClicked(){
    parentWindow->show();
    this->hide();
}
