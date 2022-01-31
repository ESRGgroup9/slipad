#ifndef REPAIRLAMP_H
#define REPAIRLAMP_H

#include <QMainWindow>
#include <QGeoPositionInfoSource>

QT_BEGIN_NAMESPACE
namespace Ui { class repairLamp; }
QT_END_NAMESPACE

class repairLamp : public QMainWindow
{
    Q_OBJECT

public:
    repairLamp(QWidget *parent = nullptr);
    ~repairLamp();

private slots:
    void on_selectLamp_b_released();

    void on_back_b_released();

private:
    Ui::repairLamp *ui;

    QGeoPositionInfoSource *source;
};
#endif // REPAIRLAMP_H
