#include "AIBaseWindow.h"

AIBaseWindow::AIBaseWindow(QWidget *parent, std::string prompt)
    : QMainWindow{parent}, AI_PROMPT(prompt), serverUrl("https://vlsmb.top/"),
    manager(new QNetworkAccessManager), parent(parent)
{

    this->setAttribute(Qt::WA_DeleteOnClose);

    checkAndSetServer();
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    apiKey = env.value("AI_DASHSCOPE_API_KEY").toStdString();
    authToken = env.value("CPP_PROJECT_AUTH_TOKEN").toStdString();
    if(apiKey == "" && !isTokenVaild()) {
        QString value = QInputDialog::getText(this, "请设置API-KEY", "API-KEY");
        apiKey = value.toStdString();
    }
}

AIBaseWindow::~AIBaseWindow() {
    parent->setEnabled(true);
}

bool AIBaseWindow::isTokenVaild() {
    checkAndSetServer();
    if(authToken == "" || !isServerAvaliable()) {
        return false;
    }

}

bool AIBaseWindow::isServerAvaliable() {
    QNetworkRequest request(QUrl(STDTOQSTR(serverUrl)));
    request.setTransferTimeout(1000);

    QNetworkReply* reply = manager->get(request);
    QEventLoop eventLoop;

    // 连接信号：请求完成时退出事件循环
    QObject::connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec(QEventLoop::ExcludeUserInputEvents);

    bool result = false;
    if (reply->error() == QNetworkReply::NoError) {
        result = true;
        qDebug() << "response: " << reply->readAll();
    } else {
        qDebug() << "Error:" << reply->errorString();
    }

    reply->deleteLater();
    return result;
}

void AIBaseWindow::checkAndSetServer() {
    if(!isServerAvaliable()) {
        WARN_MESSAGE("AI模型不可用，或者网络不佳！");
        QString value = QInputDialog::getText(this, "请设置目标服务地址：", "地址：", QLineEdit::Normal, STDTOQSTR(serverUrl));
        if(value == "") {
            this->close();
            return;
        }
        serverUrl = value.toStdString();
        if(!isServerAvaliable()) {
            ERROR_MESSAGE("AI服务仍然不可用！");
            this->close();
            return;
        }
    }
}
