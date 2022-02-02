#ifndef LAMPPOST_H
#define LAMPPOST_H

#include <QString>

class lamppost
{
public:
    int id;
    int status;
    QString street;
    QString post_code;
    QString parish;
    QString county;
    QString district;
};

class Node
{
public:
    lamppost data;
    Node *next;
};

#endif // LAMPPOST_H
