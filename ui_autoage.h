/********************************************************************************
** Form generated from reading UI file 'autoage.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AUTOAGE_H
#define UI_AUTOAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Autoage
{
public:
    QAction *quit;
    QAction *mobslist;
    QAction *radar;
    QAction *waypoints;
    QAction *openFile;
    QWidget *centralwidget;
    QPushButton *start;
    QPushButton *stop;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *pHpMp;
    QProgressBar *pHpBar;
    QProgressBar *pMpBar;
    QWidget *gridLayoutWidget;
    QGridLayout *pCoords;
    QLabel *pCos;
    QLabel *pX;
    QLabel *pY;
    QLabel *pSin;
    QLabel *pAngle;
    QLabel *pZ;
    QFrame *lineH;
    QWidget *verticalLayoutWidget_2;
    QVBoxLayout *tHpMp;
    QLabel *tName;
    QProgressBar *tHpBar;
    QFrame *lineV;
    QWidget *gridLayoutWidget_2;
    QGridLayout *tCoords;
    QLabel *tX;
    QLabel *tY;
    QLabel *tZ;
    QLabel *pStatus;
    QLabel *startPos;
    QLabel *mobs;
    QDoubleSpinBox *farmRange;
    QPushButton *wpMove;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuView;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *Autoage)
    {
        if (Autoage->objectName().isEmpty())
            Autoage->setObjectName(QString::fromUtf8("Autoage"));
        Autoage->resize(350, 255);
        quit = new QAction(Autoage);
        quit->setObjectName(QString::fromUtf8("quit"));
        mobslist = new QAction(Autoage);
        mobslist->setObjectName(QString::fromUtf8("mobslist"));
        mobslist->setCheckable(true);
        radar = new QAction(Autoage);
        radar->setObjectName(QString::fromUtf8("radar"));
        radar->setCheckable(true);
        waypoints = new QAction(Autoage);
        waypoints->setObjectName(QString::fromUtf8("waypoints"));
        waypoints->setCheckable(true);
        openFile = new QAction(Autoage);
        openFile->setObjectName(QString::fromUtf8("openFile"));
        centralwidget = new QWidget(Autoage);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        start = new QPushButton(centralwidget);
        start->setObjectName(QString::fromUtf8("start"));
        start->setGeometry(QRect(260, 130, 80, 22));
        stop = new QPushButton(centralwidget);
        stop->setObjectName(QString::fromUtf8("stop"));
        stop->setEnabled(false);
        stop->setGeometry(QRect(260, 190, 80, 22));
        verticalLayoutWidget = new QWidget(centralwidget);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(10, 0, 160, 51));
        pHpMp = new QVBoxLayout(verticalLayoutWidget);
        pHpMp->setSpacing(0);
        pHpMp->setObjectName(QString::fromUtf8("pHpMp"));
        pHpMp->setContentsMargins(0, 0, 0, 0);
        pHpBar = new QProgressBar(verticalLayoutWidget);
        pHpBar->setObjectName(QString::fromUtf8("pHpBar"));
        QFont font;
        font.setPointSize(9);
        pHpBar->setFont(font);
        pHpBar->setValue(24);
        pHpBar->setAlignment(Qt::AlignCenter);
        pHpBar->setTextDirection(QProgressBar::TopToBottom);

        pHpMp->addWidget(pHpBar);

        pMpBar = new QProgressBar(verticalLayoutWidget);
        pMpBar->setObjectName(QString::fromUtf8("pMpBar"));
        pMpBar->setFont(font);
        pMpBar->setValue(24);
        pMpBar->setAlignment(Qt::AlignCenter);

        pHpMp->addWidget(pMpBar);

        gridLayoutWidget = new QWidget(centralwidget);
        gridLayoutWidget->setObjectName(QString::fromUtf8("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(10, 50, 160, 61));
        pCoords = new QGridLayout(gridLayoutWidget);
        pCoords->setSpacing(2);
        pCoords->setObjectName(QString::fromUtf8("pCoords"));
        pCoords->setContentsMargins(0, 0, 0, 0);
        pCos = new QLabel(gridLayoutWidget);
        pCos->setObjectName(QString::fromUtf8("pCos"));
        pCos->setFont(font);

        pCoords->addWidget(pCos, 2, 0, 1, 1);

        pX = new QLabel(gridLayoutWidget);
        pX->setObjectName(QString::fromUtf8("pX"));
        pX->setFont(font);

        pCoords->addWidget(pX, 1, 1, 1, 1);

        pY = new QLabel(gridLayoutWidget);
        pY->setObjectName(QString::fromUtf8("pY"));
        pY->setFont(font);

        pCoords->addWidget(pY, 2, 1, 1, 1);

        pSin = new QLabel(gridLayoutWidget);
        pSin->setObjectName(QString::fromUtf8("pSin"));
        pSin->setFont(font);

        pCoords->addWidget(pSin, 1, 0, 1, 1);

        pAngle = new QLabel(gridLayoutWidget);
        pAngle->setObjectName(QString::fromUtf8("pAngle"));

        pCoords->addWidget(pAngle, 3, 0, 1, 1);

        pZ = new QLabel(gridLayoutWidget);
        pZ->setObjectName(QString::fromUtf8("pZ"));
        pZ->setFont(font);

        pCoords->addWidget(pZ, 3, 1, 1, 1);

        lineH = new QFrame(centralwidget);
        lineH->setObjectName(QString::fromUtf8("lineH"));
        lineH->setGeometry(QRect(0, 111, 371, 20));
        lineH->setFrameShape(QFrame::HLine);
        lineH->setFrameShadow(QFrame::Sunken);
        verticalLayoutWidget_2 = new QWidget(centralwidget);
        verticalLayoutWidget_2->setObjectName(QString::fromUtf8("verticalLayoutWidget_2"));
        verticalLayoutWidget_2->setGeometry(QRect(180, 0, 160, 51));
        tHpMp = new QVBoxLayout(verticalLayoutWidget_2);
        tHpMp->setSpacing(6);
        tHpMp->setObjectName(QString::fromUtf8("tHpMp"));
        tHpMp->setContentsMargins(0, 0, 0, 0);
        tName = new QLabel(verticalLayoutWidget_2);
        tName->setObjectName(QString::fromUtf8("tName"));
        tName->setEnabled(true);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(tName->sizePolicy().hasHeightForWidth());
        tName->setSizePolicy(sizePolicy);
        QFont font1;
        font1.setPointSize(10);
        tName->setFont(font1);
        tName->setAlignment(Qt::AlignCenter);

        tHpMp->addWidget(tName);

        tHpBar = new QProgressBar(verticalLayoutWidget_2);
        tHpBar->setObjectName(QString::fromUtf8("tHpBar"));
        tHpBar->setFont(font);
        tHpBar->setValue(24);
        tHpBar->setAlignment(Qt::AlignCenter);

        tHpMp->addWidget(tHpBar);

        lineV = new QFrame(centralwidget);
        lineV->setObjectName(QString::fromUtf8("lineV"));
        lineV->setGeometry(QRect(165, 0, 20, 121));
        lineV->setFrameShape(QFrame::VLine);
        lineV->setFrameShadow(QFrame::Sunken);
        gridLayoutWidget_2 = new QWidget(centralwidget);
        gridLayoutWidget_2->setObjectName(QString::fromUtf8("gridLayoutWidget_2"));
        gridLayoutWidget_2->setGeometry(QRect(180, 50, 160, 61));
        tCoords = new QGridLayout(gridLayoutWidget_2);
        tCoords->setSpacing(2);
        tCoords->setObjectName(QString::fromUtf8("tCoords"));
        tCoords->setContentsMargins(0, 0, 0, 0);
        tX = new QLabel(gridLayoutWidget_2);
        tX->setObjectName(QString::fromUtf8("tX"));
        tX->setFont(font);

        tCoords->addWidget(tX, 0, 0, 1, 1);

        tY = new QLabel(gridLayoutWidget_2);
        tY->setObjectName(QString::fromUtf8("tY"));
        tY->setFont(font);

        tCoords->addWidget(tY, 1, 0, 1, 1);

        tZ = new QLabel(gridLayoutWidget_2);
        tZ->setObjectName(QString::fromUtf8("tZ"));
        tZ->setFont(font);

        tCoords->addWidget(tZ, 2, 0, 1, 1);

        pStatus = new QLabel(centralwidget);
        pStatus->setObjectName(QString::fromUtf8("pStatus"));
        pStatus->setGeometry(QRect(10, 130, 120, 20));
        pStatus->setFont(font1);
        startPos = new QLabel(centralwidget);
        startPos->setObjectName(QString::fromUtf8("startPos"));
        startPos->setGeometry(QRect(10, 160, 240, 20));
        startPos->setFont(font1);
        mobs = new QLabel(centralwidget);
        mobs->setObjectName(QString::fromUtf8("mobs"));
        mobs->setGeometry(QRect(10, 190, 240, 20));
        mobs->setFont(font1);
        farmRange = new QDoubleSpinBox(centralwidget);
        farmRange->setObjectName(QString::fromUtf8("farmRange"));
        farmRange->setGeometry(QRect(180, 130, 62, 22));
        farmRange->setMaximum(999.000000000000000);
        farmRange->setValue(100.000000000000000);
        wpMove = new QPushButton(centralwidget);
        wpMove->setObjectName(QString::fromUtf8("wpMove"));
        wpMove->setGeometry(QRect(260, 160, 80, 22));
        Autoage->setCentralWidget(centralwidget);
        menubar = new QMenuBar(Autoage);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 350, 21));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuView = new QMenu(menubar);
        menuView->setObjectName(QString::fromUtf8("menuView"));
        Autoage->setMenuBar(menubar);
        statusbar = new QStatusBar(Autoage);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        Autoage->setStatusBar(statusbar);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuView->menuAction());
        menuFile->addAction(openFile);
        menuFile->addAction(quit);
        menuView->addAction(radar);
        menuView->addAction(mobslist);
        menuView->addAction(waypoints);

        retranslateUi(Autoage);

        QMetaObject::connectSlotsByName(Autoage);
    } // setupUi

    void retranslateUi(QMainWindow *Autoage)
    {
        Autoage->setWindowTitle(QCoreApplication::translate("Autoage", "Autoage", nullptr));
        quit->setText(QCoreApplication::translate("Autoage", "Quit", nullptr));
        mobslist->setText(QCoreApplication::translate("Autoage", "Mobs list", nullptr));
        radar->setText(QCoreApplication::translate("Autoage", "Radar", nullptr));
        waypoints->setText(QCoreApplication::translate("Autoage", "Waypoints", nullptr));
        openFile->setText(QCoreApplication::translate("Autoage", "Open", nullptr));
        start->setText(QCoreApplication::translate("Autoage", "Start", nullptr));
        stop->setText(QCoreApplication::translate("Autoage", "Stop", nullptr));
        pHpBar->setFormat(QCoreApplication::translate("Autoage", "HP:%v/%m", nullptr));
        pMpBar->setFormat(QCoreApplication::translate("Autoage", "MP:%v/%m", nullptr));
        pCos->setText(QCoreApplication::translate("Autoage", "player Cos", nullptr));
        pX->setText(QCoreApplication::translate("Autoage", "player X", nullptr));
        pY->setText(QCoreApplication::translate("Autoage", "player Y", nullptr));
        pSin->setText(QCoreApplication::translate("Autoage", "player Sin", nullptr));
        pAngle->setText(QCoreApplication::translate("Autoage", "player Angle", nullptr));
        pZ->setText(QCoreApplication::translate("Autoage", "player Z", nullptr));
        tName->setText(QCoreApplication::translate("Autoage", "target name", nullptr));
        tHpBar->setFormat(QCoreApplication::translate("Autoage", "HP:%v/%m", nullptr));
        tX->setText(QCoreApplication::translate("Autoage", "target X", nullptr));
        tY->setText(QCoreApplication::translate("Autoage", "target Y", nullptr));
        tZ->setText(QCoreApplication::translate("Autoage", "target Z", nullptr));
        pStatus->setText(QCoreApplication::translate("Autoage", "Player is waiting", nullptr));
        startPos->setText(QCoreApplication::translate("Autoage", "Start position", nullptr));
        mobs->setText(QCoreApplication::translate("Autoage", "Mobs around: 0", nullptr));
        wpMove->setText(QCoreApplication::translate("Autoage", "Points move", nullptr));
        menuFile->setTitle(QCoreApplication::translate("Autoage", "File", nullptr));
        menuView->setTitle(QCoreApplication::translate("Autoage", "View", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Autoage: public Ui_Autoage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AUTOAGE_H
