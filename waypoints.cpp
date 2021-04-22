#include "waypoints.h"
#include "ui_waypoints.h"

Waypoints::Waypoints(QWidget *parent, const Player *player, QVector <Point> *waypoints) :
    QWidget(parent),
    ui(new Ui::Waypoints), m_player(player), m_waypoints(waypoints)
{
    ui->setupUi(this);
    setWindowTitle("Waypoints");
    if(m_waypoints->size()!=0)
        printPoints();
    connect(ui->pointAdd, &QPushButton::clicked,this, &Waypoints::addPlayerPoint);
    connect(ui->pointDelete, &QPushButton::clicked,this, &Waypoints::deletePoint);
    connect(ui->savePoints, &QPushButton::clicked, this, &Waypoints::savePoints);
    connect(ui->openPoints, &QPushButton::clicked, this, &Waypoints::openPoints);
}

Waypoints::~Waypoints()
{
    delete ui;
}

void Waypoints::closeEvent(QCloseEvent *e)
{
    e->ignore();
    hide();
    emit onClose(false);
}

void Waypoints::savePoints()
{
    QString filename= QFileDialog::getSaveFileName(this, tr("Save waypoints"), QCoreApplication::applicationDirPath(), tr("Points files(*.pnt)"));
    QFile pFile(filename);
    if(pFile.open(QIODevice::WriteOnly)){
        QDataStream stream(&pFile);
        foreach(Point p, *m_waypoints)
        {
            stream<<p.x<<p.y<<p.z;
        }
        pFile.close();
    }else{
        QMessageBox msgBox;
        msgBox.setText("Cant save file!");
        msgBox.exec();
    }
}

void Waypoints::openPoints()
{
    QString filename= QFileDialog::getOpenFileName(this, tr("Open waypoints"), QCoreApplication::applicationDirPath(), tr("Points files(*.pnt)"));
    QFile pFile(filename);
    if(pFile.open(QIODevice::ReadOnly)){
        QDataStream stream(&pFile);
        m_waypoints->clear();
        while(!pFile.atEnd()){
            Point p;
            stream>>p.x>>p.y>>p.z;
            m_waypoints->push_back(p);
        }
        pFile.close();
        printPoints();
    }else{
        QMessageBox msgBox;
        msgBox.setText("Cant open file!");
        msgBox.exec();
    }
}

void Waypoints::printPoints()
{
    ui->pointsList->clear();
    foreach(Point p, *m_waypoints)
    {
        QString pStr=QString::number(p.x)+" "+QString::number(p.y)+" "+QString::number(p.z);
        ui->pointsList->addItem(pStr);
    }
}

void Waypoints::addPoint(const Point p)
{
    m_waypoints->push_back(p);
    QString pStr=QString::number(p.x)+" "+QString::number(p.y)+" "+QString::number(p.z);
    ui->pointsList->addItem(pStr);
}

void Waypoints::addPlayerPoint()
{
    Point p{(float)m_player->gx(), (float)m_player->gy(), (float)m_player->gz()};
    addPoint(p);
}

void Waypoints::deletePoint()
{
    int curRow=ui->pointsList->currentIndex().row();
    ui->pointsList->model()->removeRow(curRow);
    if(m_waypoints->size()>0)
        m_waypoints->remove(curRow);
}
