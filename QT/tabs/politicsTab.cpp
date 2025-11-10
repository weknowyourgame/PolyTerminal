#include "politicsTab.h"
#include "../component/card.h"
#include <QLabel>
#include <QVBoxLayout>

PoliticsTab::PoliticsTab(QWidget *parent)
    : QWidget(parent),
    titleLabel(nullptr),
    card(nullptr),
    layout(nullptr){
    setupUI();
}

PoliticsTab::~PoliticsTab() {}

void PoliticsTab::setupUI() {
    layout = new QVBoxLayout(this);
    
    titleLabel = new QLabel("Politics", this);
    titleLabel->setStyleSheet("font-size: 20px; font-weight: bold; padding: 10px;");
    
    layout->addWidget(titleLabel);
    
    // sample card
    createCard();

    
    setLayout(layout);
}

void PoliticsTab::createCard(){
    for (int i = 0; i < 10; i++){
        card = new Card(
            "Will trump be re-elected in 2024?",
            "A prediction market on Trump's re-election. Place your bets!",
            "",
            "$0.65",
            "$0.35",
            this
        );
        layout->addWidget(card);
        layout->addStretch(1);
    }
}
