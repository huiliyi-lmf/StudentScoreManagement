#include "ClassSettingWindow.h"
#include "ui_ClassSettingWindow.h"

ClassSettingWindow::ClassSettingWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ClassSettingWindow), parentPointer(parent)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
    QObject::connect(this->ui->btnAdd, &QPushButton::clicked, this, &ClassSettingWindow::onAddBtnClicked);
    QObject::connect(this->ui->btnDel, &QPushButton::clicked, this, &ClassSettingWindow::onDeleteBtnClicked);
    QObject::connect(this->ui->btnEdit, &QPushButton::clicked, this, &ClassSettingWindow::onUpdateBtnClicked);
}

ClassSettingWindow::~ClassSettingWindow()
{
    delete ui;
    parentPointer->setEnabled(true);
}

void ClassSettingWindow::onAddBtnClicked() {

}
void ClassSettingWindow::onUpdateBtnClicked() {

}
void ClassSettingWindow::onDeleteBtnClicked() {

}
