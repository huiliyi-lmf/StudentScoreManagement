#include "loginwindow.h"
#include "ui_loginwindow.h"

LoginWindow::LoginWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginWindow) {
    ui->setupUi(this);
    QObject::connect(this->ui->loginButton, &QPushButton::clicked, this, &LoginWindow::onLoginButtonClicked);
    QObject::connect(this->ui->registerButton, &QPushButton::clicked, this, &LoginWindow::onRegisterButtonClicked);
}

LoginWindow::~LoginWindow() {
    delete ui;
}

void LoginWindow::onLoginButtonClicked() {
    QMessageBox::information(this, "Hello", this->ui->userNameEdit->text());
}

void LoginWindow::onRegisterButtonClicked() {
    QMessageBox::information(this, "Hello", this->ui->passwordEdit->text());
}
