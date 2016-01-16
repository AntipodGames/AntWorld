#include "gui.h"


Gui::Gui(QPoint pos, QSize size,QWidget* parent) :QWidget(parent)
{



    mainLayout = new QVBoxLayout();
    menu = new QHBoxLayout();
    statContainer = new QVBoxLayout();
    statContainer2 = new QVBoxLayout();

    reset = new QPushButton();
    reset->setText("Reset");

    oneStepBack = new QPushButton();
    oneStepBack->setText("<");
//    oneStepBack->setAccessibleDescription("make one step backward");


    oneStep = new QPushButton();
    oneStep->setText(">");
//    oneStep->setAccessibleDescription("make one step forward");

    run = new QPushButton("Run");

    menu->addWidget(reset);
    menu->addWidget(oneStepBack);
    menu->addWidget(oneStep);
    menu->addWidget(run);

    //Definition du widget stat


    Step = new QLabel(QString("Step : 0"));
    currentState = new QLabel(QString("currentState : 1"));
    cursorPosition = new QLabel(QString("cursorPosition : (0;0)"));
    nbrOfAnts = new QLabel(QString("Number Of Ants : 0"));

    statContainer->addWidget(Step);
    statContainer->addWidget(currentState);
    statContainer2->addWidget(cursorPosition);
    statContainer2->addWidget(nbrOfAnts);


    //Ajout du tableau de stat au menu
    menu->addLayout(statContainer);
    menu->addLayout(statContainer2);

    mainLayout->addLayout(menu);
    setLayout(mainLayout);

   // move(pos);
    setMaximumSize(size);


}

void Gui::updateStep(int scr){
    QString tmpS("Step : ");
    tmpS += QString::number(scr);
    Step->setText(tmpS);
}

void Gui::updateCurrentState(int scr){
    QString tmpS("currentState : ");
    tmpS += QString::number(scr);
    currentState->setText(tmpS);
}

void Gui::updateCursorPos(int x, int y){
    QString tmpS("Position : (");
    tmpS += QString::number(x) + QString(" ; ") + QString::number(y)+QString(")");
    cursorPosition->setText(tmpS);
}

void Gui::updateNbrOfAnts(int scr){
    QString tmpS("Number Of Ants : ");
    tmpS += QString::number(scr);
    nbrOfAnts->setText(tmpS);
}
