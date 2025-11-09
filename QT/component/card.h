#ifndef CARD_H
#define CARD_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPixmap>

class Card : public QWidget {
    Q_OBJECT

public:
    Card(QWidget *parent = nullptr);
    Card(const QString& title, const QString& description, const QString& imagePath, 
         const QString& yesPrice = "", const QString& noPrice = "", QWidget *parent = nullptr);
    ~Card();

signals:
    void yesClicked();
    void noClicked();

private slots:
    void onYesClicked();
    void onNoClicked();

private:
    QLabel *titleLabel;
    QLabel *descriptionLabel;
    QLabel *imageLabel;
    QLabel *yesPriceLabel;
    QLabel *noPriceLabel;
    QPushButton *yesButton;
    QPushButton *noButton;
    
    QVBoxLayout *mainLayout;
    QHBoxLayout *buttonLayout;
    QHBoxLayout *priceLayout;
    
    void setupUI();
    void setCardData(const QString& title, const QString& description, 
                     const QString& imagePath, const QString& yesPrice, const QString& noPrice);
};

#endif // CARD_H
