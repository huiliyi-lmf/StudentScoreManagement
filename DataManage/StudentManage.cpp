#include "StudentManage.h"
#include "ui_StudentManage.h"

StudentManage::StudentManage(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::StudentManage), parent(parent)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
}

StudentManage::~StudentManage()
{
    delete ui;
    parent->setEnabled(true);
}
