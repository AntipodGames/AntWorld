#include "displaymanager.h"

DisplayManager::DisplayManager(int WIDTH,int HEIGHT,int nbrStates, int w, int h){



    colors.append(sf::Color::White);
    colors.append(sf::Color::Red);
    colors.append(sf::Color::Yellow);
    colors.append(sf::Color::Green);
    colors.append(sf::Color(0,0,255));

    rand.seed(42);

    boost::random::uniform_int_distribution<> rd(0,255);
    int r = rd(rand);

    int g = rd(rand);
    int b = 0;//rd(rand);

    for(int i = 5; i < nbrStates; i++){
        colors.append(sf::Color(r,g,b));
        r = rd(rand);
        g = rd(rand);
        b = 0; //rd(rand);

    }

    for(int x = 0; x <WIDTH ;x+=WIDTH/w){
        for(int y = 0; y < HEIGHT; y+=HEIGHT/h){
            sf::RectangleShape rect(sf::Vector2f(WIDTH/w,HEIGHT/h)); //=  sf::Shape::Rectangle(x,y,x+WIDTH/w,y+HEIGHT/h,colors[0]);
            rect.setFillColor(colors[0]);
            rect.setPosition(x,y);
            cells.insert(QPair<int,int>(x/(WIDTH/h),y/(HEIGHT/w)),rect);
        }
    }


}

void DisplayManager::updateCells(QMap<QPair<int, int>, Automaton>& automats){
    for(auto i = automats.begin(); i != automats.end();i++){
        Automaton a(i.value());
        sf::RectangleShape shp = cells.value(i.key());
        sf::Color color = colors[a.getState()];

        if(a.getState() == PHERO_SEARCH){
            if(a.getPheroScore() >=255)
                color = sf::Color(0,0, 255);
            else
                color = sf::Color(0,0, a.getPheroScore());
        }
        shp.setFillColor(color);
        cells.insert(i.key(),shp);
    }
}

void DisplayManager::draw(sf::RenderWindow & win){
    for(auto i = cells.begin(); i != cells.end(); i++){
        sf::RectangleShape shp = i.value();
        if(shp.getFillColor() != sf::Color::White)
            win.draw(shp);
    }
}

void DisplayManager::setColor(int i ,int r,int g, int b){
    colors.insert(i,sf::Color(r,g,b));
}
