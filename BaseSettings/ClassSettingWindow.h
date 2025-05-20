#ifndef CLASSSETTINGWINDOW_H
#define CLASSSETTINGWINDOW_H

#include <QMainWindow>

namespace Ui {
class ClassSettingWindow;
}

class ClassSettingWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ClassSettingWindow(QWidget *parent = nullptr);
    ~ClassSettingWindow();

private:
    Ui::ClassSettingWindow *ui;
    QWidget* parentPointer;

public slots:
    void onAddBtnClicked();
    void onUpdateBtnClicked();
    void onDeleteBtnClicked();
};

#endif // CLASSSETTINGWINDOW_H
