#ifndef PLAYQUEUEWINDOW_H
#define PLAYQUEUEWINDOW_H

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
#include "queuecontrolbuttonswidget.h"
#include "playqueueview.h"

class PlayQueueWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit PlayQueueWindow(QWidget *parent = nullptr, PlaylistModel *playlistModel = nullptr);
    ~PlayQueueWindow();

    QStackedLayout *viewStack;

    PlayQueueView *playQueue;
    QueueControlButtonsWidget *queueControlButtons;

public slots:
    void open();

private:
    PlaylistModel *m_playlistModel = nullptr;
};

#endif // PLAYQUEUEWINDOW_H
