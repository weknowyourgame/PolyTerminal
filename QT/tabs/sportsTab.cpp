#include "sportsTab.h"
#include "../component/card.h"
#include <QLabel>
#include <QVBoxLayout>

SportsTab::SportsTab(QWidget *parent)
    : QWidget(parent),
    titleLabel(nullptr),
    card(nullptr),
    layout(nullptr){
    setupUI();
}

SportsTab::~SportsTab() {}

void SportsTab::setupUI() {
    layout = new QVBoxLayout(this);
    
    titleLabel = new QLabel("Sports", this);
    titleLabel->setStyleSheet("font-size: 20px; font-weight: bold; padding: 10px;");
    
    layout->addWidget(titleLabel);
    
    // sample card
    createCard();

    
    setLayout(layout);
}

void SportsTab::createCard(){
    for (int i = 0; i < 10; i++){
        card = new Card(
            "Will the Lakers win the NBA championship in 2025?",
            "A prediction market on the Lakers' chances of winning the NBA championship. Place your bets!",
            "",
            "$0.65",
            "$0.35",
            this
        );
        layout->addWidget(card);
        layout->addStretch(1);
    }
}
