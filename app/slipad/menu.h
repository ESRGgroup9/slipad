#ifndef MENU_H
#define MENU_H

#include <QMainWindow>

#include "addLamp.h"

QT_BEGIN_NAMESPACE
namespace Ui { class menu; }
QT_END_NAMESPACE

class menu : public QMainWindow
{
    Q_OBJECT

public:
    menu(QWidget *parent = nullptr);
    ~menu();

private slots:
    void on_consult_released();

    void on_add_new_released();

private:
    Ui::menu *ui;

    addLamp *addMenu;
};
#endif // MENU_H
