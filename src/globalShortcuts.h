#include <QObject>
#include <QEvent>
#include <QKeyEvent>
#include <QDebug>

class GlobalKeyFilter : public QObject {
    Q_OBJECT
    protected:
        bool eventFilter(QObject *watched, QEvent *event) override {
            if (event->type() == QEvent::KeyPress) {
                QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

                // Inc size
                if (keyEvent->key() == Qt::Key_Plus && keyEvent->key() == Qt::Key_Control) {
                    qDebug() << "Inc window size";

                    return true;
                }

                // Dec Size
                if (keyEvent->key() == Qt::Key_Minus && keyEvent->key() == Qt::Key_Control) {
                    qDebug() << "Dec window size";

                    return true;
                }
            }
        };

};