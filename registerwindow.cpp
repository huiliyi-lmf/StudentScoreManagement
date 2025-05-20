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

    std::string userInputName = this->ui->userNameEdit->text().toStdString();
    std::string userInputPwd = this->ui->passwordEdit->text().toStdString();

    Admin mapper;
    auto admins = mapper.selectAll();
    for(auto pb:admins) {
        Admin* p = (Admin*)pb;
        if(p->userName == userInputName) {
            QMessageBox::warning(this, "警告", "用户名已存在！");
            return;
        }
    }

    mapper.userName = userInputName;
    mapper.userPwd = userInputPwd;
    if(mapper.insert()) {
        QMessageBox::information(this, "信息", "账号注册成功");
    } else {
        QMessageBox::critical(this, "错误", "账号注册失败！");
    }
}

void Registerwindow::onExitButtonClicked(){
    parentWindow->show();
    this->hide();
}
