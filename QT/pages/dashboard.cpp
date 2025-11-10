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
#include <QDebug>
#include <httplib.h>
#include "../component/card.h"

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
    layout(nullptr),
    searchResultsWidget(nullptr),
    mainScrollArea(nullptr),
    searchResultsLayout(nullptr){
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
    createSearchBar();

    layout->addWidget(searchBar);

    QWidget *contentWidget = new QWidget(this);
    QVBoxLayout *contentLayout = new QVBoxLayout(contentWidget);
    contentLayout->addWidget(welcomeLabel);
    contentLayout->addWidget(tabWidget);
    contentLayout->addStretch(1);

    mainScrollArea = new QScrollArea(this);
    mainScrollArea->setWidgetResizable(true);
    mainScrollArea->setWidget(contentWidget);

    QWidget *searchContentWidget = new QWidget(this);
    searchResultsLayout = new QVBoxLayout(searchContentWidget);
    QLabel *searchLabel = new QLabel("Search Results", searchContentWidget);
    searchLabel->setObjectName("SearchResultsLabel");
    searchLabel->setStyleSheet("font-size: 20px; font-weight: bold; padding: 20px;");
    searchResultsLayout->addWidget(searchLabel);
    searchResultsLayout->addStretch(1);
    
    QScrollArea *searchScrollArea = new QScrollArea(this);
    searchScrollArea->setWidgetResizable(true);
    searchScrollArea->setWidget(searchContentWidget);
    
    searchResultsWidget = searchScrollArea;
    
    // Initially hide search results, show main content
    searchResultsWidget->hide();
    
    layout->addWidget(mainScrollArea);
    layout->addWidget(searchResultsWidget);
};

void DashboardPage::createSearchBar(){
    searchBar = new QLineEdit(this);
    searchBar->setPlaceholderText("Search");
    searchBar->setStyleSheet("font-size: 16px;");
    layout->addWidget(searchBar);
    
    connect(searchBar, &QLineEdit::textChanged, this, &DashboardPage::onSearchChanged);
}

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

void DashboardPage::onSearchChanged(const QString& text){
    if (text.isEmpty()) {
        showMainContent();
    } else {
        showSearchResults();
        
        httplib::Client client("http://localhost:8888");
        auto res = client.Get("/search?q=" + text.toStdString());
        if (res && res->status == 200) {
            createSearchResultsCard(res->body);
        }
    }
}

void DashboardPage::showSearchResults(){
    mainScrollArea->hide();
    searchResultsWidget->show();
}

void DashboardPage::showMainContent(){
    searchResultsWidget->hide();
    mainScrollArea->show();
}

void DashboardPage::createSearchResultsCard(const std::string& body){
    // Clear existing cards (except the label at index 0)
    while (searchResultsLayout->count() > 1) {
        QLayoutItem* item = searchResultsLayout->takeAt(1);
        if (item) {
            if (item->widget()) {
                delete item->widget();
            }
            delete item;
        }
    }
    
    // qDebug() << "Search results JSON:" << QString::fromStdString(body);
    
    // placeholder cards for testing
    for (int i = 0; i < 5; i++){
        Card *newCard = new Card(
            QString("Search Result %1").arg(i + 1),
            "Description from search results...",
            "",
            "0.23",
            "0.77",
            this
        );
        searchResultsLayout->insertWidget(searchResultsLayout->count() - 1, newCard);
    }
    
    searchResultsLayout->addStretch(1);
}
