#include "StudentSearch.h"
#include "ui_StudentSearch.h"

StudentSearch::StudentSearch(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::StudentSearch), parent(parent)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
}

StudentSearch::~StudentSearch()
{
    delete ui;
    parent->setEnabled(true);
}
