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
    explicit Waypoints(QWidget *parent = nullptr, const Player *player=nullptr, QVector <Point> *waypoints=nullptr);
    ~Waypoints();

    void closeEvent (QCloseEvent *e);

    void printPoints();

signals:
    void onClose(bool check);

private:
    Ui::Waypoints *ui;
    const Player *m_player;
    QVector <Point> *m_waypoints;

    void addPoint(const Point p);
    void addPlayerPoint();

    void savePoints();
    void openPoints();
};

#endif // WAYPOINTS_H
