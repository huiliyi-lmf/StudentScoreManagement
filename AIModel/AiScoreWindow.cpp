#include "AiScoreWindow.h"
#include "ui_AiScoreWindow.h"

AiScoreWindow::AiScoreWindow(QWidget *parent)
    : AIBaseWindow(parent)
    , ui(new Ui::AiScoreWindow)
{
    ui->setupUi(this);
}

AiScoreWindow::~AiScoreWindow()
{
    delete ui;
}

void AiScoreWindow::sendData(QPlainTextEdit* edit) {

}
