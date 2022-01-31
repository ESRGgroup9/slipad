#ifndef MENU_H
#define MENU_H

#include <QMainWindow>

#include "addLamp.h"
#include "repairLamp.h"

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

    void on_logout_b_released();

    void on_mod_lamp_released();

private:
    Ui::menu *ui;

    addLamp *addMenu;
    repairLamp *repairMenu;
};
#endif // MENU_H
