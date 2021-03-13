#include "radar.h"
#include "ui_radar.h"

Radar::Radar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Radar)
{
    ui->setupUi(this);
    setWindowTitle("Radar");
    pimage.load(QCoreApplication::applicationDirPath()+"/"+"player.png");
    prect=pimage.rect();
    timerId=startTimer(100);
}

Radar::~Radar()
{
    delete ui;
}

void Radar::timerEvent(QTimerEvent *e){
    Q_UNUSED(e);
    repaint();
}

void Radar::paintEvent(QPaintEvent *e){
    Q_UNUSED(e);
    QPainter painter(this);
    int w=this->width();
    int h=this->height();
    painter.translate(w/2,h/2);
    painter.drawImage(prect, pimage);
}

void Radar::closeEvent(QCloseEvent *e)
{
    e->ignore();
    hide();
    emit onClose(false);

}
