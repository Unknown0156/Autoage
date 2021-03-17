#include "mobslist.h"
#include "ui_mobslist.h"

#include "constants.h"

Mobslist::Mobslist(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Mobslist)
{
    ui->setupUi(this);
    for (int i=0;i<NUMBER_OF_MOBS ;i++ ) {
        ui->list->addItem("");
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

void Mobslist::userPrint(const QVector<Mob*> &mobs)
{
    setWindowTitle("List of mobs: "+QString::number(mobs.size())+" mobs around");
    for (int i=0;i<mobs.size() ;i++ ) {
        QString mobstr=mobs[i]->name()+" ";
        mobstr+=QString::number(mobs[i]->addr(),16)+" ";
        mobstr+=QString::number(mobs[i]->base(),16)+" ";
        mobstr+=mobs[i]->type()+" ";
        mobstr+=QString::number(mobs[i]->x())+" ";
        mobstr+=QString::number(mobs[i]->y())+" ";
        mobstr+=QString::number(mobs[i]->z())+" ";
        mobstr+=QString::number(mobs[i]->hp())+" ";
        mobstr+=QString::number(mobs[i]->enemy());
        ui->list->item(i)->setText(mobstr);
    }
    for (int i=mobs.size();i<NUMBER_OF_MOBS ;i++ ) {
        if(ui->list->item(i)->text()=="") break;
        else ui->list->item(i)->setText("");
    }
}

