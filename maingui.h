#ifndef MAINGUI_H
#define MAINGUI_H
#include <QWidget>
#include <QVBoxLayout>
#include <qsfcanvas.h>
#include <gui.h>

class MainGUI : public QWidget
{
    Q_OBJECT
public:
    MainGUI(){}
    MainGUI(int, int );

    void init();

public slots:
    void runpause();

private:
    QVBoxLayout* container;
    QSFcanvas* SFMLView;
    Gui* gui;



};

#endif // MAINGUI_H
