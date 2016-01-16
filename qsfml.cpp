#include "qsfml.h"

Qsfml::Qsfml(QWidget* parent,const QPoint& Position, const QSize& Size
             , unsigned int FrameTime) :
    QWidget(parent),
    myInitialized (false)
{
    // Mise en place de quelques options pour autoriser le rendu direct dans le widget
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_NoSystemBackground);

    // Changement de la police de focus, pour autoriser notre widget à capter les évènements clavier
    setFocusPolicy(Qt::StrongFocus);

    // Définition de la position et de la taille du widget
    move(Position);
    setMaximumSize(Size);
    setMinimumSize(Size);

    // Préparation du timer
    myTimer.setInterval(FrameTime);

}

Qsfml::~Qsfml(){}

void Qsfml::showEvent(QShowEvent*){
    if (!myInitialized)
    {
        // Sous X11, il faut valider les commandes qui ont été envoyées au serveur
        // afin de s'assurer que SFML aura une vision à jour de la fenêtre
        #ifdef Q_WS_X11
           XFlush(QX11Info::display());
        #endif

        // On crée la fenêtre SFML avec l'identificateur du widget
        sf::Window::create(winId());

        // On laisse la classe dérivée s'initialiser si besoin
        OnInit();

        // On paramètre le timer de sorte qu'il génère un rafraîchissement à la fréquence souhaitée
        connect(&myTimer, SIGNAL(timeout()), this, SLOT(repaint()));
        myTimer.start();

        myInitialized = true;
    }

}



QPaintEngine* Qsfml::paintEngine() const
{
    return 0;
}

void Qsfml::paintEvent(QPaintEvent*)
{
    // On laisse la classe dérivée faire sa tambouille
    OnUpdate();


    // On rafraîchit le widget
    display();
}

void Qsfml::OnInit(){}
void Qsfml::OnUpdate(){}
