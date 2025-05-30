#ifndef MYMACRO_H
#define MYMACRO_H

#define DATABASE_NAME "student.db"

#define PROJECT_TITLE "学生成绩管理系统"

#define STDTOQSTR(str) (QString("%1").arg(str))

#define WARN_MESSAGE(str) QMessageBox::warning(this, QString(PROJECT_TITLE)+QString(" | 警告"), str)

#define SUCCESS_MESSAGE(str) QMessageBox::information(this, QString(PROJECT_TITLE)+QString(" | 提示"), str)

#define ERROR_MESSAGE(str) QMessageBox::critical(this, QString(PROJECT_TITLE)+QString(" | 错误"), str)

#define QUESTION_CONFIRMED_MESSAGE(str) (QMessageBox::question(this, QString(PROJECT_TITLE)+QString(" | 提示"), str)==QMessageBox::Yes)

#define QBUTTON_CONNECT_MYCREATE(btn, slot) \
QOBJECT_CONNECT_MYCREATE(btn, &QPushButton::clicked, slot)

#define QACTION_CONNECT_MYCREATE(act, slot) \
QOBJECT_CONNECT_MYCREATE(act, &QAction::triggered, slot)

#define QOBJECT_CONNECT_MYCREATE(widget, signal, slot) \
QObject::connect(widget, signal, this, slot)

#define GENERATE_CREATE_SUBWIN_SLOT_IMPL(slotName, subWinClazz) \
void slotName() {                                       \
    this->setEnabled(false);                            \
    subWinClazz* window = new subWinClazz(this);        \
    window->show();                                     \
}

#endif // MYMACRO_H
