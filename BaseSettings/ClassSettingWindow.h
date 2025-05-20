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
};

#endif // CLASSSETTINGWINDOW_H
