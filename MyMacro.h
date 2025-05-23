#ifndef MYMACRO_H
#define MYMACRO_H

#define DATABASE_NAME "student.db"

#define STDTOQSTR(str) QString(std::string(str).c_str())

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
