// Copyright (C) 2017 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause
// Copyright (C) 2023 Rodrigo Mendez.

#include "mainwindow.h"
#include "playqueuewindow.h"
#include "scale.h"

#include <QApplication>
#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QDir>
#include <QUrl>
#include <QGuiApplication>
#include <QScreen>

#define APP_VERSION_STR "1.0.1"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QCoreApplication::setApplicationName("Linamp");
    QCoreApplication::setOrganizationName("Rod");
    QCoreApplication::setApplicationVersion(APP_VERSION_STR);
    QCommandLineParser parser;
    parser.setApplicationDescription("Linamp");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("url", "The URL(s) to open.");

    QCommandLineOption showQueueWindowOption(QStringList() << "q" << "queue-window", QCoreApplication::translate("main", "Show playlist queue window"));
    parser.addOption(showQueueWindowOption);

    QCommandLineOption queueWindowScreenOption(QStringList() << "s" << "queue-window-screen",
            QCoreApplication::translate("main", "Select screen for the queue window."),
            QCoreApplication::translate("main", "screen_number"));
    parser.addOption(queueWindowScreenOption);

    QCommandLineOption windowedModeOption(QStringList() << "w" << "windowed", QCoreApplication::translate("main", "Use windowed mode"));
    parser.addOption(windowedModeOption);

    parser.process(app);

    MainWindow window;
    if (!parser.positionalArguments().isEmpty()) {
        QList<QUrl> urls;
        for (auto &a : parser.positionalArguments())
            urls.append(QUrl::fromUserInput(a, QDir::currentPath()));
        //window.player->addToPlaylist(urls);
    }

    #ifdef IS_EMBEDDED
    if (!parser.isSet(windowedModeOption)) {
        window.setWindowState(Qt::WindowFullScreen);
    }
    #endif
    window.show();

    PlayQueueWindow playQueueWindow(nullptr, window.playlistModel());
    if (parser.isSet(showQueueWindowOption)) {    
        playQueueWindow.setWindowFlags(Qt::X11BypassWindowManagerHint | Qt::FramelessWindowHint);

        if (!parser.isSet(windowedModeOption)) {
            playQueueWindow.showFullScreen();
            
        } else {
            playQueueWindow.show();
        }

        if (parser.isSet(queueWindowScreenOption)) {
            QString queueWindowScreenStr = parser.value(queueWindowScreenOption);
            const int queueWindowScreen = queueWindowScreenStr.toInt();
            QList<QScreen*> screens = QGuiApplication::screens();
            QRect queueWindowScreenGeometry = screens[queueWindowScreen]->availableGeometry();
            playQueueWindow.setScreen(screens[queueWindowScreen]);
            playQueueWindow.move(queueWindowScreenGeometry.left(), queueWindowScreenGeometry.top());
        }
    }

    return app.exec();
}
