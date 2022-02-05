#ifndef LAMPPOST_H
#define LAMPPOST_H

#include <string>

struct lamppost
{
    int id;
    int address;
    int status;
    std::string street;
    /*QString post_code;
    QString parish;
    QString county;
    QString district;*/
};

class Node
{
public:
    lamppost data;
    Node *next;
};

#endif // LAMPPOST_H
