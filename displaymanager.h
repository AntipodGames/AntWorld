#ifndef DISPLAY_H
#define DISPLAY_H
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <QVector>
#include "world.h"


class DisplayManager
{
public:
    DisplayManager(){}

    DisplayManager(int,int,int nbrStates,int w, int h);

    void updateCells(QMap<QPair<int, int>, Automaton> &);
    void draw(sf::RenderWindow&);
    void setColor(int i, int r,int g, int b);

private:
    boost::random::mt19937 rand;

    QVector<sf::Color> colors;
    QMap<QPair<int,int>,sf::RectangleShape> cells;

};

#endif // DISPLAY_H
