#include "playqueueview.h"
#include "ui_playqueueview.h"
#include "playlistview.h"

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
#include <QScrollBar>

PlayQueueView::PlayQueueView(QWidget *parent, PlaylistModel *playlistModel, QueueControlButtonsWidget *ctlBtns) :
    QWidget(parent),
    ui(new Ui::PlayQueueView)
{
    m_playlistModel = playlistModel;
    m_playlist = m_playlistModel->playlist();

    // Setup UI
    ui->setupUi(this);
    scale();

    // Setup "queue list"
    setupQueueListUi();

    controlButtons = ctlBtns;
    connect(controlButtons, &QueueControlButtonsWidget::addFileClicked, this, &PlayQueueView::addFileClicked);
    connect(controlButtons, &QueueControlButtonsWidget::addDirectoryClicked, this, &PlayQueueView::addDirectoryClicked);
    connect(controlButtons, &QueueControlButtonsWidget::addUrlClicked, this, &PlayQueueView::addUrlClicked);

    connect(controlButtons, &QueueControlButtonsWidget::removeSelectedClicked, this, &PlayQueueView::removeSelectedClicked);
    connect(controlButtons, &QueueControlButtonsWidget::cropClicked, this, &PlayQueueView::cropClicked);
    connect(controlButtons, &QueueControlButtonsWidget::removeAllClicked, this, &PlayQueueView::removeAllClicked);

/*

    connect(controlButtons, &QueueControlButtonsWidget::addClicked, this, &PlayQueueView::addClicked);
    connect(controlButtons, &QueueControlButtonsWidget::removeClicked, this, &PlayQueueView::removeClicked);
    connect(controlButtons, &QueueControlButtonsWidget::selectClicked, this, &PlayQueueView::selectClicked);
    connect(controlButtons, &QueueControlButtonsWidget::miscClicked, this, &PlayQueueView::miscClicked);
    connect(controlButtons, &QueueControlButtonsWidget::optionsClicked, this, &PlayQueueView::optionsClicked);
*/
        // Connect playlist model to our QTreeView
    connect(m_playlist, &QMediaPlaylist::currentIndexChanged, this, &PlayQueueView::playlistPositionChanged);
    connect(m_playlist, &QMediaPlaylist::currentSelectionChanged, this, &PlayQueueView::handleSelectionChanged);
    connect(ui->queueList, &QAbstractItemView::doubleClicked, this, &PlayQueueView::handleSongSelected);
    
    // Handle scrolling
    connect(ui->scrollBar, &QScrollBar::sliderMoved, this, &PlayQueueView::scrollBarMoved);
    connect(m_playlist, &QMediaPlaylist::mediaAboutToBeRemoved, this, &PlayQueueView::updateScrollBar);
    connect(ui->queueList->verticalScrollBar(), &QScrollBar::valueChanged, this, &PlayQueueView::queueListScrolling);

    // Handle playlist edit to update scrollbar
    connect(m_playlist, &QMediaPlaylist::mediaAboutToBeInserted, this, &PlayQueueView::updateScrollBar);
    connect(m_playlist, &QMediaPlaylist::mediaAboutToBeRemoved, this, &PlayQueueView::updateScrollBar);
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
    ui->queueList->setSelectionMode(QAbstractItemView::SelectionMode::MultiSelection);

    // Disable drag and drop
    ui->queueList->setDragEnabled(false);
    ui->queueList->setAcceptDrops(false);
    
    // Disable scroll bar as we will be implementing it in another way
    // ui->queueList->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->scrollBar->setRange(0, 0);
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

void PlayQueueView::queueListScrolling(int value) 
{
    Q_UNUSED(value);

    auto rect = ui->queueList->rect();
    auto bottomIndex = ui->queueList->indexAt(QPoint(rect.left() + 10, rect.bottom() - 10));
    // auto topIndex = ui->queueList->indexAt(QPoint(10, 10));
    int scrollBarMax = m_playlistModel->rowCount() - numberOfItems;
    int bottomItemIndex = bottomIndex.row() - numberOfItems;
    int scrollBarValue = scrollBarMax - bottomItemIndex - 1;
    ui->scrollBar->setValue(scrollBarValue);
}

void PlayQueueView::updateScrollBar(int start, int end)
{
    Q_UNUSED(start);
    Q_UNUSED(end);

    int range = m_playlistModel->rowCount() - numberOfItems;
    if (range < 0) {
        range = 0;
    }
    ui->scrollBar->setRange(0, range);
    scrollBarRange = range;
    ui->scrollBar->setValue(range);
}

void PlayQueueView::scrollBarMoved(int value)
{
    int index = (scrollBarRange - value) + numberOfItems;
    ui->queueList->scrollTo(m_playlistModel->index(index, 0));
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

void PlayQueueView::playlistCleared()
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
    emit songSelected(index);
}

void PlayQueueView::addFileClicked() 
{
    emit showPlaylistRequested();
}

void PlayQueueView::addUrlClicked() 
{
    emit showPlaylistRequested();
}

void PlayQueueView::addDirectoryClicked() 
{
    emit showPlaylistRequested();
}

void PlayQueueView::removeMiscClicked()
{
    
}

void PlayQueueView::removeAllClicked() 
{
    emit clearPlaylist();
}

void PlayQueueView::cropClicked() 
{   
    QModelIndex selection = ui->queueList->selectionModel()->currentIndex();
    printf("Cropping: %d\n", selection.row());
    for (int i = (m_playlistModel->rowCount() - 1); i >= 0; i--) {
        if (i > selection.row()) {
            m_playlistModel->removeRows(i, 1);
        } else if (i < selection.row()) {
            m_playlistModel->removeRows(0, 1);
        }
    }
}

void PlayQueueView::removeSelectedClicked() 
{
    QModelIndexList selection = ui->queueList->selectionModel()->selectedRows();
    std::sort(selection.begin(), selection.end(), std::less<QModelIndex>());
    QListIterator<QModelIndex> iter(selection);
    iter.toBack();
    while (iter.hasPrevious()) {
        QModelIndex item = iter.previous();
        printf("Item selected: %d\n", item.row());
        m_playlistModel->removeRows(item.row(), 1);
    } 
    
}

void PlayQueueView::selectAllClicked() 
{
    
}

void PlayQueueView::selectZeroClicked() 
{
  
}

void PlayQueueView::invertSelectClicked() 
{
    
}

void PlayQueueView::miscOptionsClicked() 
{
    
}

void PlayQueueView::fileInformationClicked() 
{
    
}

void PlayQueueView::sortListClicked() 
{
    
}

void PlayQueueView::newListClicked() 
{
    
}

void PlayQueueView::saveListClicked() 
{
    
}

void PlayQueueView::loadListClicked() 
{
    
}
