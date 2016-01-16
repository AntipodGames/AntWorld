#include "world.h"

World::World(QPair<int, int> s,QVector<QPair<int,int> > N,int** initStates){
    size = s;

    random.seed(time(NULL));

    // random.SetSeed(rand());
    for(int x = 0; x < size.second;x++){
        for(int y = 0; y < size.first; y++){
            automata.insert(QPair<int,int>(x,y),Automaton(N,initStates[x][y]));
        }
    }

}

World::World(int w, int h, QVector<QPair<int, int> > N,int** initStates){
    random.seed(time(NULL));

    size = QPair<int,int>(w,h);
    for(int x = 0; x < size.second;x++){
        for(int y = 0; y < size.first; y++){
            automata.insert(QPair<int,int>(x,y),Automaton(N,initStates[x][y]));
        }
    }
}

int World::nextStep(){

    if(relativeStep == 0){
        QMap<QPair<int,int>,Automaton> newAutomata/*(automata)*/;

        nbrofAnt = 0;
        nbrofDead = 0;
        nbrofPhero = 0;
        for(auto a = automata.begin(); a != automata.end();a++){
            Automaton tmpA = a.value();
            if(!newAutomata.contains(a.key())){

                if(tmpA.getState() != 0){
                    transitionFunction(newAutomata,a.key());
                    //newAutomata.insert(a.key(),at);
                }
                else newAutomata.insert(a.key(),a.value());



            }
            if(tmpA.getState() == ANT_RETURN
                    || tmpA.getState() == ANT_SEARCH
                    || tmpA.getState() == ANT_SEARCH2
                    || tmpA.getState() == ANT_SEARCH3){
                nbrofAnt++;
            }

        }
        //    std::cout << nbrofAnt << std::endl;



        automata = newAutomata;

        pastStates.prepend(automata);
        if(pastStates.size() > nbrOfSafedStates){
            pastStates.pop_back();
        }
    }else{
        relativeStep--;
        if(relativeStep <= 0)
            relativeStep = 0;

        automata = pastStates[relativeStep];


    }

    return nbrofAnt;

}

bool World::lastStep(){
    relativeStep++;

    if(relativeStep >= pastStates.size() -1){
        relativeStep = pastStates.size()-1;
        return false;
    }

    automata = pastStates[relativeStep];
    return true;
}

void World::reset(){
    for(auto i = automata.begin(); i != automata.end();i++){
        Automaton tmp(i.value());
        tmp.setState(0);
        automata.insert(i.key(),tmp);
    }
    pastStates.clear();
    relativeStep = 0;
}

Automaton World::getAutomaton(int x, int y){
    QPair<int,int> coord(x,y);

    return automata.value(coord);
}

QMap<QPair<int,int>,Automaton>& World::getAutomata(){
    return automata;
}

int World::getNbrOfAnts(){
    return nbrofAnt;
}

QPair<int,int> World::neighboorCoord(Automaton current, int i,QPair<int,int> coord){
    int x = current.getNeighbors()[i].first+coord.first;
    int y = current.getNeighbors()[i].second+coord.second;
    if(x > size.first-1)
        x = 0;
    else if(x < 0)
        x = size.first-1;

    if(y> size.second-1)
        y = 0;
    else if(y < 0)
        y = size.second-1;
    QPair<int,int> nCoord(x,y);
    return nCoord;
}

