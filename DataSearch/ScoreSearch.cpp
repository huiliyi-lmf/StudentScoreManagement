#include "ScoreSearch.h"
#include "ui_ScoreSearch.h"

ScoreSearch::ScoreSearch(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ScoreSearch), parent(parent)
{
    ui->setupUi(this);
}

ScoreSearch::~ScoreSearch()
{
    delete ui;
}
