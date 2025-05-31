#include "AiChatWindow.h"
#include "ui_AiChatWindow.h"

AiChatWindow::AiChatWindow(QWidget *parent)
    : AIBaseWindow(parent)
    , ui(new Ui::AiChatWindow)
{
    ui->setupUi(this);
}

AiChatWindow::~AiChatWindow()
{
    delete ui;
}

void AiChatWindow::sendData(QPlainTextEdit* edit) {

}
