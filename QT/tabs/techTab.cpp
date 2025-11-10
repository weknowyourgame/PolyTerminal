#include "techTab.h"
#include "../component/card.h"
#include <QLabel>
#include <QVBoxLayout>

TechTab::TechTab(QWidget *parent)
    : QWidget(parent),
    titleLabel(nullptr),
    card(nullptr),
    layout(nullptr){
    setupUI();
}

TechTab::~TechTab() {}

void TechTab::setupUI() {
    layout = new QVBoxLayout(this);
    
    titleLabel = new QLabel("Tech", this);
    titleLabel->setStyleSheet("font-size: 20px; font-weight: bold; padding: 10px;");
    
    layout->addWidget(titleLabel);
    
    // sample card
    createCard();

    
    setLayout(layout);
}

void TechTab::createCard(){
    for (int i = 0; i < 10; i++){
        card = new Card(
            "Will AI pass the Turing test by 2030?",
            "A prediction market on the advancement of AI. Place your bets!",
            "",
            "$0.65",
            "$0.35",
            this
        );
        layout->addWidget(card);
        layout->addStretch(1);
    }
}

