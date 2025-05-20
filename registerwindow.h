#ifndef REGISTERWINDOW_H
#define REGISTERWINDOW_H

#include <QWidget>
#include <QDialog>
#include <QMessageBox>
#include <QPushButton>
#include "DataObjects/admin.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class Registerwindow;
}

class Registerwindow : public QDialog
{
    Q_OBJECT

public:
    explicit Registerwindow(QDialog *parent = nullptr);
    ~Registerwindow();

private:
    Ui::Registerwindow *ui;
    QDialog* parentWindow;
public slots:
    void onRegisterButtonClicked();
    void onExitButtonClicked();
};

#endif // REGISTERWINDOW_H
