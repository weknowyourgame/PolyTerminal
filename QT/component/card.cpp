#include "card.h"
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPixmap>

Card::Card(QWidget *parent)
    : QWidget(parent),
    titleLabel(nullptr),
    descriptionLabel(nullptr),
    imageLabel(nullptr),
    yesPriceLabel(nullptr),
    noPriceLabel(nullptr),
    yesButton(nullptr),
    noButton(nullptr),
    mainLayout(nullptr),
    buttonLayout(nullptr),
    priceLayout(nullptr) {
    setupUI();
}

Card::Card(const QString& title, const QString& description, const QString& imagePath,
           const QString& yesPrice, const QString& noPrice, QWidget *parent)
    : QWidget(parent),
    titleLabel(nullptr),
    descriptionLabel(nullptr),
    imageLabel(nullptr),
    yesPriceLabel(nullptr),
    noPriceLabel(nullptr),
    yesButton(nullptr),
    noButton(nullptr),
    mainLayout(nullptr),
    buttonLayout(nullptr),
    priceLayout(nullptr) {
    setupUI();
    setCardData(title, description, imagePath, yesPrice, noPrice);
}

Card::~Card() {}

void Card::setupUI() {
    // Create main layout
    mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(5);
    mainLayout->setContentsMargins(8, 8, 8, 8);
    
    // Title
    titleLabel = new QLabel("Market Title", this);
    titleLabel->setWordWrap(true);

    // Description
    descriptionLabel = new QLabel("Market description goes here...", this);
    descriptionLabel->setWordWrap(true);
    
    // Image (using QLabel with pixmap)
    imageLabel = new QLabel(this);
    imageLabel->setMinimumHeight(150);
    imageLabel->setMaximumHeight(150);
    imageLabel->setAlignment(Qt::AlignCenter);
    imageLabel->setText("No Image");
    imageLabel->setScaledContents(true);
    
    // Price layout (Yes/No prices)
    priceLayout = new QHBoxLayout();
    yesPriceLabel = new QLabel("Yes: $0.50", this);
    noPriceLabel = new QLabel("No: $0.50", this);
    priceLayout->addWidget(yesPriceLabel);
    priceLayout->addWidget(noPriceLabel);
    priceLayout->addStretch();
    
    // Button layout
    buttonLayout = new QHBoxLayout();
    yesButton = new QPushButton("YES", this);
    noButton = new QPushButton("NO", this);
    
    buttonLayout->addWidget(yesButton);
    buttonLayout->addWidget(noButton);
    
    // Add everything to main layout
    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(descriptionLabel);
    mainLayout->addWidget(imageLabel);
    mainLayout->addLayout(priceLayout);
    mainLayout->addLayout(buttonLayout);
    
    setLayout(mainLayout);
    
    // Connect signals
    connect(yesButton, &QPushButton::clicked, this, &Card::onYesClicked);
    connect(noButton, &QPushButton::clicked, this, &Card::onNoClicked);
}

void Card::setCardData(const QString& title, const QString& description,
                       const QString& imagePath, const QString& yesPrice, const QString& noPrice) {
    titleLabel->setText(title);
    descriptionLabel->setText(description);
    
    // Load image if path is provided
    if (!imagePath.isEmpty()) {
        QPixmap pixmap(imagePath);
        if (!pixmap.isNull()) {
            imageLabel->setPixmap(pixmap.scaled(imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
            imageLabel->setText("");
        }
    }
    
    if (!yesPrice.isEmpty()) {
        yesPriceLabel->setText("Yes: " + yesPrice);
    }
    
    if (!noPrice.isEmpty()) {
        noPriceLabel->setText("No: " + noPrice);
    }
}

void Card::onYesClicked() {
    emit yesClicked();
}

void Card::onNoClicked() {
    emit noClicked();
}
