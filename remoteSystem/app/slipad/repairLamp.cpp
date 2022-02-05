#include "repairLamp.h"
#include "ui_repairLamp.h"

#include "comms.h"

#include <QString>
#include <QDebug>
#include <QMessageBox>

repairLamp::repairLamp(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::repairLamp)
{
    ui->setupUi(this);
}

repairLamp::~repairLamp()
{
    delete ui;
}

void repairLamp::on_selectLamp_b_released()
{
    QString address = ui->address->text();

    ui->address->clear();

    if( !address.size() )
    {
        QMessageBox::warning(this,"Repair Lamppost", "Provide an address!");
        return;
    }

    // SEARCH ID IN DATABASE
    //Usage: REP;<lamppost_address>
    QString msg = address;

    // add to database
    int err = execCmd("REP;", msg);
    if(err == EXEC_OK)
    {
        QMessageBox::warning(this,"Repair Lamppost", "Lamppost status updated!");
        // success login
        this->deleteLater();
    }
    else if(err == EXEC_FAIL)
    {
        QMessageBox::warning(this,"Repair Lamppost", "Address not found!");
        return;
    }
    else
        QMessageBox::critical(this, "Info", "Failed: Check your connection!");
    return;
}

void repairLamp::on_back_b_released()
{
    this->deleteLater();
}
