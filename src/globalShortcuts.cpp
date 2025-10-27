#include "globalShortcuts.h"
#include <QTextEdit>

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
            mainWin->move(pos.x() - 20, pos.y());
            qDebug() << "Window moved left, curr pos:" << mainWin->pos();
            return true;
        }

        // Move right
        if (keyEvent->key() == Qt::Key_Right && (keyEvent->modifiers() & Qt::ControlModifier)) {
            mainWin->move(pos.x() + 20, pos.y());
            qDebug() << "Window moved right, curr pos:" << mainWin->pos();
            return true;
        }

        // Move up
        if (keyEvent->key() == Qt::Key_Up && (keyEvent->modifiers() & Qt::ControlModifier)) {
            mainWin->move(pos.x(), pos.y() - 20);
            qDebug() << "Window moved up, curr pos:" << mainWin->pos();
            return true;
        }

        // Move down
        if (keyEvent->key() == Qt::Key_Down && (keyEvent->modifiers() & Qt::ControlModifier)) {
            mainWin->move(pos.x(), pos.y() + 20);
            qDebug() << "Window moved down, curr pos:" << mainWin->pos();
            return true;
        }

    // TODO: Make panes divide infintely depending on window size
        // Divide panes horizontal
        if (keyEvent->key() == Qt::Key_J && (keyEvent->modifiers() & Qt::ControlModifier)) {
            QTextEdit *topWidget = new QTextEdit();
            QTextEdit *bottomWidget = new QTextEdit();

            QSplitter *splitter = new QSplitter(Qt::Vertical);
            splitter->addWidget(topWidget);
            splitter->addWidget(bottomWidget);

            mainWin->setCentralWidget(splitter);

            qDebug() << "Window divided horizontally";
            return true;
        }

        // Divides panes vertical
        if (keyEvent->key() == Qt::Key_H && (keyEvent->modifiers() & Qt::ControlModifier)) {
            QTextEdit *topWidget = new QTextEdit();
            QTextEdit *bottomWidget = new QTextEdit();

            QSplitter *splitter = new QSplitter(Qt::Horizontal);
            splitter->addWidget(topWidget);
            splitter->addWidget(bottomWidget);

            mainWin->setCentralWidget(splitter);

            qDebug() << "Window divided vertically";
            return true;
        }

        // Quit
        if (keyEvent->key() == Qt::Key_Q && (keyEvent->modifiers() & Qt::ControlModifier)) {
            mainWin->close();

            qDebug() << "Window closed";
            return true;
        }

    }
    return QObject::eventFilter(watched, event);
}
