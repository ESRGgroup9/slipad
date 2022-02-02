#include "repairLamp.h"
#include "ui_repairLamp.h"

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
    QString id = ui->id->text();

    ui->id->clear();

    if( !id.size() )
    {
        QMessageBox::warning(this,"Repair Lamppost", "Provide an ID!");
        return;
    }

    // SEARCH ID IN DATABASE
    int id_db;
    if(id_db != id.toInt())
        QMessageBox::warning(this,"Repair Lamppost", "ID not found!");
    else
    {
        // UPDATE STATUS IN DATABASE
        QMessageBox::warning(this,"Repair Lamppost", "Lamppost status updated!");
    }

    return;
}

void repairLamp::on_back_b_released()
{
    this->deleteLater();
}
