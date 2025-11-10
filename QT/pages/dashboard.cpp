#include "dashboard.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

#include "../tabs/trendingTab.h"
#include "../tabs/newsTab.h"
#include "../tabs/politicsTab.h"
#include "../tabs/sportsTab.h"
#include "../tabs/worldTab.h"
#include "../tabs/techTab.h"

#include <QTabWidget>
#include <QScrollArea>

DashboardPage::DashboardPage(QWidget *parent)
    : QWidget(parent),
    welcomeLabel(nullptr),
    tabWidget(nullptr),
    trendingTab(nullptr),
    newsTab(nullptr),
    politicsTab(nullptr),
    worldTab(nullptr),
    sportsTab(nullptr),
    techTab(nullptr),
    layout(nullptr){
    setupUI();
}

DashboardPage::~DashboardPage(){}

void DashboardPage::setupUI(){
    layout = new QVBoxLayout(this);
    tabWidget = new QTabWidget(this);
    welcomeLabel = new QLabel("PolyTerminal");
    welcomeLabel->setStyleSheet("font-size: 24px; font-weight: bold;");
    createTrendingTab();
    createNewsTab();
    createPoliticsTab();
    createSportsTab();
    createWorldTab();
    createTechTab();

    QWidget *contentWidget = new QWidget(this);
    QVBoxLayout *contentLayout = new QVBoxLayout(contentWidget);
    contentLayout->addWidget(welcomeLabel);
    contentLayout->addWidget(tabWidget);
    contentLayout->addStretch(1);

    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(contentWidget);

    layout->addWidget(scrollArea);
};

void DashboardPage::createTrendingTab(){
    trendingTab = new TrendingTab(this);
    tabWidget->addTab(trendingTab, "Trending");
}


void DashboardPage::createNewsTab(){
    newsTab = new NewsTab(this);
    tabWidget->addTab(newsTab, "News");
}

void DashboardPage::createPoliticsTab(){
    politicsTab = new PoliticsTab(this);
    tabWidget->addTab(politicsTab, "Politics");
}

void DashboardPage::createWorldTab(){
    worldTab = new WorldTab(this);
    tabWidget->addTab(worldTab, "World");
}

void DashboardPage::createSportsTab(){
    sportsTab = new SportsTab(this);
    tabWidget->addTab(sportsTab, "Sports");
}

void DashboardPage::createTechTab(){
    techTab = new TechTab(this);
    tabWidget->addTab(techTab, "Tech");
}

void DashboardPage::onTabChanged(int index){}