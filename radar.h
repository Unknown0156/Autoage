#ifndef RADAR_H
#define RADAR_H

#include <QWidget>
#include <QPainter>
#include <QCloseEvent>

#include "constants.h"

namespace Ui {
class Radar;
}

class Radar : public QWidget
{
    Q_OBJECT

public:
    explicit Radar(QWidget *parent = nullptr);
    ~Radar();

    void timerEvent(QTimerEvent *e);
    void paintEvent(QPaintEvent *e);

    void closeEvent (QCloseEvent *e);

signals:
    void onClose(bool check);

private:
    int timerId;
    Ui::Radar *ui;
    QImage pimage;
    QRect prect;
};

#endif // RADAR_H
