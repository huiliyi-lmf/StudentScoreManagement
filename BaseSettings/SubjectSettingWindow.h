#ifndef SUBJECTSETTINGWINDOW_H
#define SUBJECTSETTINGWINDOW_H

#include <QMainWindow>

namespace Ui {
class SubjectSettingWindow;
}

class SubjectSettingWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SubjectSettingWindow(QWidget *parent = nullptr);
    ~SubjectSettingWindow();

private:
    Ui::SubjectSettingWindow *ui;
    QWidget* parentPointer;

public slots:
    void onAddBtnClicked();
    void onUpdateBtnClicked();
    void onDeleteBtnClicked();
};

#endif // SUBJECTSETTINGWINDOW_H
