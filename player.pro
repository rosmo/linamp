TEMPLATE = app
TARGET = player

QT += network \
      multimedia \
      multimediawidgets \
      widgets

LIBS += -ltag

HEADERS = \
    controlbuttonswidget.h \
    desktopbasewindow.h \
    desktopplayerwindow.h \
    fft.h \
    mainwindow.h \
    mediaplayer.h \
    playerview.h \
    playlistmodel.h \
    playlistview.h \
    scrolltext.h \
    qmediaplaylist.h \
    qmediaplaylist_p.h \
    qplaylistfileparser.h \
    spectrumwidget.h \
    titlebar.h

SOURCES = main.cpp \
    controlbuttonswidget.cpp \
    desktopbasewindow.cpp \
    desktopplayerwindow.cpp \
    fft.cpp \
    mainwindow.cpp \
    mediaplayer.cpp \
    playerview.cpp \
    playlistmodel.cpp \
    playlistview.cpp \
    scrolltext.cpp \
    qmediaplaylist.cpp \
    qmediaplaylist_p.cpp \
    qplaylistfileparser.cpp \
    spectrumwidget.cpp \
    titlebar.cpp

target.path = $$[QT_INSTALL_EXAMPLES]/multimedia/player
INSTALLS += target

FORMS += \
    controlbuttonswidget.ui \
    desktopbasewindow.ui \
    desktopplayerwindow.ui \
    playerview.ui \
    playlistview.ui \
    titlebar.ui

RESOURCES += \
    uiassets.qrc

DISTFILES += \
    README.md \
    scale-skin.sh
