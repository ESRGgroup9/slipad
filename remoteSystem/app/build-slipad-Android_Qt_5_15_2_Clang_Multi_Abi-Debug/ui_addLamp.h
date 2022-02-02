/********************************************************************************
** Form generated from reading UI file 'addLamp.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADDLAMP_H
#define UI_ADDLAMP_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_addLamp
{
public:
    QWidget *centralwidget;
    QTextBrowser *SLIPAD;
    QTextBrowser *smart_lighting;
    QTextBrowser *street_txt;
    QTextBrowser *postCode_txt;
    QLineEdit *postCode;
    QLineEdit *street;
    QPushButton *addLamp_b;
    QPushButton *back_b;
    QLineEdit *parish;
    QTextBrowser *parish_txt;
    QTextBrowser *count_txt;
    QLineEdit *county;
    QLineEdit *district;
    QTextBrowser *district_txt;
    QTextBrowser *add_lamp_txt;

    void setupUi(QMainWindow *addLamp)
    {
        if (addLamp->objectName().isEmpty())
            addLamp->setObjectName(QString::fromUtf8("addLamp"));
        addLamp->resize(360, 720);
        QPalette palette;
        QBrush brush(QColor(0, 0, 0, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush1(QColor(239, 239, 239, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Button, brush1);
        QBrush brush2(QColor(255, 255, 255, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Light, brush2);
        QBrush brush3(QColor(202, 202, 202, 255));
        brush3.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Midlight, brush3);
        QBrush brush4(QColor(159, 159, 159, 255));
        brush4.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Dark, brush4);
        QBrush brush5(QColor(184, 184, 184, 255));
        brush5.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Mid, brush5);
        palette.setBrush(QPalette::Active, QPalette::Text, brush);
        palette.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette.setBrush(QPalette::Active, QPalette::Window, brush2);
        QBrush brush6(QColor(118, 118, 118, 255));
        brush6.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Shadow, brush6);
        QBrush brush7(QColor(48, 140, 198, 255));
        brush7.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Highlight, brush7);
        palette.setBrush(QPalette::Active, QPalette::HighlightedText, brush2);
        QBrush brush8(QColor(0, 0, 255, 255));
        brush8.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Link, brush8);
        QBrush brush9(QColor(255, 0, 255, 255));
        brush9.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::LinkVisited, brush9);
        QBrush brush10(QColor(247, 247, 247, 255));
        brush10.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::AlternateBase, brush10);
        QBrush brush11(QColor(0, 0, 0, 255));
        brush11.setStyle(Qt::NoBrush);
        palette.setBrush(QPalette::Active, QPalette::NoRole, brush11);
        QBrush brush12(QColor(255, 255, 220, 255));
        brush12.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::ToolTipBase, brush12);
        palette.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        QBrush brush13(QColor(0, 0, 0, 128));
        brush13.setStyle(Qt::SolidPattern);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette.setBrush(QPalette::Active, QPalette::PlaceholderText, brush13);
#endif
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::Midlight, brush3);
        palette.setBrush(QPalette::Inactive, QPalette::Dark, brush4);
        palette.setBrush(QPalette::Inactive, QPalette::Mid, brush5);
        palette.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Shadow, brush6);
        palette.setBrush(QPalette::Inactive, QPalette::Highlight, brush7);
        palette.setBrush(QPalette::Inactive, QPalette::HighlightedText, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::Link, brush8);
        palette.setBrush(QPalette::Inactive, QPalette::LinkVisited, brush9);
        palette.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush10);
        QBrush brush14(QColor(0, 0, 0, 255));
        brush14.setStyle(Qt::NoBrush);
        palette.setBrush(QPalette::Inactive, QPalette::NoRole, brush14);
        palette.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush12);
        palette.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette.setBrush(QPalette::Inactive, QPalette::PlaceholderText, brush13);
#endif
        QBrush brush15(QColor(190, 190, 190, 255));
        brush15.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush15);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::Midlight, brush3);
        palette.setBrush(QPalette::Disabled, QPalette::Dark, brush15);
        palette.setBrush(QPalette::Disabled, QPalette::Mid, brush5);
        palette.setBrush(QPalette::Disabled, QPalette::Text, brush15);
        palette.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::ButtonText, brush15);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush2);
        QBrush brush16(QColor(177, 177, 177, 255));
        brush16.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Disabled, QPalette::Shadow, brush16);
        QBrush brush17(QColor(145, 145, 145, 255));
        brush17.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Disabled, QPalette::Highlight, brush17);
        palette.setBrush(QPalette::Disabled, QPalette::HighlightedText, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::Link, brush8);
        palette.setBrush(QPalette::Disabled, QPalette::LinkVisited, brush9);
        palette.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush10);
        QBrush brush18(QColor(0, 0, 0, 255));
        brush18.setStyle(Qt::NoBrush);
        palette.setBrush(QPalette::Disabled, QPalette::NoRole, brush18);
        palette.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush12);
        palette.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette.setBrush(QPalette::Disabled, QPalette::PlaceholderText, brush13);
#endif
        addLamp->setPalette(palette);
        centralwidget = new QWidget(addLamp);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        SLIPAD = new QTextBrowser(centralwidget);
        SLIPAD->setObjectName(QString::fromUtf8("SLIPAD"));
        SLIPAD->setGeometry(QRect(0, 50, 360, 101));
        SLIPAD->setFrameShape(QFrame::NoFrame);
        SLIPAD->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        SLIPAD->setTextInteractionFlags(Qt::LinksAccessibleByKeyboard|Qt::LinksAccessibleByMouse);
        smart_lighting = new QTextBrowser(centralwidget);
        smart_lighting->setObjectName(QString::fromUtf8("smart_lighting"));
        smart_lighting->setGeometry(QRect(0, 140, 360, 31));
        QPalette palette1;
        palette1.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette1.setBrush(QPalette::Active, QPalette::Button, brush1);
        palette1.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette1.setBrush(QPalette::Active, QPalette::Midlight, brush3);
        palette1.setBrush(QPalette::Active, QPalette::Dark, brush4);
        palette1.setBrush(QPalette::Active, QPalette::Mid, brush5);
        palette1.setBrush(QPalette::Active, QPalette::Text, brush);
        palette1.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette1.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette1.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette1.setBrush(QPalette::Active, QPalette::Window, brush2);
        palette1.setBrush(QPalette::Active, QPalette::Shadow, brush6);
        palette1.setBrush(QPalette::Active, QPalette::Highlight, brush7);
        palette1.setBrush(QPalette::Active, QPalette::HighlightedText, brush2);
        palette1.setBrush(QPalette::Active, QPalette::Link, brush8);
        palette1.setBrush(QPalette::Active, QPalette::LinkVisited, brush9);
        palette1.setBrush(QPalette::Active, QPalette::AlternateBase, brush10);
        QBrush brush19(QColor(0, 0, 0, 255));
        brush19.setStyle(Qt::NoBrush);
        palette1.setBrush(QPalette::Active, QPalette::NoRole, brush19);
        palette1.setBrush(QPalette::Active, QPalette::ToolTipBase, brush12);
        palette1.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette1.setBrush(QPalette::Active, QPalette::PlaceholderText, brush13);
#endif
        palette1.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette1.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::Midlight, brush3);
        palette1.setBrush(QPalette::Inactive, QPalette::Dark, brush4);
        palette1.setBrush(QPalette::Inactive, QPalette::Mid, brush5);
        palette1.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette1.setBrush(QPalette::Inactive, QPalette::Shadow, brush6);
        palette1.setBrush(QPalette::Inactive, QPalette::Highlight, brush7);
        palette1.setBrush(QPalette::Inactive, QPalette::HighlightedText, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::Link, brush8);
        palette1.setBrush(QPalette::Inactive, QPalette::LinkVisited, brush9);
        palette1.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush10);
        QBrush brush20(QColor(0, 0, 0, 255));
        brush20.setStyle(Qt::NoBrush);
        palette1.setBrush(QPalette::Inactive, QPalette::NoRole, brush20);
        palette1.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush12);
        palette1.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette1.setBrush(QPalette::Inactive, QPalette::PlaceholderText, brush13);
#endif
        palette1.setBrush(QPalette::Disabled, QPalette::WindowText, brush15);
        palette1.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette1.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::Midlight, brush3);
        palette1.setBrush(QPalette::Disabled, QPalette::Dark, brush15);
        palette1.setBrush(QPalette::Disabled, QPalette::Mid, brush5);
        palette1.setBrush(QPalette::Disabled, QPalette::Text, brush15);
        palette1.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::ButtonText, brush15);
        palette1.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette1.setBrush(QPalette::Disabled, QPalette::Window, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::Shadow, brush16);
        palette1.setBrush(QPalette::Disabled, QPalette::Highlight, brush17);
        palette1.setBrush(QPalette::Disabled, QPalette::HighlightedText, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::Link, brush8);
        palette1.setBrush(QPalette::Disabled, QPalette::LinkVisited, brush9);
        palette1.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush10);
        QBrush brush21(QColor(0, 0, 0, 255));
        brush21.setStyle(Qt::NoBrush);
        palette1.setBrush(QPalette::Disabled, QPalette::NoRole, brush21);
        palette1.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush12);
        palette1.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette1.setBrush(QPalette::Disabled, QPalette::PlaceholderText, brush13);
#endif
        smart_lighting->setPalette(palette1);
        smart_lighting->setFrameShape(QFrame::NoFrame);
        smart_lighting->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        smart_lighting->setTextInteractionFlags(Qt::LinksAccessibleByKeyboard|Qt::LinksAccessibleByMouse);
        street_txt = new QTextBrowser(centralwidget);
        street_txt->setObjectName(QString::fromUtf8("street_txt"));
        street_txt->setGeometry(QRect(40, 220, 280, 30));
        QPalette palette2;
        palette2.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette2.setBrush(QPalette::Active, QPalette::Button, brush1);
        palette2.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette2.setBrush(QPalette::Active, QPalette::Midlight, brush3);
        palette2.setBrush(QPalette::Active, QPalette::Dark, brush4);
        palette2.setBrush(QPalette::Active, QPalette::Mid, brush5);
        palette2.setBrush(QPalette::Active, QPalette::Text, brush);
        palette2.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette2.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette2.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette2.setBrush(QPalette::Active, QPalette::Window, brush2);
        palette2.setBrush(QPalette::Active, QPalette::Shadow, brush6);
        palette2.setBrush(QPalette::Active, QPalette::Highlight, brush7);
        palette2.setBrush(QPalette::Active, QPalette::HighlightedText, brush2);
        palette2.setBrush(QPalette::Active, QPalette::Link, brush8);
        palette2.setBrush(QPalette::Active, QPalette::LinkVisited, brush9);
        palette2.setBrush(QPalette::Active, QPalette::AlternateBase, brush10);
        QBrush brush22(QColor(0, 0, 0, 255));
        brush22.setStyle(Qt::NoBrush);
        palette2.setBrush(QPalette::Active, QPalette::NoRole, brush22);
        palette2.setBrush(QPalette::Active, QPalette::ToolTipBase, brush12);
        palette2.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette2.setBrush(QPalette::Active, QPalette::PlaceholderText, brush13);
#endif
        palette2.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette2.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette2.setBrush(QPalette::Inactive, QPalette::Midlight, brush3);
        palette2.setBrush(QPalette::Inactive, QPalette::Dark, brush4);
        palette2.setBrush(QPalette::Inactive, QPalette::Mid, brush5);
        palette2.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette2.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette2.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette2.setBrush(QPalette::Inactive, QPalette::Shadow, brush6);
        palette2.setBrush(QPalette::Inactive, QPalette::Highlight, brush7);
        palette2.setBrush(QPalette::Inactive, QPalette::HighlightedText, brush2);
        palette2.setBrush(QPalette::Inactive, QPalette::Link, brush8);
        palette2.setBrush(QPalette::Inactive, QPalette::LinkVisited, brush9);
        palette2.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush10);
        QBrush brush23(QColor(0, 0, 0, 255));
        brush23.setStyle(Qt::NoBrush);
        palette2.setBrush(QPalette::Inactive, QPalette::NoRole, brush23);
        palette2.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush12);
        palette2.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette2.setBrush(QPalette::Inactive, QPalette::PlaceholderText, brush13);
#endif
        palette2.setBrush(QPalette::Disabled, QPalette::WindowText, brush15);
        palette2.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette2.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette2.setBrush(QPalette::Disabled, QPalette::Midlight, brush3);
        palette2.setBrush(QPalette::Disabled, QPalette::Dark, brush15);
        palette2.setBrush(QPalette::Disabled, QPalette::Mid, brush5);
        palette2.setBrush(QPalette::Disabled, QPalette::Text, brush15);
        palette2.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette2.setBrush(QPalette::Disabled, QPalette::ButtonText, brush15);
        palette2.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette2.setBrush(QPalette::Disabled, QPalette::Window, brush2);
        palette2.setBrush(QPalette::Disabled, QPalette::Shadow, brush16);
        palette2.setBrush(QPalette::Disabled, QPalette::Highlight, brush17);
        palette2.setBrush(QPalette::Disabled, QPalette::HighlightedText, brush2);
        palette2.setBrush(QPalette::Disabled, QPalette::Link, brush8);
        palette2.setBrush(QPalette::Disabled, QPalette::LinkVisited, brush9);
        palette2.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush10);
        QBrush brush24(QColor(0, 0, 0, 255));
        brush24.setStyle(Qt::NoBrush);
        palette2.setBrush(QPalette::Disabled, QPalette::NoRole, brush24);
        palette2.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush12);
        palette2.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette2.setBrush(QPalette::Disabled, QPalette::PlaceholderText, brush13);
#endif
        street_txt->setPalette(palette2);
        street_txt->setMouseTracking(false);
        street_txt->setFrameShape(QFrame::NoFrame);
        street_txt->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        street_txt->setTextInteractionFlags(Qt::LinksAccessibleByKeyboard|Qt::LinksAccessibleByMouse);
        postCode_txt = new QTextBrowser(centralwidget);
        postCode_txt->setObjectName(QString::fromUtf8("postCode_txt"));
        postCode_txt->setGeometry(QRect(40, 290, 280, 30));
        QPalette palette3;
        palette3.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette3.setBrush(QPalette::Active, QPalette::Button, brush1);
        palette3.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette3.setBrush(QPalette::Active, QPalette::Midlight, brush3);
        palette3.setBrush(QPalette::Active, QPalette::Dark, brush4);
        palette3.setBrush(QPalette::Active, QPalette::Mid, brush5);
        palette3.setBrush(QPalette::Active, QPalette::Text, brush);
        palette3.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette3.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette3.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette3.setBrush(QPalette::Active, QPalette::Window, brush2);
        palette3.setBrush(QPalette::Active, QPalette::Shadow, brush6);
        palette3.setBrush(QPalette::Active, QPalette::Highlight, brush7);
        palette3.setBrush(QPalette::Active, QPalette::HighlightedText, brush2);
        palette3.setBrush(QPalette::Active, QPalette::Link, brush8);
        palette3.setBrush(QPalette::Active, QPalette::LinkVisited, brush9);
        palette3.setBrush(QPalette::Active, QPalette::AlternateBase, brush10);
        QBrush brush25(QColor(0, 0, 0, 255));
        brush25.setStyle(Qt::NoBrush);
        palette3.setBrush(QPalette::Active, QPalette::NoRole, brush25);
        palette3.setBrush(QPalette::Active, QPalette::ToolTipBase, brush12);
        palette3.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette3.setBrush(QPalette::Active, QPalette::PlaceholderText, brush13);
#endif
        palette3.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette3.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette3.setBrush(QPalette::Inactive, QPalette::Midlight, brush3);
        palette3.setBrush(QPalette::Inactive, QPalette::Dark, brush4);
        palette3.setBrush(QPalette::Inactive, QPalette::Mid, brush5);
        palette3.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette3.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette3.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette3.setBrush(QPalette::Inactive, QPalette::Shadow, brush6);
        palette3.setBrush(QPalette::Inactive, QPalette::Highlight, brush7);
        palette3.setBrush(QPalette::Inactive, QPalette::HighlightedText, brush2);
        palette3.setBrush(QPalette::Inactive, QPalette::Link, brush8);
        palette3.setBrush(QPalette::Inactive, QPalette::LinkVisited, brush9);
        palette3.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush10);
        QBrush brush26(QColor(0, 0, 0, 255));
        brush26.setStyle(Qt::NoBrush);
        palette3.setBrush(QPalette::Inactive, QPalette::NoRole, brush26);
        palette3.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush12);
        palette3.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette3.setBrush(QPalette::Inactive, QPalette::PlaceholderText, brush13);
#endif
        palette3.setBrush(QPalette::Disabled, QPalette::WindowText, brush15);
        palette3.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette3.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette3.setBrush(QPalette::Disabled, QPalette::Midlight, brush3);
        palette3.setBrush(QPalette::Disabled, QPalette::Dark, brush15);
        palette3.setBrush(QPalette::Disabled, QPalette::Mid, brush5);
        palette3.setBrush(QPalette::Disabled, QPalette::Text, brush15);
        palette3.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette3.setBrush(QPalette::Disabled, QPalette::ButtonText, brush15);
        palette3.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette3.setBrush(QPalette::Disabled, QPalette::Window, brush2);
        palette3.setBrush(QPalette::Disabled, QPalette::Shadow, brush16);
        palette3.setBrush(QPalette::Disabled, QPalette::Highlight, brush17);
        palette3.setBrush(QPalette::Disabled, QPalette::HighlightedText, brush2);
        palette3.setBrush(QPalette::Disabled, QPalette::Link, brush8);
        palette3.setBrush(QPalette::Disabled, QPalette::LinkVisited, brush9);
        palette3.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush10);
        QBrush brush27(QColor(0, 0, 0, 255));
        brush27.setStyle(Qt::NoBrush);
        palette3.setBrush(QPalette::Disabled, QPalette::NoRole, brush27);
        palette3.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush12);
        palette3.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette3.setBrush(QPalette::Disabled, QPalette::PlaceholderText, brush13);
#endif
        postCode_txt->setPalette(palette3);
        postCode_txt->setMouseTracking(false);
        postCode_txt->setFrameShape(QFrame::NoFrame);
        postCode_txt->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        postCode_txt->setTextInteractionFlags(Qt::LinksAccessibleByKeyboard|Qt::LinksAccessibleByMouse);
        postCode = new QLineEdit(centralwidget);
        postCode->setObjectName(QString::fromUtf8("postCode"));
        postCode->setGeometry(QRect(40, 320, 280, 30));
        QFont font;
        font.setPointSize(13);
        postCode->setFont(font);
        postCode->setEchoMode(QLineEdit::Normal);
        street = new QLineEdit(centralwidget);
        street->setObjectName(QString::fromUtf8("street"));
        street->setGeometry(QRect(40, 250, 280, 30));
        street->setFont(font);
        street->setEchoMode(QLineEdit::Normal);
        addLamp_b = new QPushButton(centralwidget);
        addLamp_b->setObjectName(QString::fromUtf8("addLamp_b"));
        addLamp_b->setGeometry(QRect(200, 590, 121, 31));
        QPalette palette4;
        palette4.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette4.setBrush(QPalette::Active, QPalette::Button, brush1);
        palette4.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette4.setBrush(QPalette::Active, QPalette::Midlight, brush3);
        palette4.setBrush(QPalette::Active, QPalette::Dark, brush4);
        palette4.setBrush(QPalette::Active, QPalette::Mid, brush5);
        palette4.setBrush(QPalette::Active, QPalette::Text, brush);
        palette4.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette4.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette4.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette4.setBrush(QPalette::Active, QPalette::Window, brush2);
        palette4.setBrush(QPalette::Active, QPalette::Shadow, brush6);
        palette4.setBrush(QPalette::Active, QPalette::Highlight, brush7);
        palette4.setBrush(QPalette::Active, QPalette::HighlightedText, brush2);
        palette4.setBrush(QPalette::Active, QPalette::Link, brush8);
        palette4.setBrush(QPalette::Active, QPalette::LinkVisited, brush9);
        palette4.setBrush(QPalette::Active, QPalette::AlternateBase, brush10);
        QBrush brush28(QColor(0, 0, 0, 255));
        brush28.setStyle(Qt::NoBrush);
        palette4.setBrush(QPalette::Active, QPalette::NoRole, brush28);
        palette4.setBrush(QPalette::Active, QPalette::ToolTipBase, brush12);
        palette4.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette4.setBrush(QPalette::Active, QPalette::PlaceholderText, brush13);
#endif
        palette4.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette4.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette4.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette4.setBrush(QPalette::Inactive, QPalette::Midlight, brush3);
        palette4.setBrush(QPalette::Inactive, QPalette::Dark, brush4);
        palette4.setBrush(QPalette::Inactive, QPalette::Mid, brush5);
        palette4.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette4.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette4.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette4.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette4.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette4.setBrush(QPalette::Inactive, QPalette::Shadow, brush6);
        palette4.setBrush(QPalette::Inactive, QPalette::Highlight, brush7);
        palette4.setBrush(QPalette::Inactive, QPalette::HighlightedText, brush2);
        palette4.setBrush(QPalette::Inactive, QPalette::Link, brush8);
        palette4.setBrush(QPalette::Inactive, QPalette::LinkVisited, brush9);
        palette4.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush10);
        QBrush brush29(QColor(0, 0, 0, 255));
        brush29.setStyle(Qt::NoBrush);
        palette4.setBrush(QPalette::Inactive, QPalette::NoRole, brush29);
        palette4.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush12);
        palette4.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette4.setBrush(QPalette::Inactive, QPalette::PlaceholderText, brush13);
#endif
        palette4.setBrush(QPalette::Disabled, QPalette::WindowText, brush15);
        palette4.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette4.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette4.setBrush(QPalette::Disabled, QPalette::Midlight, brush3);
        palette4.setBrush(QPalette::Disabled, QPalette::Dark, brush15);
        palette4.setBrush(QPalette::Disabled, QPalette::Mid, brush5);
        palette4.setBrush(QPalette::Disabled, QPalette::Text, brush15);
        palette4.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette4.setBrush(QPalette::Disabled, QPalette::ButtonText, brush15);
        palette4.setBrush(QPalette::Disabled, QPalette::Base, brush2);
        palette4.setBrush(QPalette::Disabled, QPalette::Window, brush2);
        palette4.setBrush(QPalette::Disabled, QPalette::Shadow, brush16);
        palette4.setBrush(QPalette::Disabled, QPalette::Highlight, brush17);
        palette4.setBrush(QPalette::Disabled, QPalette::HighlightedText, brush2);
        palette4.setBrush(QPalette::Disabled, QPalette::Link, brush8);
        palette4.setBrush(QPalette::Disabled, QPalette::LinkVisited, brush9);
        palette4.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush10);
        QBrush brush30(QColor(0, 0, 0, 255));
        brush30.setStyle(Qt::NoBrush);
        palette4.setBrush(QPalette::Disabled, QPalette::NoRole, brush30);
        palette4.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush12);
        palette4.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette4.setBrush(QPalette::Disabled, QPalette::PlaceholderText, brush13);
#endif
        addLamp_b->setPalette(palette4);
        addLamp_b->setFont(font);
        back_b = new QPushButton(centralwidget);
        back_b->setObjectName(QString::fromUtf8("back_b"));
        back_b->setGeometry(QRect(20, 20, 31, 21));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/new/back_arrow/icon/arrow.png"), QSize(), QIcon::Normal, QIcon::Off);
        icon.addFile(QString::fromUtf8(":/new/back_arrow/icon/arrow.png"), QSize(), QIcon::Normal, QIcon::On);
        icon.addFile(QString::fromUtf8(":/new/back_arrow/icon/arrow.png"), QSize(), QIcon::Active, QIcon::Off);
        icon.addFile(QString::fromUtf8(":/new/back_arrow/icon/arrow.png"), QSize(), QIcon::Active, QIcon::On);
        icon.addFile(QString::fromUtf8(":/new/back_arrow/icon/arrow.png"), QSize(), QIcon::Selected, QIcon::Off);
        icon.addFile(QString::fromUtf8(":/new/back_arrow/icon/arrow.png"), QSize(), QIcon::Selected, QIcon::On);
        back_b->setIcon(icon);
        back_b->setIconSize(QSize(30, 25));
        back_b->setFlat(true);
        parish = new QLineEdit(centralwidget);
        parish->setObjectName(QString::fromUtf8("parish"));
        parish->setGeometry(QRect(40, 390, 280, 30));
        parish->setFont(font);
        parish->setEchoMode(QLineEdit::Normal);
        parish_txt = new QTextBrowser(centralwidget);
        parish_txt->setObjectName(QString::fromUtf8("parish_txt"));
        parish_txt->setGeometry(QRect(40, 360, 280, 30));
        QPalette palette5;
        palette5.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette5.setBrush(QPalette::Active, QPalette::Button, brush1);
        palette5.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette5.setBrush(QPalette::Active, QPalette::Midlight, brush3);
        palette5.setBrush(QPalette::Active, QPalette::Dark, brush4);
        palette5.setBrush(QPalette::Active, QPalette::Mid, brush5);
        palette5.setBrush(QPalette::Active, QPalette::Text, brush);
        palette5.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette5.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette5.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette5.setBrush(QPalette::Active, QPalette::Window, brush2);
        palette5.setBrush(QPalette::Active, QPalette::Shadow, brush6);
        palette5.setBrush(QPalette::Active, QPalette::Highlight, brush7);
        palette5.setBrush(QPalette::Active, QPalette::HighlightedText, brush2);
        palette5.setBrush(QPalette::Active, QPalette::Link, brush8);
        palette5.setBrush(QPalette::Active, QPalette::LinkVisited, brush9);
        palette5.setBrush(QPalette::Active, QPalette::AlternateBase, brush10);
        QBrush brush31(QColor(0, 0, 0, 255));
        brush31.setStyle(Qt::NoBrush);
        palette5.setBrush(QPalette::Active, QPalette::NoRole, brush31);
        palette5.setBrush(QPalette::Active, QPalette::ToolTipBase, brush12);
        palette5.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette5.setBrush(QPalette::Active, QPalette::PlaceholderText, brush13);
#endif
        palette5.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette5.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette5.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette5.setBrush(QPalette::Inactive, QPalette::Midlight, brush3);
        palette5.setBrush(QPalette::Inactive, QPalette::Dark, brush4);
        palette5.setBrush(QPalette::Inactive, QPalette::Mid, brush5);
        palette5.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette5.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette5.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette5.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette5.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette5.setBrush(QPalette::Inactive, QPalette::Shadow, brush6);
        palette5.setBrush(QPalette::Inactive, QPalette::Highlight, brush7);
        palette5.setBrush(QPalette::Inactive, QPalette::HighlightedText, brush2);
        palette5.setBrush(QPalette::Inactive, QPalette::Link, brush8);
        palette5.setBrush(QPalette::Inactive, QPalette::LinkVisited, brush9);
        palette5.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush10);
        QBrush brush32(QColor(0, 0, 0, 255));
        brush32.setStyle(Qt::NoBrush);
        palette5.setBrush(QPalette::Inactive, QPalette::NoRole, brush32);
        palette5.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush12);
        palette5.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette5.setBrush(QPalette::Inactive, QPalette::PlaceholderText, brush13);
#endif
        palette5.setBrush(QPalette::Disabled, QPalette::WindowText, brush15);
        palette5.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette5.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette5.setBrush(QPalette::Disabled, QPalette::Midlight, brush3);
        palette5.setBrush(QPalette::Disabled, QPalette::Dark, brush15);
        palette5.setBrush(QPalette::Disabled, QPalette::Mid, brush5);
        palette5.setBrush(QPalette::Disabled, QPalette::Text, brush15);
        palette5.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette5.setBrush(QPalette::Disabled, QPalette::ButtonText, brush15);
        palette5.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette5.setBrush(QPalette::Disabled, QPalette::Window, brush2);
        palette5.setBrush(QPalette::Disabled, QPalette::Shadow, brush16);
        palette5.setBrush(QPalette::Disabled, QPalette::Highlight, brush17);
        palette5.setBrush(QPalette::Disabled, QPalette::HighlightedText, brush2);
        palette5.setBrush(QPalette::Disabled, QPalette::Link, brush8);
        palette5.setBrush(QPalette::Disabled, QPalette::LinkVisited, brush9);
        palette5.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush10);
        QBrush brush33(QColor(0, 0, 0, 255));
        brush33.setStyle(Qt::NoBrush);
        palette5.setBrush(QPalette::Disabled, QPalette::NoRole, brush33);
        palette5.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush12);
        palette5.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette5.setBrush(QPalette::Disabled, QPalette::PlaceholderText, brush13);
#endif
        parish_txt->setPalette(palette5);
        parish_txt->setMouseTracking(false);
        parish_txt->setFrameShape(QFrame::NoFrame);
        parish_txt->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        parish_txt->setTextInteractionFlags(Qt::LinksAccessibleByKeyboard|Qt::LinksAccessibleByMouse);
        count_txt = new QTextBrowser(centralwidget);
        count_txt->setObjectName(QString::fromUtf8("count_txt"));
        count_txt->setGeometry(QRect(40, 430, 280, 30));
        QPalette palette6;
        palette6.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette6.setBrush(QPalette::Active, QPalette::Button, brush1);
        palette6.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette6.setBrush(QPalette::Active, QPalette::Midlight, brush3);
        palette6.setBrush(QPalette::Active, QPalette::Dark, brush4);
        palette6.setBrush(QPalette::Active, QPalette::Mid, brush5);
        palette6.setBrush(QPalette::Active, QPalette::Text, brush);
        palette6.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette6.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette6.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette6.setBrush(QPalette::Active, QPalette::Window, brush2);
        palette6.setBrush(QPalette::Active, QPalette::Shadow, brush6);
        palette6.setBrush(QPalette::Active, QPalette::Highlight, brush7);
        palette6.setBrush(QPalette::Active, QPalette::HighlightedText, brush2);
        palette6.setBrush(QPalette::Active, QPalette::Link, brush8);
        palette6.setBrush(QPalette::Active, QPalette::LinkVisited, brush9);
        palette6.setBrush(QPalette::Active, QPalette::AlternateBase, brush10);
        QBrush brush34(QColor(0, 0, 0, 255));
        brush34.setStyle(Qt::NoBrush);
        palette6.setBrush(QPalette::Active, QPalette::NoRole, brush34);
        palette6.setBrush(QPalette::Active, QPalette::ToolTipBase, brush12);
        palette6.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette6.setBrush(QPalette::Active, QPalette::PlaceholderText, brush13);
#endif
        palette6.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette6.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette6.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette6.setBrush(QPalette::Inactive, QPalette::Midlight, brush3);
        palette6.setBrush(QPalette::Inactive, QPalette::Dark, brush4);
        palette6.setBrush(QPalette::Inactive, QPalette::Mid, brush5);
        palette6.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette6.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette6.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette6.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette6.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette6.setBrush(QPalette::Inactive, QPalette::Shadow, brush6);
        palette6.setBrush(QPalette::Inactive, QPalette::Highlight, brush7);
        palette6.setBrush(QPalette::Inactive, QPalette::HighlightedText, brush2);
        palette6.setBrush(QPalette::Inactive, QPalette::Link, brush8);
        palette6.setBrush(QPalette::Inactive, QPalette::LinkVisited, brush9);
        palette6.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush10);
        QBrush brush35(QColor(0, 0, 0, 255));
        brush35.setStyle(Qt::NoBrush);
        palette6.setBrush(QPalette::Inactive, QPalette::NoRole, brush35);
        palette6.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush12);
        palette6.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette6.setBrush(QPalette::Inactive, QPalette::PlaceholderText, brush13);
#endif
        palette6.setBrush(QPalette::Disabled, QPalette::WindowText, brush15);
        palette6.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette6.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette6.setBrush(QPalette::Disabled, QPalette::Midlight, brush3);
        palette6.setBrush(QPalette::Disabled, QPalette::Dark, brush15);
        palette6.setBrush(QPalette::Disabled, QPalette::Mid, brush5);
        palette6.setBrush(QPalette::Disabled, QPalette::Text, brush15);
        palette6.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette6.setBrush(QPalette::Disabled, QPalette::ButtonText, brush15);
        palette6.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette6.setBrush(QPalette::Disabled, QPalette::Window, brush2);
        palette6.setBrush(QPalette::Disabled, QPalette::Shadow, brush16);
        palette6.setBrush(QPalette::Disabled, QPalette::Highlight, brush17);
        palette6.setBrush(QPalette::Disabled, QPalette::HighlightedText, brush2);
        palette6.setBrush(QPalette::Disabled, QPalette::Link, brush8);
        palette6.setBrush(QPalette::Disabled, QPalette::LinkVisited, brush9);
        palette6.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush10);
        QBrush brush36(QColor(0, 0, 0, 255));
        brush36.setStyle(Qt::NoBrush);
        palette6.setBrush(QPalette::Disabled, QPalette::NoRole, brush36);
        palette6.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush12);
        palette6.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette6.setBrush(QPalette::Disabled, QPalette::PlaceholderText, brush13);
#endif
        count_txt->setPalette(palette6);
        count_txt->setMouseTracking(false);
        count_txt->setFrameShape(QFrame::NoFrame);
        count_txt->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        count_txt->setTextInteractionFlags(Qt::LinksAccessibleByKeyboard|Qt::LinksAccessibleByMouse);
        county = new QLineEdit(centralwidget);
        county->setObjectName(QString::fromUtf8("county"));
        county->setGeometry(QRect(40, 460, 280, 30));
        county->setFont(font);
        county->setEchoMode(QLineEdit::Normal);
        district = new QLineEdit(centralwidget);
        district->setObjectName(QString::fromUtf8("district"));
        district->setGeometry(QRect(40, 530, 280, 30));
        district->setFont(font);
        district->setEchoMode(QLineEdit::Normal);
        district_txt = new QTextBrowser(centralwidget);
        district_txt->setObjectName(QString::fromUtf8("district_txt"));
        district_txt->setGeometry(QRect(40, 500, 280, 30));
        QPalette palette7;
        palette7.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette7.setBrush(QPalette::Active, QPalette::Button, brush1);
        palette7.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette7.setBrush(QPalette::Active, QPalette::Midlight, brush3);
        palette7.setBrush(QPalette::Active, QPalette::Dark, brush4);
        palette7.setBrush(QPalette::Active, QPalette::Mid, brush5);
        palette7.setBrush(QPalette::Active, QPalette::Text, brush);
        palette7.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette7.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette7.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette7.setBrush(QPalette::Active, QPalette::Window, brush2);
        palette7.setBrush(QPalette::Active, QPalette::Shadow, brush6);
        palette7.setBrush(QPalette::Active, QPalette::Highlight, brush7);
        palette7.setBrush(QPalette::Active, QPalette::HighlightedText, brush2);
        palette7.setBrush(QPalette::Active, QPalette::Link, brush8);
        palette7.setBrush(QPalette::Active, QPalette::LinkVisited, brush9);
        palette7.setBrush(QPalette::Active, QPalette::AlternateBase, brush10);
        QBrush brush37(QColor(0, 0, 0, 255));
        brush37.setStyle(Qt::NoBrush);
        palette7.setBrush(QPalette::Active, QPalette::NoRole, brush37);
        palette7.setBrush(QPalette::Active, QPalette::ToolTipBase, brush12);
        palette7.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette7.setBrush(QPalette::Active, QPalette::PlaceholderText, brush13);
#endif
        palette7.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette7.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette7.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette7.setBrush(QPalette::Inactive, QPalette::Midlight, brush3);
        palette7.setBrush(QPalette::Inactive, QPalette::Dark, brush4);
        palette7.setBrush(QPalette::Inactive, QPalette::Mid, brush5);
        palette7.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette7.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette7.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette7.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette7.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette7.setBrush(QPalette::Inactive, QPalette::Shadow, brush6);
        palette7.setBrush(QPalette::Inactive, QPalette::Highlight, brush7);
        palette7.setBrush(QPalette::Inactive, QPalette::HighlightedText, brush2);
        palette7.setBrush(QPalette::Inactive, QPalette::Link, brush8);
        palette7.setBrush(QPalette::Inactive, QPalette::LinkVisited, brush9);
        palette7.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush10);
        QBrush brush38(QColor(0, 0, 0, 255));
        brush38.setStyle(Qt::NoBrush);
        palette7.setBrush(QPalette::Inactive, QPalette::NoRole, brush38);
        palette7.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush12);
        palette7.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette7.setBrush(QPalette::Inactive, QPalette::PlaceholderText, brush13);
#endif
        palette7.setBrush(QPalette::Disabled, QPalette::WindowText, brush15);
        palette7.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette7.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette7.setBrush(QPalette::Disabled, QPalette::Midlight, brush3);
        palette7.setBrush(QPalette::Disabled, QPalette::Dark, brush15);
        palette7.setBrush(QPalette::Disabled, QPalette::Mid, brush5);
        palette7.setBrush(QPalette::Disabled, QPalette::Text, brush15);
        palette7.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette7.setBrush(QPalette::Disabled, QPalette::ButtonText, brush15);
        palette7.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette7.setBrush(QPalette::Disabled, QPalette::Window, brush2);
        palette7.setBrush(QPalette::Disabled, QPalette::Shadow, brush16);
        palette7.setBrush(QPalette::Disabled, QPalette::Highlight, brush17);
        palette7.setBrush(QPalette::Disabled, QPalette::HighlightedText, brush2);
        palette7.setBrush(QPalette::Disabled, QPalette::Link, brush8);
        palette7.setBrush(QPalette::Disabled, QPalette::LinkVisited, brush9);
        palette7.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush10);
        QBrush brush39(QColor(0, 0, 0, 255));
        brush39.setStyle(Qt::NoBrush);
        palette7.setBrush(QPalette::Disabled, QPalette::NoRole, brush39);
        palette7.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush12);
        palette7.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette7.setBrush(QPalette::Disabled, QPalette::PlaceholderText, brush13);
#endif
        district_txt->setPalette(palette7);
        district_txt->setMouseTracking(false);
        district_txt->setFrameShape(QFrame::NoFrame);
        district_txt->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        district_txt->setTextInteractionFlags(Qt::LinksAccessibleByKeyboard|Qt::LinksAccessibleByMouse);
        add_lamp_txt = new QTextBrowser(centralwidget);
        add_lamp_txt->setObjectName(QString::fromUtf8("add_lamp_txt"));
        add_lamp_txt->setGeometry(QRect(40, 180, 280, 30));
        QPalette palette8;
        palette8.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette8.setBrush(QPalette::Active, QPalette::Button, brush1);
        palette8.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette8.setBrush(QPalette::Active, QPalette::Midlight, brush3);
        palette8.setBrush(QPalette::Active, QPalette::Dark, brush4);
        palette8.setBrush(QPalette::Active, QPalette::Mid, brush5);
        palette8.setBrush(QPalette::Active, QPalette::Text, brush);
        palette8.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette8.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette8.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette8.setBrush(QPalette::Active, QPalette::Window, brush2);
        palette8.setBrush(QPalette::Active, QPalette::Shadow, brush6);
        palette8.setBrush(QPalette::Active, QPalette::Highlight, brush7);
        palette8.setBrush(QPalette::Active, QPalette::HighlightedText, brush2);
        palette8.setBrush(QPalette::Active, QPalette::Link, brush8);
        palette8.setBrush(QPalette::Active, QPalette::LinkVisited, brush9);
        palette8.setBrush(QPalette::Active, QPalette::AlternateBase, brush10);
        QBrush brush40(QColor(0, 0, 0, 255));
        brush40.setStyle(Qt::NoBrush);
        palette8.setBrush(QPalette::Active, QPalette::NoRole, brush40);
        palette8.setBrush(QPalette::Active, QPalette::ToolTipBase, brush12);
        palette8.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette8.setBrush(QPalette::Active, QPalette::PlaceholderText, brush13);
#endif
        palette8.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette8.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette8.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette8.setBrush(QPalette::Inactive, QPalette::Midlight, brush3);
        palette8.setBrush(QPalette::Inactive, QPalette::Dark, brush4);
        palette8.setBrush(QPalette::Inactive, QPalette::Mid, brush5);
        palette8.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette8.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette8.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette8.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette8.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette8.setBrush(QPalette::Inactive, QPalette::Shadow, brush6);
        palette8.setBrush(QPalette::Inactive, QPalette::Highlight, brush7);
        palette8.setBrush(QPalette::Inactive, QPalette::HighlightedText, brush2);
        palette8.setBrush(QPalette::Inactive, QPalette::Link, brush8);
        palette8.setBrush(QPalette::Inactive, QPalette::LinkVisited, brush9);
        palette8.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush10);
        QBrush brush41(QColor(0, 0, 0, 255));
        brush41.setStyle(Qt::NoBrush);
        palette8.setBrush(QPalette::Inactive, QPalette::NoRole, brush41);
        palette8.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush12);
        palette8.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette8.setBrush(QPalette::Inactive, QPalette::PlaceholderText, brush13);
#endif
        palette8.setBrush(QPalette::Disabled, QPalette::WindowText, brush15);
        palette8.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette8.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette8.setBrush(QPalette::Disabled, QPalette::Midlight, brush3);
        palette8.setBrush(QPalette::Disabled, QPalette::Dark, brush15);
        palette8.setBrush(QPalette::Disabled, QPalette::Mid, brush5);
        palette8.setBrush(QPalette::Disabled, QPalette::Text, brush15);
        palette8.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette8.setBrush(QPalette::Disabled, QPalette::ButtonText, brush15);
        palette8.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette8.setBrush(QPalette::Disabled, QPalette::Window, brush2);
        palette8.setBrush(QPalette::Disabled, QPalette::Shadow, brush16);
        palette8.setBrush(QPalette::Disabled, QPalette::Highlight, brush17);
        palette8.setBrush(QPalette::Disabled, QPalette::HighlightedText, brush2);
        palette8.setBrush(QPalette::Disabled, QPalette::Link, brush8);
        palette8.setBrush(QPalette::Disabled, QPalette::LinkVisited, brush9);
        palette8.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush10);
        QBrush brush42(QColor(0, 0, 0, 255));
        brush42.setStyle(Qt::NoBrush);
        palette8.setBrush(QPalette::Disabled, QPalette::NoRole, brush42);
        palette8.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush12);
        palette8.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette8.setBrush(QPalette::Disabled, QPalette::PlaceholderText, brush13);
#endif
        add_lamp_txt->setPalette(palette8);
        add_lamp_txt->setMouseTracking(false);
        add_lamp_txt->setFrameShape(QFrame::NoFrame);
        add_lamp_txt->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        add_lamp_txt->setTextInteractionFlags(Qt::LinksAccessibleByKeyboard|Qt::LinksAccessibleByMouse);
        addLamp->setCentralWidget(centralwidget);

        retranslateUi(addLamp);

        QMetaObject::connectSlotsByName(addLamp);
    } // setupUi

    void retranslateUi(QMainWindow *addLamp)
    {
        addLamp->setWindowTitle(QCoreApplication::translate("addLamp", "slipad", nullptr));
        SLIPAD->setHtml(QCoreApplication::translate("addLamp", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Ubuntu'; font-size:11pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:72pt;\">SLiPaD</span></p></body></html>", nullptr));
        smart_lighting->setHtml(QCoreApplication::translate("addLamp", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Ubuntu'; font-size:11pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:14pt; font-weight:700;\">Smart Lighting with Park Detection</span></p></body></html>", nullptr));
#if QT_CONFIG(accessibility)
        street_txt->setAccessibleName(QString());
#endif // QT_CONFIG(accessibility)
        street_txt->setHtml(QCoreApplication::translate("addLamp", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Ubuntu'; font-size:11pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:14pt;\">Street</span></p></body></html>", nullptr));
#if QT_CONFIG(accessibility)
        postCode_txt->setAccessibleName(QString());
#endif // QT_CONFIG(accessibility)
        postCode_txt->setHtml(QCoreApplication::translate("addLamp", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Ubuntu'; font-size:11pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:14pt;\">Post Code</span></p></body></html>", nullptr));
        addLamp_b->setText(QCoreApplication::translate("addLamp", "Add Lamppost", nullptr));
        back_b->setText(QString());
#if QT_CONFIG(accessibility)
        parish_txt->setAccessibleName(QString());
#endif // QT_CONFIG(accessibility)
        parish_txt->setHtml(QCoreApplication::translate("addLamp", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Ubuntu'; font-size:11pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:14pt;\">Parish</span></p></body></html>", nullptr));
#if QT_CONFIG(accessibility)
        count_txt->setAccessibleName(QString());
#endif // QT_CONFIG(accessibility)
        count_txt->setHtml(QCoreApplication::translate("addLamp", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Ubuntu'; font-size:11pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:14pt;\">County</span></p></body></html>", nullptr));
#if QT_CONFIG(accessibility)
        district_txt->setAccessibleName(QString());
#endif // QT_CONFIG(accessibility)
        district_txt->setHtml(QCoreApplication::translate("addLamp", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Ubuntu'; font-size:11pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:14pt;\">District</span></p></body></html>", nullptr));
#if QT_CONFIG(accessibility)
        add_lamp_txt->setAccessibleName(QString());
#endif // QT_CONFIG(accessibility)
        add_lamp_txt->setHtml(QCoreApplication::translate("addLamp", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Ubuntu'; font-size:11pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:14pt;\">Add Lamppost</span></p></body></html>", nullptr));
    } // retranslateUi

};

namespace Ui {
    class addLamp: public Ui_addLamp {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADDLAMP_H
