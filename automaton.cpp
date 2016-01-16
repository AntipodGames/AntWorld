#include "automaton.h"


Automaton::Automaton(QVector<QPair<int, int> > N, int S){
    neighborhood = N;
    state = S;
}


QVector<QPair<int,int> >& Automaton::getNeighbors(){
    return neighborhood;
}


int Automaton::getState(){
    return state;
}

void Automaton::setState(int s){
    state = s;
//    pheroScore = 0;
}

void Automaton::incrementPheroScore(int n){
    pheroScore+=n;
}

void Automaton::decrementPheroScore(int n){
    pheroScore-=n;
    if(pheroScore <= 0)
        pheroScore = 0;
}
int Automaton::getPheroScore(){
    return pheroScore;
}


void Automaton::setAntLife(int n){
    antLife = n ;
}

void Automaton::decrementAntLife(int n){
    antLife -= n;
    if(antLife <= 0)
        antLife = 0;
}

int Automaton::getAntLife(){
    return antLife;
}
