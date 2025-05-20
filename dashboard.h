#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <QMainWindow>
#include <QMessageBox>
#include <QCloseEvent>

namespace Ui {
class DashBoard;
}

class DashBoard : public QMainWindow
{
    Q_OBJECT

public:
    explicit DashBoard(QWidget *parent = nullptr);
    ~DashBoard();
    void closeEvent(QCloseEvent *event) override;

private:
    Ui::DashBoard *ui;
    QWidget* parentPointer;
};

#endif // DASHBOARD_H
