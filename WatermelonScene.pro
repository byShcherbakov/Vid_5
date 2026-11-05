QT += widgets
CONFIG += c++17
TEMPLATE = app
TARGET = WatermelonScene
SOURCES += main.cpp graphicsitems.cpp watermelonscene.cpp mainwindow.cpp
HEADERS += graphicsitems.h watermelonscene.h mainwindow.h
msvc:QMAKE_CXXFLAGS += /utf-8
