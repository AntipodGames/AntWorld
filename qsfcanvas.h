#ifndef QSFCANVAS_H
#define QSFCANVAS_H
#include <iostream>
#include <sstream>
#include <QEvent>
#include <QKeyEvent>
#include "qsfml.h"
#include "displaymanager.h"
#define sizeX 100
#define sizeY 100

class QSFcanvas : public Qsfml
{

    Q_OBJECT

public:
    QSFcanvas(const QPoint& Position, const QSize& Size,QWidget *parent = 0);

signals:
    void sendStep(int);
    void sendCurrentState(int);
    void sendCursorPos(int,int);
    void sendNbrOfAnts(int);
    void runChange();

public slots:
    void reset();
    void stepbystep();
    void oneStepBack();
    void run();


private:
    World world;
    DisplayManager display;
    int step;
    sf::Clock Time;
    bool keepGoing = false;
    int currentState = 1;
    const int nbrStates = 12;

    bool leftClickPressed = false;

    void pause();

    void OnInit();
    void OnUpdate();
    void keyReleaseEvent(QKeyEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void wheelEvent(QWheelEvent *);
};

#endif // QSFCANVAS_H
