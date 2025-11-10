#include "worldTab.h"
#include "../component/card.h"
#include <QLabel>
#include <QVBoxLayout>

WorldTab::WorldTab(QWidget *parent)
    : QWidget(parent),
    titleLabel(nullptr),
    card(nullptr),
    layout(nullptr){
    setupUI();
}

WorldTab::~WorldTab() {}

void WorldTab::setupUI() {
    layout = new QVBoxLayout(this);
    
    titleLabel = new QLabel("World", this);
    titleLabel->setStyleSheet("font-size: 20px; font-weight: bold; padding: 10px;");
    
    layout->addWidget(titleLabel);
    
    // sample card
    createCard();

    
    setLayout(layout);
}

void WorldTab::createCard(){
    for (int i = 0; i < 10; i++){
        card = new Card(
            "Will a major earthquake hit the United States in 2025?",
            "A prediction market on the likelihood of a major earthquake hitting the United States. Place your bets!",
            "",
            "$0.65",
            "$0.35",
            this
        );
        layout->addWidget(card);
        layout->addStretch(1);
    }
}
