#include "slipad.h"
#include "ui_slipad.h"

#include <QString>
//#include <QDebug>
//#include <QMessageBox>

slipad::slipad(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::slipad)
{
    ui->setupUi(this);
    ui->wrong_txt->setHidden(true);
}

slipad::~slipad()
{
    delete ui;
}

void slipad::on_login_b_released()
{
    QString operator_id = ui->op_id->text();
    QString password = ui->password->text();

    ui->op_id->clear();
    ui->password->clear();

    //qDebug() << operator_id;
    menuWind = new menu(this);

    // SEARCH IN THE DATABASE
    // ------------------------------------------ DEBUG
    if( (operator_id == "D") && (password == "1") )
        // success login´
        menuWind->show();
    else
        //QMessageBox::warning(this,"Login", "Operator ID/ password incorrect");
        ui->wrong_txt->show();
}
