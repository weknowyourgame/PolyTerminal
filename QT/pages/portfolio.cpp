#include "portfolio.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

PortfolioPage::PortfolioPage(QWidget *parent)
    : QWidget(parent),
    tabWidget(nullptr),
    profileTab(nullptr),
    betsTab(nullptr),
    historyTab(nullptr),
    settingsTab(nullptr),
    layout(nullptr){
    setupUI();
}

PortfolioPage::~PortfolioPage(){}

void PortfolioPage::setupUI(){
    layout = new QVBoxLayout(this);
    tabWidget = new QTabWidget(this);

    createProfileTab();
    createBetsTab();
    createHistoryTab();
    createSettingsTab();

    layout->addWidget(tabWidget);
};

void PortfolioPage::createProfileTab(){}

void PortfolioPage::createBetsTab(){}

void PortfolioPage::createHistoryTab(){}

void PortfolioPage::createSettingsTab(){}

void PortfolioPage::onTabChanged(int index){}