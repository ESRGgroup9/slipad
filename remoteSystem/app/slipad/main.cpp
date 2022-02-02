#include "slipad.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    slipad w;
    //w.setGeometry(0,0,360*3,720*3);
    //w.resize(360*3,720*3);
    //w.setFixedSize(360*3,720*3);
    // w.showMaximized();
    w.show();
    //w.show();

    return a.exec();
}
