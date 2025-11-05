#include "MainWindow.h"
#include "constants.h"
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QString>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // window properties
    setWindowTitle("PolyTerminal");
    resize(1200, 800);
    
    // all page instances
    initializePages();
    
    // navigation menu
    setupMenuBar();
    
    connect(authPage, &AuthPage::loginSuccessful,
            this, &MainWindow::onLoginSuccessful);
    
    // authentication page as default
    setCentralWidget(authPage);
}

MainWindow::~MainWindow()
{
    // Qt parent-child relationship handles cleanup automatically
    // Pages will be deleted when MainWindow is deleted
}

void MainWindow::initializePages()
{
    // Each page is a QWidget with its own internal structure
    authPage = new AuthPage(this);
    dashboardPage = new DashboardPage(this);
    portfolioPage = new PortfolioPage(this);
    eventPage = new EventPage(this);
}

void MainWindow::setupMenuBar()
{
    QMenuBar *menu = menuBar();
    
    // Menubar
    QAction *leftButton = menu->addAction(QString::fromStdString(Constants::LEFT_BUTTON));
    QAction *rightButton = menu->addAction(QString::fromStdString(Constants::RIGHT_BUTTON));
    QAction *profileButton = menu->addAction(QString::fromStdString(Constants::PROFILE_BUTTON));
    QAction *settingsButton = menu->addAction(QString::fromStdString(Constants::SETTINGS_BUTTON));
    
    // spacer to push navigation to the right
    menu->setCornerWidget(new QWidget(), Qt::TopRightCorner);
    
    // Navigation menu
    QMenu *navMenu = menu->addMenu("Navigate");
    
    // Dashboard action
    QAction *dashboardAction = navMenu->addAction("Dashboard");
    connect(dashboardAction, &QAction::triggered,
            this, &MainWindow::showDashboard);
    
    // Portfolio action
    QAction *portfolioAction = navMenu->addAction("Portfolio");
    connect(portfolioAction, &QAction::triggered,
            this, &MainWindow::showPortfolio);
    
    // Events action
    QAction *eventsAction = navMenu->addAction("Events");
    connect(eventsAction, &QAction::triggered,
            this, &MainWindow::showEvents);
    
    // TODO: Connect leftButton, rightButton, profileButton, settingsButton to their handlers
}

// slot: called when user successfully logs in
void MainWindow::onLoginSuccessful()
{
    setCentralWidget(dashboardPage);
}

// slot: show dashboard page
void MainWindow::showDashboard()
{
    setCentralWidget(dashboardPage);
}

// slot: show portfolio page
void MainWindow::showPortfolio()
{
    setCentralWidget(portfolioPage);
}

// slot: show events page
void MainWindow::showEvents()
{
    setCentralWidget(eventPage);
}
