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

    // set up GPS
    source = QGeoPositionInfoSource::createDefaultSource(this);
    if ( source )
    {
        // conect to GPS
        connect(source, SIGNAL(positionUpdated(QGeoPositionInfo)),
                this, SLOT(positionUpdated(QGeoPositionInfo)));

        // start updating localization
        source->startUpdates();
    }
}

repairLamp::~repairLamp()
{
    delete ui;
}

void repairLamp::on_selectLamp_b_released()
{
    QString street = ui->street->text();
    QString postCode = ui->postCode->text();
    QString id = ui->id->text();

    ui->street->clear();
    ui->postCode->clear();
    ui->id->clear();

    if( !street.size() && !postCode.size() && !id.size()  )
    {
        QMessageBox::warning(this,"Add Lamppost", "Empty fields: All fields required!");
        return;
    }


}

void repairLamp::on_back_b_released()
{
    this->deleteLater();
}
