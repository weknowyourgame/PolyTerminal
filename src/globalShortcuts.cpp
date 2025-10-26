#include "globalShortcuts.h"

GlobalKeyFilter::GlobalKeyFilter(QMainWindow *win) : mainWin(win) {}

bool GlobalKeyFilter::eventFilter(QObject *watched, QEvent *event) {
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

        if ((keyEvent->key() == Qt::Key_Plus || keyEvent->key() == Qt::Key_Equal) && (keyEvent->modifiers() & Qt::ControlModifier)) {
            QSize s = mainWin->size();
            mainWin->resize(s.width() + 20, s.height() + 20);
            qDebug() << "Increased window size to" << mainWin->size();
            return true;
        }

        if (keyEvent->key() == Qt::Key_Minus && (keyEvent->modifiers() & Qt::ControlModifier)) {
            QSize s = mainWin->size();
            mainWin->resize(s.width() - 20, s.height() - 20);
            qDebug() << "Decreased window size to" << mainWin->size();
            return true;
        }
    }
    return QObject::eventFilter(watched, event);
}
