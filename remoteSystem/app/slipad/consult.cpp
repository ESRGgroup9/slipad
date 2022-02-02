#include "consult.h"
#include "ui_consult.h"

#include <QtWebView>
#include <QString>
#include <QDebug>
#include <QMessageBox>

consult::consult(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::consult)
{
    ui->setupUi(this);
    Node *head = new Node();

    lamppost l1 = { 1, 0, "travessa dos pomares", "4755-308", "martim", "Barcelos", "braga" };
    lamppost l2 = { 2, 1, "pomares", "5000-308", "pousa", "Barcelos", "braga" };
    lamppost l3 = { 3, 2, "travessa", "10-308", "cabreiros", "braga", "braga" };

    Node *node0 = new Node();
    Node *node1 = new Node();

    head->data = l1;
    head->next = node0;

    node0->data = l2;
    node0->next = node1;

    node1->data = l3;
    node1->next = nullptr;

}

consult::~consult()
{
    delete ui;
}

void consult::printList(Node *lamp)
{
    while(lamp != nullptr)
    {

        lamp = lamp->next;
    }
}
