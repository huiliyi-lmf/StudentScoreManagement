#ifndef ADMINWINDOW_H
#define ADMINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QInputDialog>
#include "DataObjects/admin.h"

namespace Ui {
class AdminWindow;
}

class AdminWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AdminWindow(QWidget *parent = nullptr);
    ~AdminWindow();

private:
    Ui::AdminWindow *ui;
    QWidget* parent;
    void refreshTable();
public slots:
    void addNewAdmin();
    void updateAdmin();
    void deleteAdmin();
    void doubleClickAdminTable();
};

#endif // ADMINWINDOW_H
