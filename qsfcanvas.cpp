#include "qsfcanvas.h"


QSFcanvas::QSFcanvas(const QPoint& Position, const QSize& Size, QWidget* parent) : Qsfml(parent, Position, Size)
{
    step =0;
    setMouseTracking(true);
}







void QSFcanvas::keyReleaseEvent(QKeyEvent * e){
    if(e->key() == Qt::Key_Space){
        run();
    }
    else if(e->key() == Qt::Key_Right){
        stepbystep();
    }
    else if(e->key() == Qt::Key_Left)
        oneStepBack();
    else if(e->key() == Qt::Key_Up){
        currentState++;

        currentState%=nbrStates;
        emit sendCurrentState(currentState);

    }
}

void QSFcanvas::mouseMoveEvent(QMouseEvent * event){
    emit sendCursorPos(event->pos().x()/(getSize().x/sizeX),event->pos().y()/(getSize().y/sizeY));
    if(leftClickPressed){
        QPair<int,int> pair(event->pos().x()/(getSize().x/sizeX),event->pos().y()/(getSize().y/sizeY));
        Automaton automa(world.getAutomata().value(pair));
        automa.setState(currentState);
        world.getAutomata().insert(pair,automa);
    }
}

void QSFcanvas::mousePressEvent(QMouseEvent * event){
    if(event->button() == Qt::LeftButton)
        leftClickPressed = true;
}

void QSFcanvas::mouseReleaseEvent(QMouseEvent * event){
    if(event->button() == Qt::LeftButton){
        QPair<int,int> pair(event->pos().x()/(getSize().x/sizeX),event->pos().y()/(getSize().y/sizeY));
        Automaton automa(world.getAutomata().value(pair));
        automa.setState(currentState);
        world.getAutomata().insert(pair,automa);
        leftClickPressed = false;

    }
}
void QSFcanvas::wheelEvent(QWheelEvent * event){
    if(event->delta() > 0)
        currentState++;


    currentState%=nbrStates;

    emit sendCurrentState(currentState);
//    std::cout << "etat courant :" << currentState << std::endl;

}

void QSFcanvas::OnInit(){

    QVector<QPair<int,int> > N;
    N.append(QPair<int,int>(-1,0));
    N.append(QPair<int,int>(1,0));
    N.append(QPair<int,int>(0,-1));
    N.append(QPair<int,int>(0,1));
    //    N.append(QPair<int,int>(-1,1));
    //    N.append(QPair<int,int>(1,1));
    //    N.append(QPair<int,int>(1,-1));
    //    N.append(QPair<int,int>(-1,-1));

    int** initS = new int*[sizeX];


    for(int i = 0; i <sizeX; i++){
        initS[i] = new int[sizeY];
        for(int j = 0; j < sizeY; j++){
            initS[i][j] = 0;
        }
    }

    //    initS[50][50] = 1;

    world = World(sizeX,sizeY,N,initS);

    display = DisplayManager(getSize().x,getSize().y,nbrStates,sizeX,sizeY);
    // display.updateCells(world.getAutomata());


}



void QSFcanvas::reset(){
    step = 0;
    world.reset();
    emit sendStep(step);
}

void QSFcanvas::stepbystep(){
    step++;
    int noa =  world.nextStep();
    emit sendNbrOfAnts(noa);

    emit sendStep(step);
}

void QSFcanvas::oneStepBack(){
    if(world.lastStep())
        step--;
    emit sendStep(step);
}

void QSFcanvas::run(){
    keepGoing = !keepGoing;
    emit runChange();
}

void QSFcanvas::pause(){
    if(myTimer.isActive())
        myTimer.stop();
    else myTimer.start();
}


void QSFcanvas::OnUpdate()
{
    //display.updateCells(world.getAutomata());
    //  std::cout << step << std::endl;
    display.updateCells(world.getAutomata());

    clear();
    display.draw(*this);
    if(keepGoing){
        stepbystep();
    }


}
