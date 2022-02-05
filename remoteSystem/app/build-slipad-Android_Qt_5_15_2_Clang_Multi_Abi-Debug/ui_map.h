/********************************************************************************
** Form generated from reading UI file 'map.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAP_H
#define UI_MAP_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_map
{
public:
    QWidget *map_w;

    void setupUi(QWidget *map)
    {
        if (map->objectName().isEmpty())
            map->setObjectName(QString::fromUtf8("map"));
        map->resize(400, 300);
        map_w = new QWidget(map);
        map_w->setObjectName(QString::fromUtf8("map_w"));
        map_w->setGeometry(QRect(100, 40, 201, 171));

        retranslateUi(map);

        QMetaObject::connectSlotsByName(map);
    } // setupUi

    void retranslateUi(QWidget *map)
    {
        map->setWindowTitle(QCoreApplication::translate("map", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class map: public Ui_map {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAP_H
