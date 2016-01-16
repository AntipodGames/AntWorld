#ifndef INGAMEUI_H
#define INGAMEUI_H
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>



class Gui : public QWidget
{
    Q_OBJECT

public:
    Gui(){}
    Gui(QPoint, QSize, QWidget *parent = 0);

    QVBoxLayout* mainLayout;
    QHBoxLayout* menu;
    QVBoxLayout* statContainer;
    QVBoxLayout* statContainer2;

    QLabel* Step;
    QLabel* currentState;
    QLabel* cursorPosition;
    QLabel* nbrOfAnts;

    QPushButton* reset;
    QPushButton* oneStepBack;
    QPushButton* oneStep;
    QPushButton* run;



public slots:
    void updateStep(int scr);
    void updateCurrentState(int scr);
    void updateCursorPos(int x,int y);
    void updateNbrOfAnts(int);
};

#endif // INGAMEUI_H
