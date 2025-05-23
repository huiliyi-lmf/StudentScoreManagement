#ifndef SCORESEARCH_H
#define SCORESEARCH_H

#include <QMainWindow>

namespace Ui {
class ScoreSearch;
}

class ScoreSearch : public QMainWindow
{
    Q_OBJECT

public:
    explicit ScoreSearch(QWidget *parent = nullptr);
    ~ScoreSearch();

private:
    Ui::ScoreSearch *ui;
    QWidget* parent;
};

#endif // SCORESEARCH_H
