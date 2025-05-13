#include "registerwindow.h"
#include "ui_registerwindow.h"

Registerwindow::Registerwindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Registerwindow)
{
    ui->setupUi(this);
    QObject::connect(this->ui->registerButton_2, &QPushButton::clicked, this, &Registerwindow::onRegisterButton_2Clicked);
    QObject::connect(this->ui->exitButton, &QPushButton::clicked, this, &Registerwindow::onExitButtonClicked);
}

Registerwindow::~Registerwindow()
{

    delete ui;
}
void Registerwindow::onRegisterButton_2Clicked()
{
    QMessageBox::information(this, "账号注册成功", this->ui->userNameEdit->text());
}
void Registerwindow::onExitButtonClicked(){

}

