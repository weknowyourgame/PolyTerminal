#include "dashboard.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

DashboardPage::DashboardPage(QWidget *parent)
    : QWidget(parent),
    tabWidget(nullptr),
    trendingTab(nullptr),
    guideTab(nullptr),
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

    createTrendingTab();
    createGuideTab();
    createNewsTab();
    createPoliticsTab();
    createSportsTab();
    createWorldTab();
    createTechTab();

    layout->addWidget(tabWidget);
};

void DashboardPage::createTrendingTab(){}

void DashboardPage::createGuideTab(){}

void DashboardPage::createNewsTab(){}

void DashboardPage::createPoliticsTab(){}

void DashboardPage::createWorldTab(){}

void DashboardPage::createSportsTab(){}

void DashboardPage::createTechTab(){}

void DashboardPage::onTabChanged(int index){}