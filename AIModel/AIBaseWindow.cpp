#include "AIBaseWindow.h"

AIBaseWindow::AIBaseWindow(QWidget *parent, std::string prompt)
    : QMainWindow{parent}, AI_PROMPT(prompt), serverUrl("https://cpp.vlsmb.top"),
    chartId(QUuid::createUuid().toString(QUuid::Id128).toStdString()), manager(new QNetworkAccessManager)
    , parent(parent)
{

    this->setAttribute(Qt::WA_DeleteOnClose);

    checkAndSetServer();
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    apiKey = env.value("AI_DASHSCOPE_API_KEY").toStdString();
    authToken = env.value("CPP_PROJECT_AUTH_TOKEN").toStdString();
    if(!isTokenVaild() && apiKey == "") {
        // QString value = QInputDialog::getText(this, "请设置API-KEY", "API-KEY");
        // apiKey = value.toStdString();
        ERROR_MESSAGE("API-KEY已经无效！");
        this->close();
    }

    qDebug() << "chart UUID: " << chartId;
}

AIBaseWindow::~AIBaseWindow() {
    parent->setEnabled(true);
    delete manager;
}

bool AIBaseWindow::isTokenVaild() {
    checkAndSetServer();
    if(authToken == "" || !isServerAvaliable()) {
        return false;
    }
    QNetworkRequest request(QUrl(QString("%1/chat/check-token").arg(STDTOQSTR(serverUrl))));
    request.setTransferTimeout(3000);
    request.setRawHeader("Auth-Token", STDTOQSTR(authToken).toUtf8());

    QNetworkReply* reply = manager->get(request);
    QEventLoop eventLoop;
    QObject::connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec(QEventLoop::ExcludeUserInputEvents);
    QByteArray data = reply->readAll();
    qDebug() << "response: " << data;
    if (reply->error() == QNetworkReply::NoError) {
        QString responseString = QString::fromUtf8(data);
        if(responseString == "1") {
            qDebug() << "Token Valid";
            return true;
        }
        else {
            qDebug() << "Token Invalid";
            return false;
        }
    } else {
        qDebug() << "Error:" << reply->errorString();
        return false;
    }
}

bool AIBaseWindow::isServerAvaliable() {
    QNetworkRequest request(QUrl(STDTOQSTR(serverUrl)));
    request.setTransferTimeout(3000);

    QNetworkReply* reply = manager->get(request);
    QEventLoop eventLoop;

    // 连接信号：请求完成时退出事件循环
    QObject::connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec(QEventLoop::ExcludeUserInputEvents);

    bool result = false;
    qDebug() << "response: " << reply->readAll();
    if (reply->error() != QNetworkReply::TimeoutError) {
        result = true;
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

void AIBaseWindow::sendAndFlush(QPlainTextEdit* edit, QString baseUrl, QString query, std::vector<QWidget*>* disableWidget) {
    for(auto p : *disableWidget) {
        p->setEnabled(false);
    }

    QNetworkRequest request(QUrl(baseUrl + "?chartId=" + STDTOQSTR(chartId)));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "text/event-stream");
    request.setRawHeader("Auth-Token", STDTOQSTR(authToken).toUtf8());
    request.setRawHeader("Api-Key", STDTOQSTR(apiKey).toUtf8());

    QNetworkReply* reply = manager->post(request, query.toUtf8());
    QObject::connect(reply, &QNetworkReply::readyRead, this, [reply, edit]() {
        auto response = QString::fromUtf8(reply->readAll());
        response = response.replace("data:", "");
        response = response.trimmed();
        edit->setPlainText(edit->toPlainText() + response);
        edit->moveCursor(QTextCursor::End);
    });
    QObject::connect(reply, &QNetworkReply::errorOccurred, this, [edit, disableWidget, reply]() {
        edit->appendPlainText("错误: " + reply->errorString());
        qDebug() << "Error: " << reply->errorString();
        for(auto p : *disableWidget) {
            p->setEnabled(true);
        }
         reply->deleteLater();
    });
    QObject::connect(reply, &QNetworkReply::finished, this, [reply, disableWidget]() {
        reply->deleteLater();
        qDebug() << "Connection closed";
        for(auto p : *disableWidget) {
            p->setEnabled(true);
        }
        delete disableWidget;
    });
}
