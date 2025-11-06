#include "auth.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

AuthPage::AuthPage(QWidget *parent)
    : QWidget(parent),
    titleLabel(nullptr),
    emailField(nullptr),
    passwordField(nullptr),
    loginButton(nullptr),
    signupButton(nullptr),
    errorLabel(nullptr),
    layout(nullptr){
    setupUI();
}

AuthPage::~AuthPage(){}

void AuthPage::setupUI(){
    titleLabel = new QLabel("Welcome to PolyTerminal");
    emailField = new QLineEdit;
    emailField->setPlaceholderText("john@gmail.com");
    passwordField = new QLineEdit;
    passwordField->setPlaceholderText("*******");
    passwordField->setEchoMode(QLineEdit::Password);

    loginButton = new QPushButton("Login", this);
    signupButton = new QPushButton("Signup", this);
    errorLabel = new QLabel(this);
    errorLabel->setStyleSheet("color: red");

    // Layout
    layout = new QVBoxLayout(this);
    layout->addWidget(titleLabel);
    layout->addWidget(emailField);
    layout->addWidget(passwordField);
    layout->addWidget(loginButton);
    layout->addWidget(signupButton);
    layout->addWidget(errorLabel);

    setLayout(layout);

    connect(loginButton, &QPushButton::clicked, this, &AuthPage::onLoginClicked);
    connect(signupButton, &QPushButton::clicked, this, &AuthPage::onSignupClicked);
}

void AuthPage::onLoginClicked(){
    attemptLogin();
}

void AuthPage::onSignupClicked(){
    attemptSignup();
}

void AuthPage::attemptLogin(){
    // TODO: Implement mysql & grpc login
}

void AuthPage::attemptSignup(){
    // TODO: Implement mysql & grpc signup
}