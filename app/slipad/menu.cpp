#include "menu.h"
#include "ui_menu.h"

menu::menu(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::menu)
{
    ui->setupUi(this);

}

menu::~menu()
{
    delete ui;
}

void menu::on_add_new_released()
{
    addMenu = new addLamp(this);
    addMenu->show();
}

void menu::on_consult_released()
{

}


