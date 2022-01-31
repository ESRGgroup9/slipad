#include "addLamp.h"
#include "ui_addLamp.h"

#include <QString>
#include <QDebug>

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

    ui->street->clear();
    ui->postCode->clear();
    ui->parish->clear();
    ui->county->clear();
    ui->district->clear();

     //qDebug() << "Position: " << source->lastKnownPosition();
    QGeoCoordinate coords = source->lastKnownPosition().coordinate();

    // store longitude and latitude coordinates
    int longitude = coords.longitude();
    int latitude = coords.latitude();

    if( (longitude != 0) && (latitude != 0) )
    {
        // stop updating coordinates
        source->stopUpdates();

        // ADD LAMPPOST TO DATABASE
    }
}
