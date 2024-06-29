#include "playqueueview.h"
#include "ui_playqueueview.h"

#include "scale.h"
#include "util.h"

#include <QApplication>
#include <QAudioDevice>
#include <QAudioOutput>
#include <QDir>
#include <QFileDialog>
#include <QMediaDevices>
#include <QMediaFormat>
#include <QMediaMetaData>
#include <QMessageBox>
#include <QStandardPaths>
#include <QFontDatabase>

PlayQueueView::PlayQueueView(QWidget *parent, PlaylistModel *playlistModel, QueueControlButtonsWidget *ctlBtns) :
    QWidget(parent),
    ui(new Ui::PlayQueueView)
{
    m_playlistModel = playlistModel;
    m_playlist = m_playlistModel->playlist();

    // Load custom fonts
    QFontDatabase::addApplicationFont(":/assets/bignumbers.ttf");

    // Setup UI
    ui->setupUi(this);
    scale();

    // Setup "queue list"
    setupQueueListUi();

    controlButtons = ctlBtns;

    connect(controlButtons, &QueueControlButtonsWidget::addClicked, this, &PlayQueueView::addClicked);
    connect(controlButtons, &QueueControlButtonsWidget::removeClicked, this, &PlayQueueView::removeClicked);
    connect(controlButtons, &QueueControlButtonsWidget::selectClicked, this, &PlayQueueView::selectClicked);
    connect(controlButtons, &QueueControlButtonsWidget::miscClicked, this, &PlayQueueView::miscClicked);
    connect(controlButtons, &QueueControlButtonsWidget::optionsClicked, this, &PlayQueueView::optionsClicked);

    // Connect playlist model to our QTreeView
    connect(m_playlistModel->playlist(), &QMediaPlaylist::currentIndexChanged, this, &PlayQueueView::playlistPositionChanged);
    connect(ui->queueList, &QAbstractItemView::clicked, this, &PlayQueueView::handleSongSelected);
    //connect(m_playlist, &QMediaPlaylist::mediaChanged, this, &PlayQueueView::updateTotalDuration);
    //connect(m_playlist, &QMediaPlaylist::mediaInserted, this, &PlayQueueView::updateTotalDuration);
    //connect(m_playlist, &QMediaPlaylist::mediaRemoved, this, &PlayQueueView::updateTotalDuration);
    connect(m_playlist, &QMediaPlaylist::currentSelectionChanged, this, &PlayQueueView::handleSelectionChanged);

    // connect(controlButtons, &ControlButtonsWidget::playClicked, this, &PlayerView::playClicked);

    // duration slider and label
    ui->scrollBar->setRange(0, 0);
    // connect(ui->posBar, &QSlider::sliderMoved, this, &PlayerView::positionChanged);

    // connect(ui->playlistButton, &QCheckBox::clicked, this, &PlayerView::plClicked);
}

PlayQueueView::~PlayQueueView()
{
    delete ui;
}

