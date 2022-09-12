QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
RC_ICONS=QMidiPlayer.ico
# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
SOURCES += \
    QMidiFile.cpp \
    QMidiIn.cpp \
    QMidiOut.cpp \
    QMidi_Win32.cpp \
    actions.cpp \
    analyzer.cpp \
    gotownd.cpp \
    main.cpp \
    mainwindow.cpp \
    mididecoder.cpp \
    preferences.cpp \
    preferencewnd.cpp \
    rendercorebasic.cpp \
    rendercoreblue.cpp \
    sdlrender.cpp

HEADERS += \
    QMidi.h \
    QMidiFile.h \
    QMidiIn.h \
    QMidiOut.h \
    analyzer.h \
    gotownd.h \
    irendercore.h \
    irendercorefactory.h \
    mainwindow.h \
    mididecoder.h \
    preferences.h \
    preferencewnd.h \
    rendercorebasic.h \
    rendercoreblue.h \
    sdlrender.h

FORMS += \
    analyzer.ui \
    gotownd.ui \
    mainwindow.ui \
    preferencewnd.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resource.qrc



win32: LIBS += -L$$PWD/lib/ -lSDL2 -lwinmm

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include

QMAKE_CXXFLAGS_RELEASE += -O2
