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
    void clearPlaylist();
    void removeItem();
    void handleSongSelected(const QModelIndex &index);
    void handleSelectionChanged(int index);

signals:
    void addClicked();
    void removeClicked();
    void selectClicked();
    void miscClicked();
    void optionsClicked();

private:
    Ui::PlayQueueView *ui;
    QMediaPlaylist *m_playlist = nullptr;
    PlaylistModel *m_playlistModel = nullptr;
    QScroller *m_playlistViewScroller = nullptr;
    QueueControlButtonsWidget *controlButtons = nullptr;

    void scale();
    void setupQueueListUi();
};

#endif // PLAYERQUEUEVIEW_H
