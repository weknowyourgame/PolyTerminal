#include <QApplication>
#include <QMainWindow>
#include <QMenuBar>
#include <QAction>
#include <QDebug>
#include "constants.h"
#include <QString>
#include "globalShortcuts.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QMainWindow window;
    window.resize(1200, 800);
    window.setWindowFlags(Qt::FramelessWindowHint);
    QWidget *mainWidget = new QWidget;

    // MenuBar
    QMenuBar *menuBar = window.menuBar();
    QAction *leftButton = menuBar->addAction(QString::fromStdString(Constants::LEFT_BUTTON));
    QAction *rightButton = menuBar->addAction(QString::fromStdString(Constants::RIGHT_BUTTON));
    QAction *profileButton = menuBar->addAction(QString::fromStdString(Constants::PROFILE_BUTTON));
    QAction *settingsButton = menuBar->addAction(QString::fromStdString(Constants::SETTINGS_BUTTON));

    // Shortcut Configs
    GlobalKeyFilter *filter = new GlobalKeyFilter(&window);
    app.installEventFilter(filter);

    window.show();
    qInfo() << "Application started";
    return app.exec();
}
