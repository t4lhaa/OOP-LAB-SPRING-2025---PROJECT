QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    board.cpp \
    clickablelabel.cpp \
    dice.cpp \
    gamepage.cpp \
    homepage.cpp \
    main.cpp \
    mainwindow.cpp \
    player.cpp \
    setuppage.cpp \
    winpop.cpp

HEADERS += \
    board.h \
    clickablelabel.h \
    dice.h \
    gamepage.h \
    homepage.h \
    mainwindow.h \
    player.h \
    setuppage.h \
    winpop.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

DISTFILES += \
    assets/sounds/forladder.wav \
    assets/sounds/forsnake.wav \
    assets/sounds/press.wav \
    assets/sounds/roll.wav \
    assets/sounds/win.wav \
    assets/sounds/yesis2.wav
