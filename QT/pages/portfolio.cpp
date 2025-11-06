#include "portfolio.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

PortfolioPage::PortfolioPage(QWidget *parent)
    : QWidget(parent)
    tabWidget(nullptr),
    profileTab(nullptr),
    betsTab(nullptr),
    historyTab(nullptr),
    settingsTab(nullptr),
    layout(nullptr),{
    setupUI();
}

PortfolioPage::~PortfolioPage()

void PortfolioPage::setupUI(){
    createProfileTab();
    createBetsTab();
    createSettingsTab();
    createHistoryTab();
    createSettingsTab();
};

void PortfolioPage::createProfileTab(){}

void PortfolioPage::createBetsTab(){}

void PortfolioPage::createHistoryTab(){}

void PortfolioPage::createSettingsTab(){}