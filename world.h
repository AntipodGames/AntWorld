#ifndef WORLD_H
#define WORLD_H
#include <QMap>
#include "automaton.h"
#include <time.h>
#include <iostream>
#include "boost/random.hpp"


#define PHERO_NORM 50

//STATES
#define  ANT_SEARCH 1 //Ant will choice a direction randomly. Ant without food
#define  BASE 2 //base in produce mod
#define  FOOD 3
#define  PHERO_SEARCH 4 //Normal Pheromone
#define  ANT_SEARCH2 5 //Ant will follow the direction choose by ANT_SEARCH. Ant without food
#define  ANT_SEARCH3 6 //Deprecated
#define  ANT_RETURN 7 //Ant with food
#define  PHERO_SEARCH2 8 // Pheromone of ANT_SEARCH2 and ANT_RETURN.
//#define  PHERO_RETURN 8
#define  BASE_EMPTY 9 // Cell wait for a ant with food
#define  BASE_EXPEND 10 //Base in expand mod
#define  IN_BASE 11 //Cell in the base



class World
{

public:


    World(){}

    World(QPair<int,int>, QVector<QPair<int, int> > N,int**);
    World(int,int, QVector<QPair<int, int> > N,int**);

    int nextStep();
    bool lastStep();
    void reset();

    QPair<int,int> neighboorCoord(Automaton, int, QPair<int, int>);

    Automaton getAutomaton(int,int);
    QMap<QPair<int,int>,Automaton>& getAutomata();

    int getNbrOfAnts();

private:
    QPair<int,int> size;
    QMap<QPair<int,int>,Automaton> automata;
    QVector<QMap<QPair<int,int>,Automaton> > pastStates;
    int nbrOfSafedStates = 10;
    int relativeStep = 0;

    void transitionFunction(QMap<QPair<int,int>,Automaton>&, QPair<int, int> coord);
    boost::random::mt19937 random;
    int nbrofAnt=0;
    int nbrofDead = 0;
    int nbrofPhero = 0;


};

#endif // WORLD_H
