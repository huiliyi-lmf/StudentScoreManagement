#include "SubjectSettingWindow.h"
#include "ui_SubjectSettingWindow.h"

SubjectSettingWindow::SubjectSettingWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SubjectSettingWindow)
{
    ui->setupUi(this);
}

SubjectSettingWindow::~SubjectSettingWindow()
{
    delete ui;
}
