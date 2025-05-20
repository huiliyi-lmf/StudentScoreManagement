#include "loginwindow.h"
#include "ui_loginwindow.h"

LoginWindow::LoginWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginWindow) {
    ui->setupUi(this);
    this->setLayout(ui->verticalLayout);
    QObject::connect(this->ui->loginButton, &QPushButton::clicked, this, &LoginWindow::onLoginButtonClicked);
    QObject::connect(this->ui->registerButton, &QPushButton::clicked, this, &LoginWindow::onRegisterButtonClicked);
}

LoginWindow::~LoginWindow() {
    delete registerwindow_ui;
    delete ui;
    DataObject::closeDataBase();
}

void LoginWindow::onLoginButtonClicked() {
    std::string userInputName = this->ui->userNameEdit->text().toStdString();
    std::string userInputPwd = this->ui->passwordEdit->text().toStdString();
    Admin admin;
    auto allAdmins = admin.selectAll();
    for(auto pb:allAdmins) {
        Admin* p = (Admin*)pb;
        if(p->userName == userInputName) {
            if(p->userPwd != userInputPwd) {
                QMessageBox::critical(this, "错误", "密码错误！");
            } else {
                QMessageBox::information(this, "信息", "登陆成功！");
                DashBoard* dashboard = new DashBoard(this);
                this->hide();
                dashboard->show();
            }
            return;
        }
    }
    QMessageBox::critical(this, "错误", "用户不存在！");
}

void LoginWindow::onRegisterButtonClicked() {

    if(!registerwindow_ui)
    {
        registerwindow_ui= new Registerwindow(this);
    }
    this->hide();
    registerwindow_ui->show();
}
