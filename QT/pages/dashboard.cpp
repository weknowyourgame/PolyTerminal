#include "dashboard.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

#include "../tabs/trendingTab.h"

#include <QTabWidget>

DashboardPage::DashboardPage(QWidget *parent)
    : QWidget(parent),
    welcomeLabel(nullptr),
    tabWidget(nullptr),
    trendingTab(nullptr),
    newsTab(nullptr),
    politicsTab(nullptr),
    worldTab(nullptr),
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

    layout->addWidget(welcomeLabel);
    layout->addWidget(tabWidget);
};

void DashboardPage::createTrendingTab(){
    trendingTab = new TrendingTab(this);
    tabWidget->addTab(trendingTab, "Trending");
}


void DashboardPage::createNewsTab(){
    newsTab = new QWidget(this);
    tabWidget->addTab(newsTab, "News");
}

void DashboardPage::createPoliticsTab(){
    politicsTab = new QWidget(this);
    tabWidget->addTab(politicsTab, "Politics");
}

void DashboardPage::createWorldTab(){
    worldTab = new QWidget(this);
    tabWidget->addTab(worldTab, "World");
}

void DashboardPage::createSportsTab(){
    sportsTab = new QWidget(this);
    tabWidget->addTab(sportsTab, "Sports");
}

void DashboardPage::createTechTab(){
    techTab = new QWidget(this);
    tabWidget->addTab(techTab, "Tech");
}

void DashboardPage::onTabChanged(int index){}