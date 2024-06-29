#ifndef QUEUECONTROLMENU_H
#define QUEUECONTROLMENU_H

#include <QWidget>
#include <QMenu>
#include <QAction>

class QueueControlMenu : public QMenu
{
    public:
        QueueControlMenu(QWidget *parent);
        void setVisible(bool visible);
};

#endif