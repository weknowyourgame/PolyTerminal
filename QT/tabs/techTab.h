#ifndef TECHTAB_H
#define TECHTAB_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

class Card;

class TechTab : public QWidget {
    Q_OBJECT

public:
    TechTab(QWidget *parent = nullptr);
    ~TechTab();

private:
    QLabel *titleLabel;
    Card *card;
    QVBoxLayout *layout;

    void setupUI();
    void createCard();
};

#endif // TECHTAB_H
