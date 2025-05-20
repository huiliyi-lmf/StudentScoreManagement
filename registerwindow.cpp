#include "registerwindow.h"
#include "ui_registerwindow.h"

Registerwindow::Registerwindow(QDialog *parent)
    : QDialog(parent)
    , ui(new Ui::Registerwindow)
    , parentWindow(parent)
{
    ui->setupUi(this);
    this->setLayout(ui->verticalLayout);
    this->setWindowTitle("欢迎来到注册界面");
    QObject::connect(this->ui->registerButton, &QPushButton::clicked, this, &Registerwindow::onRegisterButtonClicked);
    QObject::connect(this->ui->exitButton, &QPushButton::clicked, this, &Registerwindow::onExitButtonClicked);
}

Registerwindow::~Registerwindow()
{
    delete ui;
}
void Registerwindow::onRegisterButtonClicked()
{
    if (this->ui->userNameEdit->text().isEmpty() || this->ui->passwordEdit->text().isEmpty()) {
        QMessageBox::warning(this, "警告", "用户名或密码不能为空");
        return; 
    }
    QMessageBox::information(this, "账号注册成功", this->ui->userNameEdit->text());
}
void Registerwindow::onExitButtonClicked(){
    parentWindow->show();
    this->hide();
    
}
