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
    QWidget* parentPointer;

public slots:
    void onAddBtnClicked();
    void onUpdateBtnClicked();
    void onDeleteBtnClicked();
};

#endif // MAJORSETTINGWINDOW_H
