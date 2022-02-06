#include "slipad.h"
#include "consult.h"

#include <QApplication>
#include "tcp/CTCPclient.h"
#include "tcp/CTCPcomm.h"
#include <string>
#include <QDebug>
#include <QMessageBox>
#include <errno.h>

using namespace std;

extern QString op_id;

CTCPclient tcp(string("192.168.1.114"), 5000);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    int ret = -1;
    int err;

    do
    {
        // connect to tcp server
        ret = tcp.connect();

        if(ret == 0)
        {
            qDebug() << "Connection established";
            break;
        }

        err = errno;
        if(err != ECONNREFUSED)
        {
            // unexpected connection error
            qDebug() << "[CGateway::connect] Connect failed: " << err;
        }
    }
    while(err == ECONNREFUSED);

    QString msg = "TYPE;2";
    // send type to remote system
    do
    {
        ret = tcp.send(msg.toStdString());
        err = errno;
    } while((ret == -1) && (err == EAGAIN));

    // handle possible comm errors
    if((ret == -1) && (err != EAGAIN))
    {
        //QMessageBox::warning(this, "Info", "Failed: Check your connection!");
        return -1;
    }

    slipad w;
    w.show();

    return a.exec();
}
