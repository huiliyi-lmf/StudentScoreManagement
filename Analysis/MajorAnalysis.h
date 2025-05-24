#ifndef MAJORANALYSIS_H
#define MAJORANALYSIS_H

#include <QMainWindow>

namespace Ui {
class MajorAnalysis;
}

class MajorAnalysis : public QMainWindow
{
    Q_OBJECT

public:
    explicit MajorAnalysis(QWidget *parent = nullptr);
    ~MajorAnalysis();

private:
    Ui::MajorAnalysis *ui;
    QWidget* parent;
};

#endif // MAJORANALYSIS_H
