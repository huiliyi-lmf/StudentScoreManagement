#ifndef AICHATWINDOW_H
#define AICHATWINDOW_H

#include <QMainWindow>
#include "AIBaseWindow.h"

namespace Ui {
class AiChatWindow;
}

class AiChatWindow : public AIBaseWindow
{
    Q_OBJECT

public:
    explicit AiChatWindow(QWidget *parent = nullptr);
    ~AiChatWindow();
    void sendData(QPlainTextEdit* edit) override;
private:
    Ui::AiChatWindow *ui;
};

#endif // AICHATWINDOW_H
