#include "SubjectSettingWindow.h"
#include "ui_SubjectSettingWindow.h"

SubjectSettingWindow::SubjectSettingWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SubjectSettingWindow), parentPointer(parent)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
    QObject::connect(this->ui->btnAdd, &QPushButton::clicked, this, &SubjectSettingWindow::onAddBtnClicked);
    QObject::connect(this->ui->btnDel, &QPushButton::clicked, this, &SubjectSettingWindow::onDeleteBtnClicked);
    QObject::connect(this->ui->btnEdit, &QPushButton::clicked, this, &SubjectSettingWindow::onUpdateBtnClicked);
}

SubjectSettingWindow::~SubjectSettingWindow()
{
    delete ui;
    parentPointer->setEnabled(true);
}

void SubjectSettingWindow::onAddBtnClicked() {

}
void SubjectSettingWindow::onUpdateBtnClicked() {

}
void SubjectSettingWindow::onDeleteBtnClicked() {

}
