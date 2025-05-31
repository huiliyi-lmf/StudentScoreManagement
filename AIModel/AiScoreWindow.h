#ifndef AISCOREWINDOW_H
#define AISCOREWINDOW_H

#include <QMainWindow>
#include "AIBaseWindow.h"
#include "DataObjects/subject.h"
#include "DataObjects/major.h"
#include "DataObjects/score.h"

namespace Ui {
class AiScoreWindow;
}

class AiScoreWindow : public AIBaseWindow
{
    Q_OBJECT

public:
    explicit AiScoreWindow(QWidget *parent = nullptr);
    ~AiScoreWindow();
    void sendData() override;
private:
    Ui::AiScoreWindow *ui;
    void loadMajorToCombobox();
    void loadSubToCombobox(int majorId);
    bool isMajorChanging;
public slots:
    void onSubjectChanged();
    void onMajorChanged();
};

#endif // AISCOREWINDOW_H
