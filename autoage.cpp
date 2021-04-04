#include "autoage.h"
#include "ui_autoage.h"

Autoage::Autoage(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::Autoage),
      target(new Target), player(new Player(target)), mobs(new Mobs),  waypoints(new QVector <Point>)
{
    ui->setupUi(this);
    setFixedSize(geometry().width(), geometry().height());//фиксирует размер окна
    statusBar()->showMessage("Ready");
    ui->pHpBar->setStyleSheet("QProgressBar::chunk{background-color: #6B9F18;}");//цвет хп бара персонажа
    ui->pMpBar->setStyleSheet("QProgressBar::chunk{background-color: #2B7ED5;}");//цвет мана бара персонажа
    ui->tHpBar->setStyleSheet("QProgressBar::chunk{background-color: #6B9F18;}");//цвет хп бара цели

    stPos={player->x(), player->y(), player->z()};//инициализация стартовой позиции
    farmRange=ui->farmRange->value();//радуис фарма из ui
    timerId=startTimer(TIMER_DELAY);//таймер главного окна

    connect(player, &Player::statusChanged, ui->pStatus, &QLabel::setText);//статус персонажа в ui
    connect(player, &Player::sendStatus, statusBar(), &QStatusBar::showMessage);//сообщения в статус бар

    connect(ui->openFile, &QAction::triggered, this, &Autoage::openFile);
    connect(ui->quit, &QAction::triggered, qApp, &QApplication::quit);
    connect(ui->radar, &QAction::triggered, this, &Autoage::radarSH);
    connect(ui->mobslist, &QAction::triggered, this, &Autoage::mobslistSH);
    connect(ui->waypoints, &QAction::triggered, this, &Autoage::waypointsSH);
    connect(ui->start, &QPushButton::clicked, this, &Autoage::start);
    connect(ui->wpMove, &QPushButton::clicked, this, &Autoage::wpMove);
    connect(ui->stop, &QPushButton::clicked, this, &Autoage::stop);
}

Autoage::~Autoage()
{
    delete ui;
    if (mobslist!=nullptr)
        delete mobslist;
    if (radar!=nullptr)
        delete radar;
    if (waypointslist!=nullptr)
        delete waypointslist;
    if(bot!=nullptr)
        delete bot;
    if(move!=nullptr)
        delete move;
    if(player!=nullptr)
        delete player;
    if(target!=nullptr)
        delete target;
    if(mobs!=nullptr)
        delete mobs;
    if(waypoints!=nullptr){
        waypoints->clear();
        delete waypoints;
    }
}

void Autoage::timerEvent(QTimerEvent *e)
{
    Q_UNUSED(e);
    mobs->refresh();
    mobs->filter();
    userPrint(); //вывод в ui по таймеру
}

void Autoage::closeEvent(QCloseEvent *e){
    Q_UNUSED(e);
    if (mobslist!=nullptr){
        mobslist->close();
    }
    if (waypointslist!=nullptr){
        waypointslist->close();
    }
}

void Autoage::radarSH()//показать\скрыть радар
{
    if (radar==nullptr){
        radar=new Radar(stPos, farmRange, this, player, target, &mobs->mobs(), waypoints);
        radar->move(0,geometry().height()-statusBar()->height());
    }
    if (ui->radar->isChecked()){
        radar->show();
        this->setFixedSize(geometry().width(), geometry().height()+radar->height());
    }else{
        this->setFixedSize(geometry().width(), geometry().height()-radar->height());
        delete radar;
        radar=nullptr;
    }
}
void Autoage::mobslistSH()//показать\скрыть окно списка мобов
{
    if (mobslist==nullptr){
        mobslist=new Mobslist(nullptr, mobs);
        connect(mobslist, &Mobslist::onClose, ui->mobslist, &QAction::setChecked);
    }
    if (ui->mobslist->isChecked())
        mobslist->show();
    else
        mobslist->hide();
}
void Autoage::waypointsSH()
{
    if (waypointslist==nullptr){
        waypointslist=new Waypoints(nullptr, player, waypoints);
        connect(waypointslist, &Waypoints::onClose, ui->waypoints, &QAction::setChecked);
    }
    if (ui->waypoints->isChecked())
        waypointslist->show();
    else
        waypointslist->hide();
}

