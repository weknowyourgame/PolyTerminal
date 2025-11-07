#ifndef AUTHPAGE_H
#define AUTHPAGE_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

class AuthPage : public QWidget {
    Q_OBJECT

public:
    AuthPage(QWidget *parent = nullptr);
    ~AuthPage();
    void attemptLogin();
    void attemptSignup();

// signals for MainWindow to connect to
signals:        
    void loginSuccessful();
    void loginFailed(const QString& error);

// methods connected to UI signals
private slots:  
    void onLoginClicked();
    void onSignupClicked();

private:
    // UI Elements
    QLabel *titleLabel;
    QLineEdit *emailField;
    QLineEdit *passwordField;
    QPushButton *loginButton;
    QPushButton *signupButton;
    QLabel *errorLabel;

    // Layout
    QVBoxLayout *layout;

    void setupUI();
};

#endif // AUTHPAGE_H