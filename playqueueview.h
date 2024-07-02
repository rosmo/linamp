#ifndef PLAYQUEUEVIEW_H
#define PLAYQUEUEVIEW_H

#include "queuecontrolbuttonswidget.h"
#include "mediaplayer.h"
#include "qmediaplaylist.h"
#include "playlistmodel.h"

#include <QMediaMetaData>
#include <QAudioOutput>
#include <QWidget>
#include <QTimer>
#include <QScroller>

QT_BEGIN_NAMESPACE
class QAbstractItemView;
class QLabel;
class QModelIndex;
class QPushButton;
class QComboBox;
class QSlider;
class QStatusBar;
QT_END_NAMESPACE

namespace Ui {
class PlayQueueView;
}

class PlayQueueView : public QWidget
{
    Q_OBJECT

public:
    explicit PlayQueueView(QWidget *parent = nullptr, PlaylistModel *playlistModel = nullptr, QueueControlButtonsWidget *ctlBtns = nullptr);
    ~PlayQueueView();

    void setSourceLabel(QString label);

public slots:
    void playlistPositionChanged(int);
    void playlistCleared();
    void removeItem();
    void handleSongSelected(const QModelIndex &index);
    void handleSelectionChanged(int index);
    void updateScrollBar(int start, int end);
    void scrollBarMoved(int value);
    void queueListScrolling(int value);

    void addFileClicked();
    void addUrlClicked();
    void addDirectoryClicked();

    void removeMiscClicked();
    void removeAllClicked();
    void cropClicked();
    void removeSelectedClicked();

    void selectAllClicked();
    void selectZeroClicked();
    void invertSelectClicked();

    void miscOptionsClicked();
    void fileInformationClicked();
    void sortListClicked();

    void newListClicked();
    void saveListClicked();
    void loadListClicked();

signals:
    void showPlaylistRequested();
    void clearPlaylist();
    void songSelected(const QModelIndex &index);

private:
    Ui::PlayQueueView *ui;
    QMediaPlaylist *m_playlist = nullptr;
    PlaylistModel *m_playlistModel = nullptr;
    QScroller *m_playlistViewScroller = nullptr;
    QueueControlButtonsWidget *controlButtons = nullptr;

    void scale();
    void setupQueueListUi();

    int numberOfItems = 5;
    int scrollBarRange = 0;
};

#endif // PLAYERQUEUEVIEW_H
