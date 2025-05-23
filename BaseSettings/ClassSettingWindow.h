#ifndef CLASSSETTINGWINDOW_H
#define CLASSSETTINGWINDOW_H
#include"DataObjects/class.h"
#include"DataObjects/major.h"
#include <QMainWindow>
#include <QMessageBox>
#include"DataObjects/class.h"
namespace Ui {
class ClassSettingWindow;
}

class ClassSettingWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ClassSettingWindow(QWidget *parent = nullptr);
    ~ClassSettingWindow();
     void loadMajorsToComboBox();
    void loadClassToTable();
private:
    Ui::ClassSettingWindow *ui;
    QWidget* parentPointer;


public slots:
    void onAddBtnClicked();
    void onUpdateBtnClicked();
    void onDeleteBtnClicked();
    void onExitBtnClicked();
    void onTableItemDoubleClicked(int row, int column);
};

#endif // CLASSSETTINGWINDOW_H
