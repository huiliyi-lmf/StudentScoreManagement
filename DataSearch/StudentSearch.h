#ifndef STUDENTSEARCH_H
#define STUDENTSEARCH_H

#include <QMainWindow>

namespace Ui {
class StudentSearch;
}

class StudentSearch : public QMainWindow
{
    Q_OBJECT

public:
    explicit StudentSearch(QWidget *parent = nullptr);
    ~StudentSearch();

private:
    Ui::StudentSearch *ui;
    QWidget* parent;
};

#endif // STUDENTSEARCH_H
