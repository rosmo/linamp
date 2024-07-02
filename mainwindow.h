#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedLayout>
#include <QProcess>
#include "audiosourcebluetooth.h"
#include "audiosourcecd.h"
#include "audiosourcecoordinator.h"
#include "audiosourcefile.h"
#include "controlbuttonswidget.h"
#include "mainmenuview.h"
#include "playerview.h"
#include "playlistview.h"
#include "qmediaplaylist.h"
#include "playlistmodel.h"
#include "playqueuewindow.h"
#include "playqueueview.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    PlaylistModel *playlistModel();
    void setPlayQueueWindow(PlayQueueWindow *playQueueWindow);

    QStackedLayout *viewStack;

    PlayerView *player;
    ControlButtonsWidget *controlButtons;
    PlaylistView *playlist;
    MainMenuView *menu;
    AudioSourceCoordinator *coordinator;
    AudioSourceFile *fileSource;
    AudioSourceBluetooth *btSource;
    AudioSourceCD *cdSource;

public slots:
    void showPlayer();
    void showPlaylist();
    void showMenu();
    void showShutdownModal();
    void open();

private:
    QMediaPlaylist *m_playlist = nullptr;
    PlaylistModel *m_playlistModel = nullptr;
    PlayQueueWindow *m_playQueueWindow = nullptr;
    QProcess *shutdownProcess = nullptr;
    void shutdown();

};

#endif // MAINWINDOW_H
