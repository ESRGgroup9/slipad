#ifndef SLIPAD_H
#define SLIPAD_H

#include <QMainWindow>

#include "menu.h"

QT_BEGIN_NAMESPACE
namespace Ui { class slipad; }
QT_END_NAMESPACE

class slipad : public QMainWindow
{
    Q_OBJECT

public:
    slipad(QWidget *parent = nullptr);
    ~slipad();

private slots:
    void on_login_b_released();

private:
    Ui::slipad *ui;

    menu menuWind;
};
#endif // SLIPAD_H
