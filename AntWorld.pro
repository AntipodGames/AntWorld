LIBS += -L/usr/local/lib -lX11 -lsfml-window -lsfml-system -lsfml-graphics -lsfml-audio

INCLUDEPATH += /home/leni/SFML-2.1/include
INCLUDEPATH += /usr/include/boost

CONFIG += c++11
QT += widgets
QT += core
QT += gui

SOURCES += \
    world.cpp \
    qsfml.cpp \
    qsfcanvas.cpp \
    maingui.cpp \
    Main.cpp \
    gui.cpp \
    displaymanager.cpp \
    automaton.cpp

HEADERS += \
    world.h \
    qsfml.h \
    qsfcanvas.h \
    maingui.h \
    gui.h \
    displaymanager.h \
    automaton.h
