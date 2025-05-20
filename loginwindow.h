#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H
#include<QMainWindow>
#include"registerwindow.h"
#include <QDialog>
#include <QMessageBox>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui {
class LoginWindow;
}
QT_END_NAMESPACE

class LoginWindow : public QDialog {
    Q_OBJECT

public:
    LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();

private:
    Ui::LoginWindow *ui;
    Registerwindow *registerwindow_ui=nullptr;
private slots:
    void onLoginButtonClicked();
    void onRegisterButtonClicked();

};
#endif // LOGINWINDOW_H
