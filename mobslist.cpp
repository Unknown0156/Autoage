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

void Mobslist::userPrint(const Mobs *mobs)
{

    QVector <Mob*> mobsList;
    if (ui->filtermobs->isChecked())
        mobsList=mobs->mobs();
    else
        mobsList=mobs->allmobs();
    setWindowTitle("List of mobs: "+QString::number(mobsList.size())+" mobs around");
    for (int i=0;i<mobsList.size() ;i++ ) {
        QString mobstr=mobsList[i]->name()+" ";
        mobstr+=QString::number(mobsList[i]->addr(),16)+" ";
        mobstr+=QString::number(mobsList[i]->base(),16)+" ";
        mobstr+=mobsList[i]->type()+" ";
        mobstr+=QString::number(mobsList[i]->x())+" ";
        mobstr+=QString::number(mobsList[i]->y())+" ";
        mobstr+=QString::number(mobsList[i]->z())+" ";
        mobstr+=QString::number(mobsList[i]->hp())+" ";
        mobstr+=QString::number(mobsList[i]->enemy());
        ui->list->item(i)->setText(mobstr);
    }
    for (int i=mobsList.size();i<NUMBER_OF_MOBS ;i++ ) {
        if(ui->list->item(i)->text()=="") break;
        else ui->list->item(i)->setText("");
    }
}

