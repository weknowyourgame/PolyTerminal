#ifndef SPORTSTAB_H
#define SPORTSTAB_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

class Card;

class SportsTab : public QWidget {
    Q_OBJECT

public:
    SportsTab(QWidget *parent = nullptr);
    ~SportsTab();

private:
    QLabel *titleLabel;
    Card *card;
    QVBoxLayout *layout;

    void setupUI();
    void createCard();
};

#endif // SPORTSTAB_H
