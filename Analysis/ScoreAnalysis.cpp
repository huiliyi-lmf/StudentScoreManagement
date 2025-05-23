#include "ScoreAnalysis.h"
#include "ui_ScoreAnalysis.h"

ScoreAnalysis::ScoreAnalysis(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ScoreAnalysis), parent(parent)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
}

ScoreAnalysis::~ScoreAnalysis()
{
    delete ui;
    parent->setEnabled(true);
}
