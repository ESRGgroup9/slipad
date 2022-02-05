#ifndef CONSULT_H
#define CONSULT_H

#include <QMainWindow>
#include <QTableWidgetItem>

#include "lamppost.h"

QT_BEGIN_NAMESPACE
namespace Ui { class consult; }
QT_END_NAMESPACE

class consult : public QMainWindow
{
    Q_OBJECT

public:
    consult(QWidget *parent = nullptr);
    ~consult();

    void printList(Node *n);

private slots:
    void on_lampList_itemClicked(QTableWidgetItem *item);

    void on_back_b_released();

private:
    Ui::consult *ui;

    enum collumns
    {
        ID, STREET, ADDRESS, STATUS
    };
};
#endif // CONSULT_H
