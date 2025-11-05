#include <QMainWindow>
#include "pages/auth.h"
#include "pages/dashboard.h"
#include "pages/portfolio.h"
#include "pages/event.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onLoginSuccessful();
    void showDashboard();
    void showPortfolio();
    void showEvents();

private:
    AuthPage *authPage;
    DashboardPage *dashboardPage;
    PortfolioPage *portfolioPage;
    EventPage *eventPage;

    void initializePages();
    void setupMenuBar();
};

