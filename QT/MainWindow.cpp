#include "MainWindow.h"
#include "constants.h"
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QString>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("PolyTerminal");
    resize(1200, 800);
    
    // Setup menu bar
    setupMenuBar();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupMenuBar()
{
    QMenuBar *menu = menuBar();
    
    // Menubar buttons
    QAction *leftButton = menu->addAction(QString::fromStdString(Constants::LEFT_BUTTON));
    QAction *rightButton = menu->addAction(QString::fromStdString(Constants::RIGHT_BUTTON));
    QAction *profileButton = menu->addAction(QString::fromStdString(Constants::PROFILE_BUTTON));
    QAction *settingsButton = menu->addAction(QString::fromStdString(Constants::SETTINGS_BUTTON));
    
    // Spacer to push navigation to the right
    menu->setCornerWidget(new QWidget(), Qt::TopRightCorner);
    
    // TODO: Connect leftButton, rightButton, profileButton, settingsButton to their handlers
}
