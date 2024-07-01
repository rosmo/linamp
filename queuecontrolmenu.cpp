#include "queuecontrolmenu.h"

#include <QFile>

QueueControlMenu::QueueControlMenu(QWidget *parent)
: QMenu(parent)
{
}

void QueueControlMenu::setVisible(bool visible)
{
    if (visible) {
        if (parentWidget() != nullptr) {
            const QPoint menuTopLeft = pos();
            const QSize menuSize = sizeHint();
            int menuY = menuTopLeft.y() - menuSize.height();

            move(QPoint(menuTopLeft.x(), menuY));
        }
    }
    QMenu::setVisible(visible);
}