bool Autoage::openFile()
{
    QString filename= QFileDialog::getOpenFileName(this, tr("Open waypoints"), QCoreApplication::applicationDirPath(), tr("Points files(*.pnt)"));
    QFile pFile(filename);
    if(pFile.open(QIODevice::ReadOnly)){
        QDataStream stream(&pFile);
        waypoints->clear();
        while(!pFile.atEnd()){
            Point p;
            stream>>p.x>>p.y>>p.z;
            waypoints->push_back(p);
        }
        pFile.close();
        if(waypointslist!=nullptr)
            waypointslist->printPoints();
        return true;
    }else{
        QMessageBox msgBox;
        msgBox.setText("Cant open file!");
        msgBox.exec();
        return false;
    }
}

void Autoage::userPrint()//вывод данных в ui
{
    //Стартовая позиция
    ui->startPos->setText("Start at ("+QString::number(stPos.x)+", "+QString::number(stPos.y)+"), dist="+QString::number(player->distTo(stPos),'f',2));

    //Персонаж
    QString title=QString::number(player->x(),'f',1)+" "+QString::number(player->y(),'f',1)+" "+player->nick();//заголовок окна
    setWindowTitle(title);
    ui->pHpBar->setMaximum(player->maxHp());
    ui->pHpBar->setValue(player->hp());
    ui->pMpBar->setMaximum(player->maxMp());
    ui->pMpBar->setValue(player->mp());
    ui->pX->setText("X: "+QString::number(player->x()));
    ui->pY->setText("Y: "+QString::number(player->y()));
    ui->pZ->setText("Z: "+QString::number(player->z()));
    ui->pCos->setText("Cos: "+QString::number(player->cos()));
    ui->pSin->setText("Sin: "+QString::number(player->sin()));
    ui->pAngle->setText("Angle: "+QString::number(player->angle()));


    //Таргет
    ui->tName->setText(target->name());
    ui->tHpBar->setMaximum(target->maxHp());
    ui->tHpBar->setValue(target->hp());
    ui->tX->setText("X: "+QString::number(target->x()));
    ui->tY->setText("Y: "+QString::number(target->y()));
    ui->tZ->setText("Z: "+QString::number(target->z()));

    //Мобы
    ui->mobs->setText("Mobs around: "+QString::number(mobs->mobs().size()));
    if(mobslist!=nullptr)
        mobslist->userPrint();
    if(radar!=nullptr)
        radar->repaint();
}

void Autoage::start()
{  
    ui->start->setDisabled(true);
    ui->wpMove->setDisabled(true);
    ui->stop->setEnabled(true);
    stPos={player->x(), player->y(), player->z()};
    farmRange=ui->farmRange->value();//радуис фарма из ui
    player->setGlobal(false);
    player->start();
    if(bot==nullptr){
        bot=new QTimer(this);
        connect(bot, &QTimer::timeout, this, &Autoage::botting);
    }
    bot->start(TIMER_DELAY);
}

void Autoage::wpMove()
{
    if(waypoints->size()==0)
        if(!openFile())
            return;
    ui->start->setDisabled(true);
    ui->wpMove->setDisabled(true);
    ui->stop->setEnabled(true);
    stPos={0,0,0};
    farmRange=0.0f;
    player->setGlobal(true);
    player->start();
    if(move==nullptr){
        move=new QTimer(this);
        connect(move, &QTimer::timeout, this, &Autoage::moving);
    }
    if(nextPoint==nullptr){//нахождение ближайшей точки из массива
        nextPoint=waypoints->begin();
        foreach(auto &p, *waypoints){
            if(player->distTo(p)<player->distTo(*nextPoint))
                nextPoint=&p;
        }
    }
    move->start(TIMER_DELAY);

}

void Autoage::stop()
{ 
    ui->stop->setDisabled(true);
    ui->start->setEnabled(true);
    ui->wpMove->setEnabled(true);
    if(bot!=nullptr){
        bot->stop();
        delete bot;
        bot=nullptr;
    }
    if(move!=nullptr){
        move->stop();
        delete move;
        move=nullptr;
    }
    player->stop();  
}


