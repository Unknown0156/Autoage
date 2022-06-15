/********************************************************************************
** Form generated from reading UI file 'waypoints.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WAYPOINTS_H
#define UI_WAYPOINTS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Waypoints
{
public:
    QPushButton *pointAdd;
    QPushButton *savePoints;
    QPushButton *openPoints;
    QListWidget *pointsList;
    QPushButton *pointDelete;

    void setupUi(QWidget *Waypoints)
    {
        if (Waypoints->objectName().isEmpty())
            Waypoints->setObjectName(QString::fromUtf8("Waypoints"));
        Waypoints->resize(400, 390);
        pointAdd = new QPushButton(Waypoints);
        pointAdd->setObjectName(QString::fromUtf8("pointAdd"));
        pointAdd->setGeometry(QRect(310, 20, 80, 22));
        savePoints = new QPushButton(Waypoints);
        savePoints->setObjectName(QString::fromUtf8("savePoints"));
        savePoints->setGeometry(QRect(310, 100, 80, 22));
        openPoints = new QPushButton(Waypoints);
        openPoints->setObjectName(QString::fromUtf8("openPoints"));
        openPoints->setGeometry(QRect(310, 140, 80, 22));
        pointsList = new QListWidget(Waypoints);
        pointsList->setObjectName(QString::fromUtf8("pointsList"));
        pointsList->setGeometry(QRect(10, 10, 281, 361));
        pointDelete = new QPushButton(Waypoints);
        pointDelete->setObjectName(QString::fromUtf8("pointDelete"));
        pointDelete->setGeometry(QRect(310, 60, 80, 22));

        retranslateUi(Waypoints);

        QMetaObject::connectSlotsByName(Waypoints);
    } // setupUi

    void retranslateUi(QWidget *Waypoints)
    {
        Waypoints->setWindowTitle(QCoreApplication::translate("Waypoints", "Form", nullptr));
        pointAdd->setText(QCoreApplication::translate("Waypoints", "Add point", nullptr));
        savePoints->setText(QCoreApplication::translate("Waypoints", "Save points", nullptr));
        openPoints->setText(QCoreApplication::translate("Waypoints", "Open points", nullptr));
        pointDelete->setText(QCoreApplication::translate("Waypoints", "Delete point", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Waypoints: public Ui_Waypoints {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WAYPOINTS_H
