#-------------------------------------------------
#
# Project created by QtCreator 2013-10-30T09:37:04
#
#-------------------------------------------------

QT  += core gui network webkit webkitwidgets xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SWGEmuLaunchpad
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    settings.cpp \
    statusxmlcontenthandler.cpp \
    loginservers.cpp \
    addloginserver.cpp \
    configparser.cpp \
    gameprocess.cpp \
    selfupdater.cpp \
    installfromswg.cpp \
    utils.cpp \
    gamemods.cpp \
    macroeditor.cpp \
    filescanner.cpp

HEADERS  += mainwindow.h \
    settings.h \
    statusxmlcontenthandler.h \
    loginservers.h \
    loginserver.h \
    addloginserver.h \
    configparser.h \
    gameprocess.h \
    selfupdater.h \
    singleinstance.h \
    installfromswg.h \
    utils.h \
    gamemods.h \
    macroeditor.h \
    filescanner.h

FORMS    += mainwindow.ui \
    settings.ui \
    loginservers.ui \
    addloginserver.ui \
    gameprocess.ui \
    selfupdater.ui \
    installfromswg.ui \
    gamemods.ui \
    macroeditor.ui

OTHER_FILES += \
    logo_yellow.png \
    emu.rc \
    LICENSE.txt \
    swgemu.svg \
    search.svg \
    play.svg \
    logo-emu.svg \
    cogs.svg \
    required2.txt \
    required.txt \
    info.svg

RESOURCES += \
    rsources.qrc

win32 {
    SOURCES += windebugmonitor.cpp
    HEADERS += windebugmonitor.h
    CONFIG += embed_manifest_exe
    QMAKE_LFLAGS_WINDOWS += /MANIFESTUAC:level=\'requireAdministrator\'
}


RC_FILE = emu.rc
