#ifndef COMMS_H
#define COMMS_H

#define EXEC_FAIL       0
#define EXEC_OK         1
#define COMM_FAIL       2

#include <QString>
#include <string>

int execCmd(QString cmd, QString args);
//int execMultipleCmd(QVector<std::string> &received, QString cmd, QString args);

int receive_return(std::string &str);
int send_cmd(std::string str);

#endif // COMMS_H
