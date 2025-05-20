#include "dashboard.h"
#include "ui_dashboard.h"

DashBoard::DashBoard(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::DashBoard), parentPointer(parent)
{
    ui->setupUi(this);
}

DashBoard::~DashBoard()
{
    delete ui;
    parentPointer->show();
}

void DashBoard::closeEvent(QCloseEvent *event) {
    if(QMessageBox::question(this, "提示", "确定要退出吗？") == QMessageBox::Yes) {
        event->accept();
        this->deleteLater();
    } else {
        event->ignore();
    }
}
