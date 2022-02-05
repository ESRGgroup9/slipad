#include "slipad.h"
#include "ui_slipad.h"

#include "comms.h"
#include "tcp/CTCPclient.h"

#include <QString>
#include <QMessageBox>

extern CTCPclient tcp;

QString op_id = "0";

slipad::slipad(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::slipad)
{
    ui->setupUi(this);
    ui->wrong_txt->setHidden(true);
    ui->no_con_txt->setHidden(true);

   /*int err = tcp.connect();

    if(err == -1)
    {
        QMessageBox::warning(this, "Remote Server Connection", "Not able to connect to remote server: Make sure you are connected to the same network.");
        return;
    }

    QString msg = "TYPE;2";
    int ret = 0;
    // send type to remote system
    do
    {
        ret = tcp.send(msg.toStdString());
        err = errno;
    } while((ret == -1) && (err == EAGAIN));

    // handle possible comm errors
    if((ret == -1) && (err != EAGAIN))
    {
        QMessageBox::warning(this, "Info", "Failed: Check your connection!");
        return;
    }*/

    //resize();
}

slipad::~slipad()
{
    delete ui;
}

void slipad::on_login_b_released()
{
    QString operator_id = ui->op_id->text();
    QString password = ui->password->text();

    op_id = operator_id;

    ui->op_id->clear();
    ui->password->clear();

    // next window
    menuWind = new menu(this);

    // SIGNIN;<operator_id>;<name>;<password>
    /*QString query = operator_id + ";" + password;
    // search in database
    int err = execCmd("SIGNIN;", query);

    if(err == EXEC_OK)
    {
        // success login
        menuWind->show();
        ui->wrong_txt->setHidden(true);
    }
    else if(err == EXEC_FAIL)
        //QMessageBox::warning(this,"Login", "Operator ID/ password incorrect");
        ui->wrong_txt->show();
    else
        QMessageBox::critical(this, "Info", "Failed: Check your connection!");*/


    // ------------------------------------------ DEBUG
    if( (operator_id == "D") && (password == "1") )
    {
        // success login
        menuWind->show();
        ui->wrong_txt->setHidden(true);
    }
    else
        //QMessageBox::warning(this,"Login", "Operator ID/ password incorrect");
        ui->wrong_txt->show();
}
