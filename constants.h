#pragma once

#include <QString>

const QString F_NAME_SETTINGS = "settings.ini"; //файл настроек
const QString F_NAME_OFFSETS = "offsets.ini"; //файл оффсетов
const QString TITLE_CONTAIN = "Arche"; //заголовок окна содержит
const int TIMER_DELAY = 100; //задержка таймера главного окна

const int NUMBER_OF_MOBS = 3+9+27+27*3;//колличество указателей на мобов
const int MOB_BASE = 969501312; //значение vtable моба

const float TURN_PRECISION = 6.0f; //точность поворота
const float MOVE_TO_POINT_PRECISION = 2.0f; //точность движения к точке
const float MOVE_TO_MOB_PRECISION = 20.0f; //точность движения к таргету
const float MAX_DIST_FROM_START=100.0f;//максимальное расстояние от начальной точки

