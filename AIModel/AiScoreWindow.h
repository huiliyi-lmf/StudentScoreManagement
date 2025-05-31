#ifndef AISCOREWINDOW_H
#define AISCOREWINDOW_H

#include <QMainWindow>
#include "AIBaseWindow.h"

namespace Ui {
class AiScoreWindow;
}

class AiScoreWindow : public AIBaseWindow
{
    Q_OBJECT

public:
    explicit AiScoreWindow(QWidget *parent = nullptr);
    ~AiScoreWindow();
    void sendData(QPlainTextEdit* edit) override;
private:
    Ui::AiScoreWindow *ui;
};

#endif // AISCOREWINDOW_H
