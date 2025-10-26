#include "globalShortcuts.h"

GlobalKeyFilter::GlobalKeyFilter(QMainWindow *win) : mainWin(win) {}

bool GlobalKeyFilter::eventFilter(QObject *watched, QEvent *event) {
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        QPoint pos = mainWin->pos();

        // Inc window size
        if ((keyEvent->key() == Qt::Key_Plus || keyEvent->key() == Qt::Key_Equal) && (keyEvent->modifiers() & Qt::ControlModifier)) {
            QSize s = mainWin->size();
            mainWin->resize(s.width() + 20, s.height() + 20);
            qDebug() << "Increased window size to" << mainWin->size();
            return true;
        }

        // Dec window size
        if (keyEvent->key() == Qt::Key_Minus && (keyEvent->modifiers() & Qt::ControlModifier)) {
            QSize s = mainWin->size();
            mainWin->resize(s.width() - 20, s.height() - 20);
            qDebug() << "Decreased window size to" << mainWin->size();
            return true;
        }

        // Move left
        if (keyEvent->key() == Qt::Key_Left && (keyEvent->modifiers() & Qt::ControlModifier)) {
            mainWin->move(pos.x() - 10, pos.y());
            qDebug() << "Window moved left, curr pos:" << mainWin->pos();
            return true;
        }

        // Move right
        if (keyEvent->key() == Qt::Key_Right && (keyEvent->modifiers() & Qt::ControlModifier)) {
            mainWin->move(pos.x() + 10, pos.y());
            qDebug() << "Window moved right, curr pos:" << mainWin->pos();
            return true;
        }

        // Move up
        if (keyEvent->key() == Qt::Key_Up && (keyEvent->modifiers() & Qt::ControlModifier)) {
            mainWin->move(pos.x(), pos.y() - 10);
            qDebug() << "Window moved up, curr pos:" << mainWin->pos();
            return true;
        }

        // Move down
        if (keyEvent->key() == Qt::Key_Down && (keyEvent->modifiers() & Qt::ControlModifier)) {
            mainWin->move(pos.x(), pos.y() + 10);
            qDebug() << "Window moved down, curr pos:" << mainWin->pos();
            return true;
        }
    }
    return QObject::eventFilter(watched, event);
}
