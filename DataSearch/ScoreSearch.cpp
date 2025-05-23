#include "ScoreSearch.h"
#include "ui_ScoreSearch.h"

ScoreSearch::ScoreSearch(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ScoreSearch), parent(parent)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
}

ScoreSearch::~ScoreSearch()
{
    delete ui;
    parent->setEnabled(true);
}
