#include "AdminWindow.h"
#include "ui_AdminWindow.h"

AdminWindow::AdminWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AdminWindow), parent(parent)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);

    QObject::connect(this->ui->btnAdd, &QPushButton::clicked, this, &AdminWindow::addNewAdmin);
    QObject::connect(this->ui->btnEdit, &QPushButton::clicked, this, &AdminWindow::updateAdmin);
    QObject::connect(this->ui->btnDel, &QPushButton::clicked, this, &AdminWindow::deleteAdmin);
    QObject::connect(this->ui->tbUser, &QTableWidget::clicked, this, &AdminWindow::doubleClickAdminTable);
}

AdminWindow::~AdminWindow()
{
    delete ui;
    parent->setEnabled(true);
}

void AdminWindow::refreshTable() {

}

void AdminWindow::addNewAdmin() {

}

void AdminWindow::updateAdmin() {

}

void AdminWindow::deleteAdmin() {

}

void AdminWindow::doubleClickAdminTable() {

}
