#ifndef AUTOMATON_H
#define AUTOMATON_H
#include <QVector>
#include <QPair>

class Automaton
{
public:
    Automaton(){}
    Automaton(QVector<QPair<int,int> > N,int S);
    Automaton(const Automaton& a)
        : neighborhood(a.neighborhood)
        , state(a.state)
        , pheroScore(a.pheroScore)
        , antLife(a.antLife)
        , direction(a.direction)
    {}

    QVector<QPair<int,int> >& getNeighbors();

    int getState();
    void setState(int);

    void incrementPheroScore(int n);
    void decrementPheroScore(int n);
    int getPheroScore();

    void setAntLife(int n);
    void decrementAntLife(int n);
    int  getAntLife();

    int getDirection(){return direction;}
    void setDirection(int dir){direction=dir;}


private:
    QVector<QPair<int,int> > neighborhood;
    int state;
    int pheroScore = 0;
    int antLife = 0;
    int direction = 0;


};

#endif // AUTOMATON_H
