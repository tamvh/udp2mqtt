TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

HEADERS += \
    zudp.h \
    zmqtt.h

SOURCES += main.cpp \
    zudp.cpp \
    zmqtt.cpp

INCLUDEPATH += /usr/local/include
LIBS += -L"/usr/local/lib" -lpaho-mqtt3a  -lpaho-mqtt3as -lpaho-mqtt3c -lpaho-mqtt3cs
LIBS += -pthread -lpthread



