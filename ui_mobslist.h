/********************************************************************************
** Form generated from reading UI file 'mobslist.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MOBSLIST_H
#define UI_MOBSLIST_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Mobslist
{
public:
    QListWidget *list;
    QCheckBox *filtermobs;

    void setupUi(QWidget *Mobslist)
    {
        if (Mobslist->objectName().isEmpty())
            Mobslist->setObjectName(QString::fromUtf8("Mobslist"));
        Mobslist->resize(500, 400);
        list = new QListWidget(Mobslist);
        list->setObjectName(QString::fromUtf8("list"));
        list->setGeometry(QRect(10, 10, 480, 350));
        filtermobs = new QCheckBox(Mobslist);
        filtermobs->setObjectName(QString::fromUtf8("filtermobs"));
        filtermobs->setGeometry(QRect(390, 370, 91, 20));
        QFont font;
        font.setPointSize(10);
        filtermobs->setFont(font);

        retranslateUi(Mobslist);

        QMetaObject::connectSlotsByName(Mobslist);
    } // setupUi

    void retranslateUi(QWidget *Mobslist)
    {
        Mobslist->setWindowTitle(QCoreApplication::translate("Mobslist", "Form", nullptr));
        filtermobs->setText(QCoreApplication::translate("Mobslist", "Filter mobs", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Mobslist: public Ui_Mobslist {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MOBSLIST_H
