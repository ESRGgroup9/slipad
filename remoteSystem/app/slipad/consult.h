#ifndef CONSULT_H
#define CONSULT_H

#include <QMainWindow>

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

private:
    Ui::consult *ui;
};
#endif // CONSULT_H
