#ifndef WAYPOINTS_H
#define WAYPOINTS_H

#include <QWidget>
#include <QCloseEvent>
#include <QFileDialog>
#include <QMessageBox>

#include "player.h"
#include "mobs.h"

namespace Ui {
class Waypoints;
}

class Waypoints : public QWidget
{
    Q_OBJECT

public:
    explicit Waypoints(QVector <Point> *waypoints, QWidget *parent = nullptr, const Player *player=nullptr);
    ~Waypoints();

    void closeEvent (QCloseEvent *e);

    void savePoints();
    void openPoints();
    void userPrint();

signals:
    void onClose(bool check);

private:
    Ui::Waypoints *ui;
    const Player *m_player;
    QVector <Point> *m_waypoints;

    void addPoint(const Point p);
    void addPlayerPoint();
};

#endif // WAYPOINTS_H
