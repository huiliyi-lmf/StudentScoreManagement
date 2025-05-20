#include "MajorSettingWindow.h"
#include "ui_MajorSettingWindow.h"

MajorSettingWindow::MajorSettingWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MajorSettingWindow), parentPointer(parent)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
    QObject::connect(this->ui->btnAdd, &QPushButton::clicked, this, &MajorSettingWindow::onAddBtnClicked);
    QObject::connect(this->ui->btnDel, &QPushButton::clicked, this, &MajorSettingWindow::onDeleteBtnClicked);
    QObject::connect(this->ui->btnEdit, &QPushButton::clicked, this, &MajorSettingWindow::onUpdateBtnClicked);
}

MajorSettingWindow::~MajorSettingWindow()
{
    delete ui;
    parentPointer->setEnabled(true);
}

void MajorSettingWindow::onAddBtnClicked() {

}
void MajorSettingWindow::onUpdateBtnClicked() {

}
void MajorSettingWindow::onDeleteBtnClicked() {

}
