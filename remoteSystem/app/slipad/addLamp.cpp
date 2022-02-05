#include "addLamp.h"
#include "ui_addLamp.h"

#include "comms.h"

#include <QString>
#include <QDebug>
#include <QMessageBox>

extern QString op_id;

addLamp::addLamp(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::addLamp)
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

addLamp::~addLamp()
{
    delete ui;
}

void addLamp::on_addLamp_b_released()
{
    QString street = ui->street->text();
    QString postCode = ui->postCode->text();
    QString parish = ui->parish->text();
    QString county = ui->county->text();
    QString district = ui->district->text();
    QString address = ui->id->text();

    ui->street->clear();
    ui->postCode->clear();
    ui->parish->clear();
    ui->county->clear();
    ui->district->clear();
    ui->id->clear();

    if( !street.size() || !postCode.size() ||
        !parish.size() || !county.size() ||
        !district.size() || !address.size() )
    {
        QMessageBox::warning(this,"Add Lamppost", "Empty fields: All fields required!");
        return;
    }

     //qDebug() << "Position: " << source->lastKnownPosition();
    QGeoCoordinate coords = source->lastKnownPosition().coordinate();

    // store longitude and latitude coordinates
    double longitude = coords.longitude();
    double latitude = coords.latitude();

    //qDebug() << "longitude: " << QVariant {latitude}.toString() << " latitude: " << QVariant {longitude}.toString();

    if( (longitude != 0) && (latitude != 0) )
    {
        // stop updating coordinates
        source->stopUpdates();

        // ADD;<operator_id>;<addr>;<street_name>;<post_code>;<parish>;<county>;<district>;<latitude>;<longitude>
        QString msg = op_id + ";" + address + ";" + street + ";" + postCode + ";" + parish + ";" + county + ";"
                + district + ";" + QVariant {latitude}.toString() + ";" + QVariant {longitude}.toString();

        // add to database
        int err = execCmd("ADD;", msg);
        if(err == EXEC_OK)
            // success login
            this->deleteLater();

        else if(err == EXEC_FAIL)
        {
            QMessageBox::warning(this,"Add Lamppost", "Lamppost already defined in database!");
            return;
        }
        else
            QMessageBox::critical(this, "Info", "Failed: Check your connection!");
    }
}

void addLamp::on_back_b_released()
{
    this->deleteLater();
}
