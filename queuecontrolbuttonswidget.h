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
    QString name;
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

    QAction *action(QString name);

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
        {"add-0", {.name = QString("add-url"),       .icon = QString("qadd_url"),  .menu = &addMenu, .action = &addUrlAction,  }},
        {"add-1", {.name = QString("add-directory"), .icon = QString("qadd_dir"),  .menu = &addMenu, .action = &addDirAction,  }},
        {"add-2", {.name = QString("add-file"),      .icon = QString("qadd_file"), .menu = &addMenu, .action = &addFileAction  }},

        {"remove-0", {.name = QString("remove-misc"),    .icon = QString("qrem_misc"), .menu = &removeMenu, .action = &removeMiscAction}},
        {"remove-1", {.name = QString("remove-all"),     .icon = QString("qrem_all"),  .menu = &removeMenu, .action = &removeAllAction}},
        {"remove-2", {.name = QString("remove-crop"),    .icon = QString("qrem_crop"), .menu = &removeMenu, .action = &removeCropAction}},
        {"remove-3", {.name = QString("remove-select"), .icon = QString("qrem_sel"),  .menu = &removeMenu, .action = &removeSelAction}},
        
        {"sel-0", {.name = QString("select-inverse"), .icon = QString("qsel_inv"),  .menu = &selectMenu, .action = &invSelectAction}},
        {"sel-1", {.name = QString("select-zero"),    .icon = QString("qsel_zero"), .menu = &selectMenu, .action = &zeroSelectAction}},
        {"sel-2", {.name = QString("select-all"),     .icon = QString("qsel_all"),  .menu = &selectMenu, .action = &allSelectAction}},

        {"misc-0", {.name = QString("sort"),             .icon = QString("qmisc_sort"), .menu = &miscMenu, .action = &sortMiscAction}},
        {"misc-1", {.name = QString("file-information"), .icon = QString("qmisc_inf"),  .menu = &miscMenu, .action = &infMiscAction}},
        {"misc-2", {.name = QString("options"),          .icon = QString("qmisc_opts"), .menu = &miscMenu, .action = &optsMiscAction}},

        {"list-0", {.name = QString("list-new"),  .icon = QString("qlistopts_new"),  .menu = &optionsMenu, .action = &newOptionsAction}},
        {"list-1", {.name = QString("list-save"), .icon = QString("qlistopts_save"), .menu = &optionsMenu, .action = &saveOptionsAction}},
        {"list-2", {.name = QString("list-load"), .icon = QString("qlistopts_load"), .menu = &optionsMenu, .action = &loadOptionsAction}},
    };

public slots:
    void beepBoop();

signals:
    void addFileClicked(bool checked = false);
    void addUrlClicked(bool checked = false);
    void addDirectoryClicked(bool checked = false);

    void removeMiscClicked(bool checked = false);
    void removeAllClicked(bool checked = false);
    void cropClicked(bool checked = false);
    void removeSelectedClicked(bool checked = false);

    void selectAllClicked(bool checked = false);
    void selectZeroClicked(bool checked = false);
    void invertSelectClicked(bool checked = false);

    void miscOptionsClicked(bool checked = false);
    void fileInformationClicked(bool checked = false);
    void sortListClicked(bool checked = false);

    void newListClicked(bool checked = false);
    void saveListClicked(bool checked = false);
    void loadListClicked(bool checked = false);
};

#endif // CONTROLBUTTONSWIDGET_H
