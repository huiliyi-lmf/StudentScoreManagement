#include "MajorAnalysis.h"
#include "ui_MajorAnalysis.h"

MajorAnalysis::MajorAnalysis(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MajorAnalysis), parent(parent)
{
    ui->setupUi(this);
}

MajorAnalysis::~MajorAnalysis()
{
    delete ui;
    parent->setEnabled(true);
}
