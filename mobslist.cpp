#include "mobslist.h"
#include "ui_mobslist.h"

#include "constants.h"

Mobslist::Mobslist(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Mobslist)
{
    ui->setupUi(this);
    setWindowTitle("List of mobs around");
    for (int i=0;i<NUMBER_OF_MOBS ;i++ ) {
        ui->list->addItem("invalid_value");
    }
}

Mobslist::~Mobslist()
{
    delete ui;
}

void Mobslist::closeEvent(QCloseEvent *e)
{
    e->ignore();
    hide();
    emit onClose(false);

}

void Mobslist::userPrint(QVector<Mob*> &mobs)
{
    foreach(Mob* mob, mobs){
        mob->refresh();
    }
    for (int i=0;i<NUMBER_OF_MOBS ;i++ ) {
        QString mobstr=mobs[i]->name()+" ";
        mobstr+=QString::number(mobs[i]->addr(),16)+" ";
        mobstr+=QString::number(mobs[i]->base(),16)+" ";
        mobstr+=mobs[i]->type()+" ";
        mobstr+=QString::number(mobs[i]->x())+" ";
        mobstr+=QString::number(mobs[i]->y())+" ";
        mobstr+=QString::number(mobs[i]->z())+" ";
        ui->list->item(i)->setText(mobstr);
    }
}

