#include "MajorAnalysis.h"
#include "ui_MajorAnalysis.h"

MajorAnalysis::MajorAnalysis(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MajorAnalysis), parent(parent)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
}

MajorAnalysis::~MajorAnalysis()
{
    delete ui;
    parent->setEnabled(true);
}
