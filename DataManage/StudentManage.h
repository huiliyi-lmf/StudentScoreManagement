#ifndef STUDENTMANAGE_H
#define STUDENTMANAGE_H

#include <QMainWindow>

namespace Ui {
class StudentManage;
}

class StudentManage : public QMainWindow
{
    Q_OBJECT

public:
    explicit StudentManage(QWidget *parent = nullptr);
    ~StudentManage();

private:
    Ui::StudentManage *ui;
    QWidget* parent;
};

#endif // STUDENTMANAGE_H
