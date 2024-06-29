#ifndef QUEUECONTROLBUTTONSWIDGET_H
#define QUEUECONTROLBUTTONSWIDGET_H

#include <QWidget>
#include <QMenu>
#include <QAction>
#include "queuecontrolmenu.h"

namespace Ui {
class QueueControlButtonsWidget;
}

typedef struct {
    QString icon;
    QueueControlMenu **menu;
    QAction **action;
} QueueControlIcon;

class QueueControlButtonsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit QueueControlButtonsWidget(QWidget *parent = nullptr);
    ~QueueControlButtonsWidget();

private:
    Ui::QueueControlButtonsWidget *ui;
    void scale();

    QueueControlMenu *addMenu;
    QAction *addFileAction;
    QAction *addDirAction;
    QAction *addUrlAction;

    QueueControlMenu *removeMenu;
    QAction *removeMiscAction;
    QAction *removeAllAction;
    QAction *removeCropAction;
    QAction *removeSelAction;

    QueueControlMenu *selectMenu;
    QAction *zeroSelectAction;
    QAction *allSelectAction;
    QAction *invSelectAction;

    QueueControlMenu *miscMenu;
    QAction *sortMiscAction;
    QAction *infMiscAction;
    QAction *optsMiscAction;

    QueueControlMenu *optionsMenu;
    QAction *newOptionsAction;
    QAction *loadOptionsAction;
    QAction *saveOptionsAction;
    
    QMap<QString, QueueControlIcon> iconMap{
        {"add-0", {.icon = QString("qadd_url"), .menu = &addMenu, .action = &addUrlAction}},
        {"add-1", {.icon = QString("qadd_dir"), .menu = &addMenu, .action = &addDirAction}},
        {"add-2", {.icon = QString("qadd_file"), .menu = &addMenu, .action = &addFileAction}},

        {"remove-0", {.icon = QString("qrem_misc"), .menu = &removeMenu, .action = &removeMiscAction}},
        {"remove-1", {.icon = QString("qrem_all"),  .menu = &removeMenu, .action = &removeAllAction}},
        {"remove-2", {.icon = QString("qrem_crop"), .menu = &removeMenu, .action = &removeCropAction}},
        {"remove-3", {.icon = QString("qrem_sel"),  .menu = &removeMenu, .action = &removeSelAction}},
        
        {"sel-0", {.icon = QString("qsel_inv"),  .menu = &selectMenu, .action = &invSelectAction}},
        {"sel-1", {.icon = QString("qsel_zero"), .menu = &selectMenu, .action = &zeroSelectAction}},
        {"sel-2", {.icon = QString("qsel_all"),  .menu = &selectMenu, .action = &allSelectAction}},

        {"misc-0", {.icon = QString("qmisc_sort"), .menu = &miscMenu, .action = &sortMiscAction}},
        {"misc-1", {.icon = QString("qmisc_inf"),  .menu = &miscMenu, .action = &infMiscAction}},
        {"misc-2", {.icon = QString("qmisc_opts"), .menu = &miscMenu, .action = &optsMiscAction}},

        {"list-0", {.icon = QString("qlistopts_new"),  .menu = &optionsMenu, .action = &newOptionsAction}},
        {"list-1", {.icon = QString("qlistopts_save"), .menu = &optionsMenu, .action = &saveOptionsAction}},
        {"list-2", {.icon = QString("qlistopts_load"), .menu = &optionsMenu, .action = &loadOptionsAction}},
    };

    

signals:
    void addClicked();
    void removeClicked();
    void selectClicked();
    void miscClicked();
    void optionsClicked();
};

#endif // CONTROLBUTTONSWIDGET_H
