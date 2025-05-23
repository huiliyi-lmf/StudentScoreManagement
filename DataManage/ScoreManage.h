#ifndef SCOREMANAGE_H
#define SCOREMANAGE_H

#include <QMainWindow>

namespace Ui {
class ScoreManage;
}

class ScoreManage : public QMainWindow
{
    Q_OBJECT

public:
    explicit ScoreManage(QWidget *parent = nullptr);
    ~ScoreManage();

private:
    Ui::ScoreManage *ui;
    QWidget* parent;
};

#endif // SCOREMANAGE_H
