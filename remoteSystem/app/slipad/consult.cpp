#include "consult.h"
#include "ui_consult.h"

#include <QtWebView>
#include <QString>
#include <QDebug>
#include <QMessageBox>
#include <string>
#include <cstdlib>    /* atoi */

#include "slipad.h"
#include "comms.h"

extern QString op_id;

using namespace std;

consult::consult(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::consult)
{
    ui->setupUi(this);

    ui->lampList->setColumnCount(4);
    QStringList titulos;
    titulos << "ID" << "Street" << "Addr" << "Status";
    ui->lampList->setHorizontalHeaderLabels(titulos);
    ui->lampList->setColumnWidth(ID, 20);
    ui->lampList->setColumnWidth(STREET, 200);
    ui->lampList->setColumnWidth(STATUS, 45);
    ui->lampList->setColumnWidth(ADDRESS, 40);
    ui->lampList->verticalHeader()->setVisible(false);
    //ui->lampList->setVisible(false);
    //ui->lampList->resizeColumnsToContents();
    //ui->lampList->setVisible(true);

    string msg = "CONSULT;" + op_id.toStdString();

    //send cmd
    if(send_cmd(msg) == COMM_FAIL)
    {
        QMessageBox::warning(this,"Consult Lamppost", "Not able to communicate with database!");
        return;
    }

    int err = receive_return(msg);
    //qDebug() << "POSTS NUM: " << QString::fromStdString(msg);
    int num_msg = 0;
    if(err == COMM_FAIL)
     {
        QMessageBox::warning(this,"Consult Lamppost", "Not able to communicate with database!");
        return;
    }

    size_t p = msg.find_last_of(";");
    //qDebug() << "p: " << QString::number(p);
    string str = msg.substr(p+1, msg.length());

    qDebug() << "[consult()] Lamppost num" << QString::fromStdString(msg);
    num_msg = atoi(str.c_str());

    string delimiter = ";";

    //QVector<lamppost> lamppostList;
    for( int i = 0; i < num_msg; i++)
    {
        qDebug() << "i: " << i;

        string token;
        int pos;

        int err = receive_return(msg);
        qDebug() << "[consult()] received!" << QString::fromStdString(msg);
        if(err == COMM_FAIL)
        {
            QMessageBox::warning(this,"Consult Lamppost", "Not able to communicate with database!");
            return;
        }

        string lamppost[4];
        // parse received string
        for(int j = 0; j < 4; j++)
        {
            //qDebug() << "j: " << j;

            // get token position
            pos = msg.find(delimiter);
            // get substring
            token = msg.substr(0, pos);
            // erase substring in received string
            msg.erase(0, pos + delimiter.length());

            lamppost[j] = token;
            qDebug() << "[consult()] parse!" << QString::fromStdString(token);
        }
        //struct lamppost lp;
        string street = lamppost[0];
        string id = lamppost[1];
        string address = lamppost[2];
        string status = lamppost[3];

        qDebug() << "street: " << QString::fromStdString(street);
        qDebug() << "id: " << QString::fromStdString(id);
        qDebug() << "address: " << QString::fromStdString(address);
        qDebug() << "status: " << QString::fromStdString(status);

        ui->lampList->insertRow(ui->lampList->rowCount());
        int row = ui->lampList->rowCount() - 1;
        ui->lampList->setItem(row, ID, new QTableWidgetItem(QString::fromStdString(id)));
        ui->lampList->setItem(row, STREET, new QTableWidgetItem(QString::fromStdString(street)));
        ui->lampList->setItem(row, ADDRESS, new QTableWidgetItem(QString::fromStdString(address)));
        ui->lampList->setItem(row, STATUS, new QTableWidgetItem(QString::fromStdString(status)));

        string msg = "CONSULT;OK";

        //send ok
        if(send_cmd(msg) == COMM_FAIL)
        {
            QMessageBox::warning(this,"Consult Lamppost", "Not able to communicate with database!");
            return;
        }
    }
}

consult::~consult()
{
    delete ui;
}

void consult::on_lampList_itemClicked(QTableWidgetItem *item)
{
    ui->street_amp->setText(item->text());
}


void consult::on_back_b_released()
{
    this->deleteLater();
}

