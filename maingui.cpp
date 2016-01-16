#include "maingui.h"

MainGUI::MainGUI(int width,int height)
{

    gui = new Gui(QPoint(width,0),QSize(height,50));

    SFMLView = new QSFcanvas(QPoint(0,0),QSize(width,height-50));

    container = new QVBoxLayout(this);

    container->addWidget(SFMLView);
    container->addWidget(gui);
    container->setMargin(0);


    connect(SFMLView,SIGNAL(sendCurrentState(int)),gui,SLOT(updateCurrentState(int)));
    connect(SFMLView,SIGNAL(sendStep(int)),gui,SLOT(updateStep(int)));
    connect(SFMLView,SIGNAL(sendCursorPos(int,int)),gui,SLOT(updateCursorPos(int,int)));
    connect(SFMLView,SIGNAL(sendNbrOfAnts(int)),gui,SLOT(updateNbrOfAnts(int)));
    connect(gui->reset,SIGNAL(released()),SFMLView,SLOT(reset()));
    connect(gui->oneStepBack,SIGNAL(released()),SFMLView,SLOT(oneStepBack()));
    connect(gui->oneStep,SIGNAL(released()),SFMLView,SLOT(stepbystep()));
    connect(gui->run,SIGNAL(released()),SFMLView,SLOT(run()));
    connect(SFMLView,SIGNAL(runChange()),this,SLOT(runpause()));
}

void MainGUI::init(){
    SFMLView->show();

}

void MainGUI::runpause(){
    if(QString::compare(gui->run->text(),"Run") == 0)
        gui->run->setText("Pause");
    else gui->run->setText("Run");
}
