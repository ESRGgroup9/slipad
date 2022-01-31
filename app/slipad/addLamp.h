#ifndef ADDLAMP_H
#define ADDLAMP_H

#include <QMainWindow>
#include <QGeoPositionInfoSource>

QT_BEGIN_NAMESPACE
namespace Ui { class addLamp; }
QT_END_NAMESPACE

class addLamp : public QMainWindow
{
    Q_OBJECT

public:
    addLamp(QWidget *parent = nullptr);
    ~addLamp();

private slots:
    void on_addLamp_b_released();

private:
    Ui::addLamp *ui;

    QGeoPositionInfoSource *source;
};
#endif // ADDLAMP_H
