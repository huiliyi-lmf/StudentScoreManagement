#ifndef MAJORSETTINGWINDOW_H
#define MAJORSETTINGWINDOW_H

#include <QMainWindow>

namespace Ui {
class MajorSettingWindow;
}

class MajorSettingWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MajorSettingWindow(QWidget *parent = nullptr);
    ~MajorSettingWindow();

private:
    Ui::MajorSettingWindow *ui;
};

#endif // MAJORSETTINGWINDOW_H
