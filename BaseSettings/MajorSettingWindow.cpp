#include "MajorSettingWindow.h"
#include "ui_MajorSettingWindow.h"

MajorSettingWindow::MajorSettingWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MajorSettingWindow)
{
    ui->setupUi(this);
}

MajorSettingWindow::~MajorSettingWindow()
{
    delete ui;
}
