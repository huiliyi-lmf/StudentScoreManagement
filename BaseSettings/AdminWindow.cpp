#include "AdminWindow.h"
#include "ui_AdminWindow.h"

AdminWindow::AdminWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AdminWindow), parent(parent)
{
    ui->setupUi(this);
}

AdminWindow::~AdminWindow()
{
    delete ui;
    parent->setEnabled(true);
}
