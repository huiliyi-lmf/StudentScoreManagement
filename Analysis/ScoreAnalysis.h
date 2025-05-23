#ifndef SCOREANALYSIS_H
#define SCOREANALYSIS_H

#include <QMainWindow>

namespace Ui {
class ScoreAnalysis;
}

class ScoreAnalysis : public QMainWindow
{
    Q_OBJECT

public:
    explicit ScoreAnalysis(QWidget *parent = nullptr);
    ~ScoreAnalysis();

private:
    Ui::ScoreAnalysis *ui;
    QWidget* parent;
};

#endif // SCOREANALYSIS_H
