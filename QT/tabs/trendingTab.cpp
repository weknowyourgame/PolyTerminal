#include "trendingTab.h"
#include "../component/card.h"
#include <QLabel>
#include <QVBoxLayout>

TrendingTab::TrendingTab(QWidget *parent)
    : QWidget(parent),
    titleLabel(nullptr),
    card(nullptr),
    layout(nullptr){
    setupUI();
}

TrendingTab::~TrendingTab() {}

void TrendingTab::setupUI() {
    layout = new QVBoxLayout(this);
    
    titleLabel = new QLabel("Trending Markets", this);
    titleLabel->setStyleSheet("font-size: 20px; font-weight: bold; padding: 10px;");
    
    layout->addWidget(titleLabel);
    
    // sample card
    createCard();

    
    setLayout(layout);
}

void TrendingTab::createCard(){
    for (int i = 0; i < 10; i++){
        card = new Card(
            "Will Bitcoin hit $100k by 2025?",
            "A prediction market on Bitcoin's price movement. Place your bets!",
            "",
            "$0.65",
            "$0.35",
            this
        );
        layout->addWidget(card);
        layout->addStretch(1);
    }
}
