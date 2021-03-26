#ifndef MOBSLIST_H
#define MOBSLIST_H

#include <QWidget>
#include <QCloseEvent>

#include "mobs.h"

namespace Ui {
class Mobslist;
}

class Mobslist : public QWidget
{
    Q_OBJECT

public:
    explicit Mobslist(QWidget *parent = nullptr, const Mobs *mobs=nullptr);
    ~Mobslist();

    void closeEvent (QCloseEvent *e);

    void userPrint();

signals:
    void onClose(bool check);

private:
    Ui::Mobslist *ui;
    const Mobs *m_mobs;
};

#endif // MOBSLIST_H
