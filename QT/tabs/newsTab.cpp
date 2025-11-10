#include "newsTab.h"
#include "../component/card.h"
#include <QLabel>
#include <QVBoxLayout>

NewsTab::NewsTab(QWidget *parent)
    : QWidget(parent),
    titleLabel(nullptr),
    card(nullptr),
    layout(nullptr){
    setupUI();
}

NewsTab::~NewsTab() {}

void NewsTab::setupUI() {
    layout = new QVBoxLayout(this);
    
    titleLabel = new QLabel("News", this);
    titleLabel->setStyleSheet("font-size: 20px; font-weight: bold; padding: 10px;");
    
    layout->addWidget(titleLabel);
    
    // sample card
    createCard();

    
    setLayout(layout);
}

void NewsTab::createCard(){
    for (int i = 0; i < 10; i++){
        card = new Card(
            "News article title goes here...",
            "News article goes here...",
            "",
            "$0.65",
            "$0.35",
            this
        );
        layout->addWidget(card);
        layout->addStretch(1);
    }
}
