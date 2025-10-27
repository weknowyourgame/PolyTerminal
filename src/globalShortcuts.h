#pragma once
#include <QMainWindow>
#include <QObject>
#include <QSplitter>
#include <QKeyEvent>
#include <QDebug>

class GlobalKeyFilter : public QObject {
    Q_OBJECT
    QMainWindow *mainWin;
    QWidget *widget;
public:
    GlobalKeyFilter(QMainWindow *win) : mainWin(win) {};
protected:
    bool eventFilter(QObject *watched, QEvent *event) override;
};
