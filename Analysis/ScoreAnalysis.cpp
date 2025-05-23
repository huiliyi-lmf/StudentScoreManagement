#include "ScoreAnalysis.h"
#include "ui_ScoreAnalysis.h"

ScoreAnalysis::ScoreAnalysis(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ScoreAnalysis), parent(parent)
{
    ui->setupUi(this);
}

ScoreAnalysis::~ScoreAnalysis()
{
    delete ui;
    parent->setEnabled(true);
}