void PlayQueueView::setupQueueListUi() 
{
    QScrollerProperties sp;
    sp.setScrollMetric(QScrollerProperties::DragVelocitySmoothingFactor, 0.6);
    sp.setScrollMetric(QScrollerProperties::MinimumVelocity, 0.0);
    sp.setScrollMetric(QScrollerProperties::MaximumVelocity, 0.5);
    sp.setScrollMetric(QScrollerProperties::AcceleratingFlickMaximumTime, 0.4);
    sp.setScrollMetric(QScrollerProperties::AcceleratingFlickSpeedupFactor, 1.2);
    sp.setScrollMetric(QScrollerProperties::SnapPositionRatio, 0.2);
    sp.setScrollMetric(QScrollerProperties::MaximumClickThroughVelocity, 0);
    sp.setScrollMetric(QScrollerProperties::DragStartDistance, 0.001);
    sp.setScrollMetric(QScrollerProperties::MousePressEventDelay, 1.0);
    sp.setScrollMetric(QScrollerProperties::OvershootDragResistanceFactor, 0.3);
    sp.setScrollMetric(QScrollerProperties::OvershootScrollDistanceFactor, 0.1);
    m_playlistViewScroller = QScroller::scroller(ui->queueList);
    m_playlistViewScroller->grabGesture(ui->queueList, QScroller::LeftMouseButtonGesture);
    m_playlistViewScroller->setScrollerProperties(sp);

    ui->queueList->setModel(m_playlistModel);
    ui->queueList->setCurrentIndex(m_playlistModel->index(m_playlist->currentIndex(), 0));

    ui->queueList->setSortingEnabled(false);
    ui->queueList->setIndentation(0);
    ui->queueList->setAllColumnsShowFocus(false);
    ui->queueList->header()->setSectionResizeMode(0, QHeaderView::ResizeMode::Fixed);
    ui->queueList->header()->setSectionResizeMode(1, QHeaderView::ResizeMode::Stretch);
    ui->queueList->header()->setSectionResizeMode(2, QHeaderView::ResizeMode::ResizeToContents);
    ui->queueList->header()->setSectionResizeMode(3, QHeaderView::ResizeMode::ResizeToContents);
    ui->queueList->header()->setSectionResizeMode(4, QHeaderView::ResizeMode::Fixed);
    ui->queueList->header()->setDefaultSectionSize(80);
    ui->queueList->setHeaderHidden(true);
    ui->queueList->header()->setStretchLastSection(false);

    ui->queueList->setDropIndicatorShown(true);
    ui->queueList->setDragDropMode(QAbstractItemView::DragDrop);
    ui->queueList->setDefaultDropAction(Qt::CopyAction);
    ui->queueList->setDragDropOverwriteMode(false);
    ui->queueList->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);

    // Disable drag and drop until we enable edit mode
    ui->queueList->setDragEnabled(false);
    ui->queueList->setAcceptDrops(false);
}

void PlayQueueView::scale()
{
    this->setMaximumSize(this->maximumSize() * UI_SCALE);
    this->setMinimumSize(this->minimumSize() * UI_SCALE);

    ui->scrollBarContainer->layout()->setContentsMargins(ui->scrollBarContainer->layout()->contentsMargins() * UI_SCALE);

    ui->scrollBar->setMaximumHeight(ui->scrollBar->maximumHeight() * UI_SCALE);
    ui->scrollBar->setMinimumHeight(ui->scrollBar->minimumHeight() * UI_SCALE);
    ui->scrollBar->setStyleSheet(getStylesheet("playqueueview.scrollBar"));

    ui->queueContainer->setContentsMargins(ui->queueContainer->contentsMargins() * UI_SCALE);
    ui->queueFrame->setMaximumSize(ui->queueFrame->maximumSize() * UI_SCALE);
    ui->queueFrame->setMinimumSize(ui->queueFrame->minimumSize() * UI_SCALE);
    ui->queueFrame->setStyleSheet(getStylesheet("playqueueview.queueFrame"));
}

void PlayQueueView::handleSelectionChanged(int index)
{
    if(index < 0)
        return;
    auto idx = m_playlistModel->index(index, 0);
    ui->queueList->selectionModel()->setCurrentIndex(idx,
                                                     QItemSelectionModel::SelectionFlag::Rows |
                                                     QItemSelectionModel::SelectionFlag::SelectCurrent);
}

void PlayQueueView::playlistPositionChanged(int currentItem)
{
    if (ui->queueList) {
        ui->queueList->setCurrentIndex(m_playlistModel->index(currentItem, 0));
        ui->queueList->update(); // Force refresh for making sure play icon is updated correctly
    }
}

void PlayQueueView::clearPlaylist()
{
    ui->queueList->update(); 
}

void PlayQueueView::removeItem()
{
    QModelIndex selection = ui->queueList->selectionModel()->currentIndex();
    m_playlistModel->removeRow(selection.row());
}

void PlayQueueView::handleSongSelected(const QModelIndex &index)
{
    printf("Song selected\n");
}

