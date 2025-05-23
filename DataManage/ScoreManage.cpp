#include "ScoreManage.h"
#include "ui_ScoreManage.h"

ScoreManage::ScoreManage(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ScoreManage), parent(parent)
{
    ui->setupUi(this);
}

ScoreManage::~ScoreManage()
{
    delete ui;
}
