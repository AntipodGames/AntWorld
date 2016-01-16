#ifndef QSFML_H
#define QSFML_H
#include <SFML/Graphics.hpp>
#include <QWidget>
#include <qtimer.h>
#include <iostream>

#define TIMER 100


#ifdef Q_WS_X11
    #include <Qt/qx11info_x11.h>
    #include <X11/Xlib.h>
#endif //Q_WS_X11

class Qsfml : public QWidget, public sf::RenderWindow
{
    Q_OBJECT

public:
    Qsfml(QWidget* parent,const QPoint& Position, const QSize& Size
          , unsigned int FrameTime = TIMER);
    virtual ~Qsfml();




protected:
    virtual void OnInit();

    virtual void OnUpdate();

    virtual QPaintEngine* paintEngine() const;
    virtual void showEvent(QShowEvent*);


    virtual void paintEvent(QPaintEvent*);

    QTimer myTimer;
    bool   myInitialized;
};

#endif // QSFML_H