void World::transitionFunction(QMap<QPair<int, int>, Automaton> & newAuto, QPair<int,int> coord){

    Automaton current(automata.value(coord));
    QPair<int,int> nCoord = coord;
    QVector<int> deadState;
    QVector<int> pheroState;
    //    QVector<int> pheroReturn;
    int baseState = -1;
    int foodState = -1;
    int pheroSearch = -1;

    if(current.getState() == ANT_SEARCH){
        float r;
        QVector<float> weights;
        QVector<int> neighboors;//free neighboors, list of cells who the ant can go.
        QVector<int> ants;
        float currentWeight = 0;
        int pheroSearch2 = -1;

        for(int i = 0; i < current.getNeighbors().size();i++){
            nCoord = neighboorCoord(current,i,coord);
            Automaton tmp(automata.value(nCoord));

            if(tmp.getState() == 0){ //|| tmp.getState() == PHERO_RETURN)
                currentWeight+=1.;
                weights.append(currentWeight);
                neighboors.append(i);
            }
            else if(tmp.getState() == PHERO_SEARCH ){
                currentWeight+=1.+((float)tmp.getPheroScore()/PHERO_NORM);
                weights.append(currentWeight);
                neighboors.append(i);
            }
            else if(tmp.getState() == FOOD)
                foodState = i;
            else if(tmp.getState() == PHERO_SEARCH2)
                pheroSearch2 = i;
            else if(tmp.getState() == BASE_EMPTY)
                baseState = i;
            else if(tmp.getState() == ANT_SEARCH || tmp.getState() == ANT_RETURN){
                ants.append(i);
            }
        }

        if(ants.size() > 0 && pheroSearch2 != -1 && foodState == -1 && current.getAntLife() > 0){
            int i=0;
            //            for(i = 0; i < ants.size(); i++){
            //                Automaton tmp(automata.value(neighboorCoord(current,ants[i],coord)));
            //                if(current.getDirection() != tmp.getDirection())
            //                    break;
            //            }
            //            if(i != ants.size()){

            nCoord = neighboorCoord(current,ants[i],coord);
            Automaton tmp(automata.value(nCoord));
            if(tmp.getState() == ANT_SEARCH){
                Automaton newAnt(automata.value(neighboorCoord(current,pheroSearch2,coord)));
                newAnt.setState(ANT_SEARCH);
                newAnt.setAntLife(current.getAntLife());
                newAnt.setDirection(current.getDirection());
                newAnt.decrementAntLife(1);
                newAuto.insert(neighboorCoord(current,pheroSearch2,coord),newAnt);
            }
            else if(tmp.getState() == ANT_RETURN){
                Automaton newAnt(automata.value(neighboorCoord(current,pheroSearch2,coord)));
                newAnt.setState(ANT_RETURN);
                newAnt.setDirection(current.getDirection());
                newAuto.insert(neighboorCoord(current,pheroSearch2,coord),newAnt);
            }
            current.setState(PHERO_SEARCH2);
            //            }
        }
        else if(neighboors.size() > 0 && foodState == -1 && current.getAntLife() > 0){
            boost::random::uniform_real_distribution<> rd(0,currentWeight);
            r = rd(random);

            int i;
            for(i = 0; i < weights.size();i++){
                if(r<weights[i])
                    break;
            }
            current.setDirection(neighboors[i]);
            nCoord = neighboorCoord(current,neighboors[i],coord);

            Automaton tmp(newAuto.value(nCoord));
            if(!newAuto.contains(nCoord) || (tmp.getState() != ANT_RETURN
                                             && tmp.getState() != ANT_SEARCH)){
                //                                             && tmp.getState() != ANT_SEARCH2)){
                Automaton newAnt(automata.value(nCoord));
                newAnt.setState(ANT_SEARCH);
                newAnt.setAntLife(current.getAntLife());
                newAnt.setDirection(current.getDirection());
                newAnt.decrementAntLife(1);
                newAuto.insert(nCoord,newAnt);
                current.setState(PHERO_SEARCH2);
            }
        }else if(current.getAntLife() <= 0){
            current.setState(0);
        }else if(foodState != -1){
            //            nCoord = neighboorCoord(current,foodState,coord);
            //            Automaton newA(automata.value(nCoord));
            //            newA.setState(0);
            //            newAuto.insert(nCoord,newA);
            current.setState(ANT_RETURN);
        }

        //        else if(baseState != -1){
        //            current.setState(PHERO_SEARCH);
        //        }
        else{
            current.setState(ANT_SEARCH);
        }
    }
    else if(current.getState() == ANT_RETURN){

        int x;
        int y;

        float r;
        QVector<float> weights;
        QVector<int> neighboors;
        float currentWeight = 0;
        int pheroSearch2 = -1;
        QVector<int> ants;
        for(int i = 0; i < current.getNeighbors().size();i++){

            nCoord = neighboorCoord(current,i,coord);
            Automaton tmp(automata.value(nCoord));


            if(tmp.getState() == 0){
                currentWeight+=1.;
                weights.append(currentWeight);
                neighboors.append(i);
            }
            else if(tmp.getState() == PHERO_SEARCH ){
                currentWeight+=1.+((float)tmp.getPheroScore()*(float)tmp.getPheroScore())/(float)PHERO_NORM;

                weights.append(currentWeight);
                neighboors.append(i);
            }
            else if(tmp.getState() == PHERO_SEARCH2)
                pheroSearch2 = i;
            else if(tmp.getState() == BASE_EMPTY)
                baseState = i;
            else if(tmp.getState() == ANT_RETURN || tmp.getState() == ANT_SEARCH){
                ants.append(i);
            }

        }

        if(ants.size() > 0 && pheroSearch2 != -1 && baseState == -1){
            int i = 0;
            //            for(i = 0; i < ants.size(); i++){
            //                Automaton tmp(automata.value(neighboorCoord(current,ants[i],coord)));
            //                if(current.getDirection() != tmp.getDirection())
            //                    break;
            //            }
            //            if(i != ants.size()){
            nCoord = neighboorCoord(current,ants[i],coord);
            Automaton tmp(automata.value(nCoord));
            if(tmp.getState() == ANT_SEARCH){
                Automaton newAnt(automata.value(neighboorCoord(current,pheroSearch2,coord)));
                newAnt.setState(ANT_SEARCH);
                newAnt.setAntLife(current.getAntLife());
                newAnt.setDirection(current.getDirection());
                newAnt.decrementAntLife(1);
                newAuto.insert(neighboorCoord(current,pheroSearch2,coord),newAnt);
            }
            else if(tmp.getState() == ANT_RETURN){
                Automaton newAnt(automata.value(neighboorCoord(current,pheroSearch2,coord)));
                newAnt.setState(ANT_RETURN);
                newAnt.setDirection(current.getDirection());
                newAuto.insert(neighboorCoord(current,pheroSearch2,coord),newAnt);
            }
            current.setState(PHERO_SEARCH2);
            //            }
        }
        else if(baseState == -1 ){
            if(neighboors.size() > 0){
                boost::random::uniform_real_distribution<> rd(0,weights.last());
                r = rd(random);

                int i;
                for(i = 0; i < weights.size();i++){
                    if(r<weights[i])
                        break;
                }
                current.setDirection(neighboors[i]);
                x = current.getNeighbors()[neighboors[i]].first+coord.first;
                y = current.getNeighbors()[neighboors[i]].second+coord.second;
            }

            else{
                x = coord.first;
                y = coord.second;
            }

            if(x > size.first-1)
                x = 0;
            else if(x < 0)
                x = size.first-1;

            if(y> size.second-1)
                y = 0;
            else if(y < 0)
                y = size.second-1;
            nCoord.first = x;
            nCoord.second = y;
            Automaton tmp(newAuto.value(nCoord));

            if(!newAuto.contains(nCoord) || (tmp.getState() != ANT_RETURN
                                             && tmp.getState() != ANT_SEARCH
                                             //                                             && tmp.getState() != ANT_SEARCH2
                                             )){
                Automaton newAnt(automata.value(nCoord));
                newAnt.setState(ANT_RETURN);
                newAnt.setDirection(current.getDirection());
                newAuto.insert(nCoord,newAnt);
                current.setState(PHERO_SEARCH2);

            }
        }
        else{

            //            nCoord = neighboorCoord(current,baseState,coord);
            //            Automaton newAnt(automata.value(nCoord));

            //            newAnt.setState(BASE_EXPEND);
            //            newAuto.insert(nCoord,newAnt);
            current.setState(0);
        }

    }
    else if(current.getState() == BASE){
        QVector<int> outbase;

        for(int i =0;i<current.getNeighbors().size();i++){

            nCoord = neighboorCoord(current,i,coord);
            Automaton newAnt(automata.value(nCoord));
            if(newAnt.getState() != IN_BASE &&  newAnt.getState() != BASE_EMPTY){
                outbase.append(i);
            }
        }
        if(outbase.size() >0){
            boost::random::uniform_int_distribution<> rd(0,outbase.size()-1);
            int r = rd(random);

            nCoord = neighboorCoord(current,outbase[r],coord);
            Automaton newAnt(automata.value(nCoord));
            newAnt.setState(ANT_SEARCH);
            newAnt.setAntLife(50);
            newAuto.insert(nCoord,newAnt);
            current.setState(BASE_EMPTY);
        }

    }
    else if(current.getState() == BASE_EXPEND){
        for(int i =0;i<current.getNeighbors().size();i++){
            nCoord = neighboorCoord(current,i,coord);
            Automaton newBase(automata.value(nCoord));
            if(newBase.getState() != IN_BASE){
                newBase.setState(BASE);
                newAuto.insert(nCoord,newBase);
            }
        }
        current.setState(IN_BASE);
    }
    else if(current.getState() == BASE_EMPTY){
        //        bool inbase = true;
        bool antR = false;
        for(int i =0;i<current.getNeighbors().size();i++){
            nCoord = neighboorCoord(current,i,coord);
            Automaton tmp(automata.value(nCoord));
            //            if(/*newBase.getState() != IN_BASE*/
            //                    newBase.getState() != BASE_EMPTY
            //                    && newBase.getState() != BASE
            //                    && newBase.getState() != BASE_EXPEND){
            //                inbase = false;
            //            }
            if(tmp.getState() == ANT_RETURN)
                antR = true;
        }

        if(antR)
            current.setState(BASE_EXPEND);
    }

    else if(current.getState() == PHERO_SEARCH){
        current.decrementPheroScore(1);
        if(current.getPheroScore() <= 0)
            current.setState(0);
    }
    else if(current.getState() == PHERO_SEARCH2){
        current.setState(PHERO_SEARCH);
        current.incrementPheroScore(PHERO_NORM);

        //        for(int i = 0; i < current.getNeighbors().size();i++){

        //            nCoord = neighboorCoord(current,i,coord);
        //            Automaton tmp(automata.value(nCoord));


        ////            if(tmp.getState() == ANT_SEARCH2 ){
        ////                current.setState(PHERO_SEARCH);
        ////                current.incrementPheroScore(PHERO_NORM);
        ////                break;
        ////            }
        ////            else if (tmp.getState() == ANT_RETURN){
        ////                current.setState(PHERO_SEARCH);
        ////                break;

        ////            }

        //        }
    }
    else if(current.getState() == FOOD){
        bool antAround = false;

        for(int i = 0;i < current.getNeighbors().size();i++){
            nCoord = neighboorCoord(current,i,coord);
            Automaton tmp(automata.value(nCoord));
            if(tmp.getState() == ANT_SEARCH){
                current.setState(0);
                break;
            }
        }
    }


    //    else if(current.getState() == IN_BASE){
    //        bool outbase = true;
    //        for(int i =0;i<current.getNeighbors().size();i++){
    //            nCoord = neighboorCoord(current,i,coord);
    //            Automaton newBase(automata.value(nCoord));
    //            if(newBase.getState() != IN_BASE
    //                    && newBase.getState() != BASE_EMPTY
    //                    && newBase.getState() != BASE
    //                    && newBase.getState() != BASE_EXPEND){
    //                outbase = false;
    //            }
    //        }
    //        if(!outbase)
    //            current.setState(BASE_EMPTY);

    //    }

    //    else if(current.getState() == ANT_SEARCH2){

    //        for(int i = 0; i < current.getNeighbors().size();i++){

    //            nCoord = neighboorCoord(current,i,coord);
    //            Automaton tmp(automata.value(nCoord));


    //            if(tmp.getState() == 0) //|| tmp.getState() == PHERO_RETURN)
    //                deadState.append(i);
    //            else if(tmp.getState() == PHERO_SEARCH)
    //                pheroState.append(i);
    //            else if(tmp.getState() == PHERO_SEARCH2)
    //                pheroSearch = i;
    //            else if(tmp.getState() == BASE_EMPTY)
    //                baseState = i;
    //            else if(tmp.getState() == FOOD)
    //                foodState = i;
    //        }

    //        if(deadState.size() > 0 && foodState == -1 && pheroSearch != -1){

    //            if(pheroSearch == 0)
    //                nCoord = neighboorCoord(current,1,coord);
    //            else if(pheroSearch == 1)
    //                nCoord = neighboorCoord(current,0,coord);
    //            else if(pheroSearch == 2)
    //                nCoord = neighboorCoord(current,3,coord);
    //            else if(pheroSearch == 3)
    //                nCoord = neighboorCoord(current,2,coord);

    //            Automaton newAnt(automata.value(nCoord));
    //            Automaton tmp(newAuto.value(nCoord));

    //            if((newAnt.getState() == 0
    //                || newAnt.getState() == PHERO_SEARCH
    //                || newAnt.getState() == PHERO_SEARCH2)
    //                    && (!newAuto.contains(nCoord) ||
    //                        (tmp.getState() != ANT_RETURN
    //                         && tmp.getState() != ANT_SEARCH
    //                         && tmp.getState() != ANT_SEARCH2
    //                         && tmp.getState() != ANT_SEARCH3))){

    //                newAnt.setState(ANT_SEARCH);
    //                newAnt.setAntLife(current.getAntLife());
    //                newAuto.insert(nCoord,newAnt);

    //                current.setState(PHERO_SEARCH);
    //                current.incrementPheroScore(PHERO_NORM);


    //            }else{
    //                current.setState(ANT_SEARCH);
    //            }
    //        }else if(foodState != -1 ){
    //            nCoord = neighboorCoord(current,foodState,coord);

    //            Automaton newA(automata.value(nCoord));
    //            newA.setState(0);
    //            newAuto.insert(nCoord,newA);
    //            current.setState(ANT_RETURN);

    //        }else{
    //            current.setState(ANT_SEARCH);
    //        }
    //    }
    //    else if(current.getState() == ANT_SEARCH3){
    //        int r;
    //        if(deadState.size() == 0){
    //            if(pheroState.size()>0){
    //                boost::random::uniform_int_distribution<> rd(0,pheroState.size()-1);
    //                r = rd(random);

    //                nCoord = neighboorCoord(current,pheroState[r],coord);

    //                Automaton tmp(newAuto.value(nCoord));


    //                if(!newAuto.contains(nCoord) || (tmp.getState() != ANT_RETURN
    //                                                 && tmp.getState() != ANT_SEARCH
    //                                                 && tmp.getState() != ANT_SEARCH2
    //                                                 && tmp.getState() != ANT_SEARCH3)){
    //                    Automaton newAnt(automata.value(nCoord));
    //                    newAnt.setState(ANT_SEARCH3);

    //                    newAuto.insert(nCoord,newAnt);

    //                    current.setState(PHERO_SEARCH);
    //                    current.incrementPheroScore(PHERO_NORM/2);

    //                }  else{
    //                    current.setState(ANT_SEARCH);
    //                }
    //            }
    //        }else{
    //            current.setState(ANT_SEARCH);
    //        }
    //    }

    //else if(current.getState() == PHERO_RETURN){
    //    current.decrementPheroScore(1);
    //    if(current.getPheroScore() <=0)
    //        current.setState(0);
    //}

    newAuto.insert(coord,current);


    //    return newAuto;

}
