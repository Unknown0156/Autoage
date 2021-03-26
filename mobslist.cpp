#include "mobslist.h"
#include "ui_mobslist.h"

#include "constants.h"

Mobslist::Mobslist(QWidget *parent, const Mobs *mobs) :
    QWidget(parent), ui(new Ui::Mobslist),
    m_mobs(mobs)
{
    ui->setupUi(this);
    for (int i=0;i<m_mobs->allmobs().size() ;i++ ) {
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

void Mobslist::userPrint()
{

    QVector <Mob*> mobsList;
    if (ui->filtermobs->isChecked()){
        mobsList=m_mobs->mobs();
        for (int i=mobsList.size();i<ui->list->count();i++ ) {
            ui->list->item(i)->setHidden(true);
        }
    }else{
        mobsList=m_mobs->allmobs();
        for (int i=0;i<ui->list->count();i++ ) {
            ui->list->item(i)->setHidden(false);
        }
    }
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
}

