#ifndef MOBSLIST_H
#define MOBSLIST_H

#include <QWidget>
#include <QCloseEvent>

#include "mob.h"

namespace Ui {
class Mobslist;
}

class Mobslist : public QWidget
{
    Q_OBJECT

public:
    explicit Mobslist(QWidget *parent = nullptr);
    ~Mobslist();

    void closeEvent (QCloseEvent *e);

    void userPrint(QVector<Mob*> &mobs);

signals:
    void onClose(bool check);

private:
    Ui::Mobslist *ui;
};

#endif // MOBSLIST_H
