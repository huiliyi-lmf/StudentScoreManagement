#include "AiChatWindow.h"
#include "ui_AiChatWindow.h"

AiChatWindow::AiChatWindow(QWidget *parent)
    : AIBaseWindow(parent)
    , ui(new Ui::AiChatWindow)
{
    ui->setupUi(this);
    QBUTTON_CONNECT_MYCREATE(ui->sendBtn, &AiChatWindow::onUserBtnClicked);
}

AiChatWindow::~AiChatWindow()
{
    delete ui;
}

void AiChatWindow::sendData() {
    QString userInput = ui->userEdit->toPlainText();
    ui->showEdit->appendPlainText("用户输入：" + userInput + "\nAI回答：");
    std::vector<QWidget*>* widgets = new std::vector<QWidget*>();
    widgets->push_back(ui->userEdit);
    widgets->push_back(ui->sendBtn);
    this->sendAndFlush(ui->showEdit, QString("%1/chat").arg(QString::fromStdString(this->serverUrl)), userInput, widgets);
}

void AiChatWindow::onUserBtnClicked() {
    this->sendData();
}
