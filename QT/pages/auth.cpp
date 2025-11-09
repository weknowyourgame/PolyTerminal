#include "auth.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <httplib.h>

const char* BACKEND_URL = "http://localhost:8888";

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

bool AuthPage::attemptLogin(){
    errorLabel->clear();
    httplib::Client client(BACKEND_URL);
    std::string body = "email=" + emailField->text().toStdString() + "&password=" + passwordField->text().toStdString();
    auto res = client.Post("/login", body, "application/x-www-form-urlencoded");
    if (res && res->status == 200) {
        emit loginSuccessful();
        return true;
    } else {
        QString errorMsg = res ? QString::fromStdString(res->body) : "Connection failed";
        errorLabel->setText(errorMsg);
        emit loginFailed(errorMsg);
        return false;
    }
}

bool AuthPage::attemptSignup(){
    errorLabel->clear();
    httplib::Client client(BACKEND_URL);
    std::string body = "email=" + emailField->text().toStdString() + "&password=" + passwordField->text().toStdString();
    auto res = client.Post("/signup", body, "application/x-www-form-urlencoded");
    if (res && res->status == 200) {
        emit signupSuccessful();
        return true;
    } else {
        QString errorMsg = res ? QString::fromStdString(res->body) : "Connection failed";
        errorLabel->setText(errorMsg);
        emit signupFailed(errorMsg);
        return false;
    }
}