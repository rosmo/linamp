#include <QFileDialog>

#include "playqueuewindow.h"
#include "desktopplayerwindow.h"
#include "qstandardpaths.h"
#include "ui_desktopplayerwindow.h"
#include "scale.h"
#include "util.h"

#ifdef IS_EMBEDDED
#include "embeddedbasewindow.h"
#include "ui_embeddedbasewindow.h"
#else
#include "desktopbasewindow.h"
#include "ui_desktopbasewindow.h"
#endif

#include <QLabel>
#include <QVBoxLayout>
#include <QMessageBox>

#ifdef IS_EMBEDDED
const unsigned int WINDOW_W = 320 * UI_SCALE;
const unsigned int WINDOW_H = 100 * UI_SCALE;
#else
const unsigned int WINDOW_W = 277 * UI_SCALE;
const unsigned int WINDOW_H = 117 * UI_SCALE;
#endif

PlayQueueWindow::PlayQueueWindow(QWidget *parent, PlaylistModel *playlistModel)
     : QMainWindow{parent}
{
    m_playlistModel = playlistModel;

    // Prepare playlist queue window
    #ifdef IS_EMBEDDED
    EmbeddedBaseWindow *playQueueWindow = new EmbeddedBaseWindow(this);
    #else
    DesktopBaseWindow *playQueueWindow = new DesktopBaseWindow(this);
    #endif

    queueControlButtons = new QueueControlButtonsWidget(this);

    playQueue = new PlayQueueView(this, m_playlistModel, queueControlButtons);
    playQueue->setAttribute(Qt::WidgetAttribute::WA_StyledBackground,  true);

    playQueueWindow->setAttribute(Qt::WidgetAttribute::WA_StyledBackground,  true);

    DesktopPlayerWindow *playQueueWindowContent = new DesktopPlayerWindow(this);
    QVBoxLayout *playQueueLayout = new QVBoxLayout;
    playQueueLayout->setContentsMargins(0, 0, 0, 0);
    playQueueLayout->addWidget(playQueue);
    playQueueWindowContent->ui->playerViewContainer->setLayout(playQueueLayout);
    QVBoxLayout *queueButtonsLayout = new QVBoxLayout;
    queueButtonsLayout->setContentsMargins(0, 0, 0, 0);
    queueButtonsLayout->addWidget(queueControlButtons);
    playQueueWindowContent->ui->controlButtonsContainer->setLayout(queueButtonsLayout);

    QHBoxLayout *playQueueContentLayout = new QHBoxLayout;
    playQueueContentLayout->setContentsMargins(0, 0, 0, 0);
    playQueueContentLayout->addWidget(playQueueWindowContent);
    playQueueWindow->ui->body->setLayout(playQueueContentLayout);
    
    // Prepare navigation stack
    viewStack = new QStackedLayout;
    viewStack->addWidget(playQueueWindow);
    viewStack->setCurrentIndex(0);

    // Final UI setup and show
    QVBoxLayout *centralLayout = new QVBoxLayout;
    centralLayout->addLayout(viewStack);
    centralLayout->setContentsMargins(0, 0, 0, 0);
    centralLayout->setSpacing(0);
    QWidget *centralWidget = new QWidget;
    centralWidget->setLayout(centralLayout);

    setCentralWidget(centralWidget);

    resize(WINDOW_W, WINDOW_H);
    this->setMaximumWidth(WINDOW_W);
    this->setMaximumHeight(WINDOW_H);
    this->setMinimumWidth(WINDOW_W);
    this->setMinimumHeight(WINDOW_H);

    #ifndef IS_EMBEDDED
    setWindowFlags(Qt::CustomizeWindowHint);
    #endif
}

PlayQueueWindow::~PlayQueueWindow()
{

}

// Shows a standard file picker for adding items to the playlist
// Not used currently
void PlayQueueWindow::open()
{
    return;
}
