#pragma once
#include <QMainWindow>
#include <QObject>
#include <QEvent>
#include <QKeyEvent>
#include <QDebug>

class GlobalKeyFilter : public QObject {
    Q_OBJECT
    QMainWindow *mainWin;
public:
    GlobalKeyFilter(QMainWindow *win);
protected:
    bool eventFilter(QObject *watched, QEvent *event) override;
};
