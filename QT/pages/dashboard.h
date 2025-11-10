#ifndef DASHBOARDPAGE_H
#define DASHBOARDPAGE_H

#include <QWidget>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QScrollArea>
#include "../tabs/trendingTab.h"
#include "../tabs/newsTab.h"
#include "../tabs/politicsTab.h"
#include "../tabs/sportsTab.h"
#include "../tabs/worldTab.h"
#include "../tabs/techTab.h"

// DashboardPage - Main app page with actual tabs for different categories
class DashboardPage : public QWidget {
    Q_OBJECT

public:
    DashboardPage(QWidget *parent = nullptr);
    ~DashboardPage();

signals:
    void categorySelected(const QString& category);

private slots:
    void onTabChanged(int index);
    void onSearchChanged(const QString& text);

private:
    // UI Elements - QTabWidget with QWidget tabs
    QLabel *welcomeLabel;
    QTabWidget *tabWidget;
    TrendingTab *trendingTab;
    NewsTab *newsTab;
    PoliticsTab *politicsTab;
    SportsTab *sportsTab;
    WorldTab *worldTab;
    TechTab *techTab;
    QLineEdit *searchBar;
    QVBoxLayout *layout;
    QWidget *searchResultsWidget;
    QScrollArea *mainScrollArea;
    QVBoxLayout *searchResultsLayout;

    void setupUI();
    void showSearchResults();
    void showMainContent();
    void createTrendingTab();
    void createGuideTab();
    void createNewsTab();
    void createPoliticsTab();
    void createSportsTab();
    void createWorldTab();
    void createTechTab();
    void createSearchBar();
    void createSearchResultsCard(const std::string& body);
};

#endif // DASHBOARDPAGE_H