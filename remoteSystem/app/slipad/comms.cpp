#include "comms.h"

#include <QString>
#include <QDebug>

#include <errno.h>
#include <string>
#include "tcp/CTCPclient.h"
#include "tcp/CTCPcomm.h"

extern CTCPclient tcp;

using namespace std;

int execCmd(QString cmd, QString args)
{
    string str = cmd.toStdString() + args.toStdString();

    if(send_cmd(str) == COMM_FAIL)
        return COMM_FAIL;

    if(receive_return(str) == COMM_FAIL)
        return COMM_FAIL;

    // return true (EXEC_OK = 1) if received "<cmd> OK"
    // return false (EXEC_FAIL = 0) if not
    return (int)(str.compare(cmd.toStdString()+string("OK")) == 0);
}

/*int execMultipleCmd(QVector<string> &received, QString cmd, QString args)
{
    string msg = cmd.toStdString() + args.toStdString();
    int num_msg = 0;

    if(send_cmd(msg) == COMM_FAIL)
        return COMM_FAIL;

    int err = receive_return(msg);
    if(err != COMM_FAIL)
    {
        size_t i = msg.find_last_of(";");
        string str = msg.substr(0,i);
        num_msg = atoi(str.c_str());
    }

    for( int i = 0; i <= num_msg; i++)
    {
        int err = receive_return(msg);
        if(err != COMM_FAIL)
        {
            received.push_back(msg);
        }
    }

    return 0;
}*/

int send_cmd(string str)
{
    int err = 0;
    int ret = 0;
    // send command to remote system
    do
    {
        ret = tcp.send(str);
        err = errno;
    } while((ret == -1) && (err == EAGAIN));

    // handle possible comm errors
    if((ret == -1) && (err != EAGAIN))
    {
        qDebug() << "[Send] Communication failed ..." << err;
        return COMM_FAIL;
    }
    return 0;
}


int receive_return(string &str)
{
    int err = 0;
    int ret = 0;
    // recv remote system response
    /*do
    {
        ret = tcp.recv(str);
        qDebug() << "[Recv] " << QString::fromStdString(str);
        err = errno;
    } while((ret == -1) && (err == EAGAIN));

    // handle possible comm errors
    if((ret == -1) && (err != EAGAIN))
    {
        qDebug() << "[Recv] Communication failed ... " << err;
        return COMM_FAIL;
    }*/

    do
    {
        ret = tcp.recv(str);

        if(ret == -1)
        {
            int err = errno;

            // non blocking operation?
            if(err != EAGAIN)
            {
                // unexpected error
                qDebug() << "[Comms::receive_return] " << err;
                return COMM_FAIL;
            }
        }
        else if(ret > 0)
        {
            return 0;
        }
    }
    while(ret != 0);

    qDebug() << "[Recv] Disconect ... " << err;

    return 0;
}
