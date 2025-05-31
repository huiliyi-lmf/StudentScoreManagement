#ifndef AIBASEWINDOW_H
#define AIBASEWINDOW_H

#include <QMainWindow>
#include <QPlainTextEdit>
#include <QProcessEnvironment>
#include <QTcpSocket>
#include <QElapsedTimer>
#include "MyMacro.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>
#include <QTimer>

class AIBaseWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit AIBaseWindow(QWidget *parent = nullptr, std::string prompt="");
    ~AIBaseWindow();
    virtual void sendData(QPlainTextEdit* edit)=0;
protected:
    const std::string AI_PROMPT;
    std::string serverUrl;
    std::string apiKey;
    std::string authToken;
    std::string chartId;
    QNetworkAccessManager *manager;
    bool isTokenVaild();
    bool isServerAvaliable();
    void checkAndSetServer();
private:
    QWidget* parent;
};

#endif // AIBASEWINDOW_H
