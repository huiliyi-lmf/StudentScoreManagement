#ifndef REGISTERWINDOW_H
#define REGISTERWINDOW_H

#include <QWidget>

#include <QDialog>
#include <QMessageBox>
#include <QPushButton>
QT_BEGIN_NAMESPACE
namespace Ui {
class Registerwindow;
}

class Registerwindow : public QWidget
{
    Q_OBJECT

public:
    explicit Registerwindow(QWidget *parent = nullptr);
    ~Registerwindow();

private:
    Ui::Registerwindow *ui;
    void onRegisterButton_2Clicked();
    void onExitButtonClicked();
};

#endif // REGISTERWINDOW_H
