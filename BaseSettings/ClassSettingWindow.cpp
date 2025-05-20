#include "ClassSettingWindow.h"
#include "ui_ClassSettingWindow.h"

ClassSettingWindow::ClassSettingWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ClassSettingWindow)
{
    ui->setupUi(this);
}

ClassSettingWindow::~ClassSettingWindow()
{
    delete ui;
}
