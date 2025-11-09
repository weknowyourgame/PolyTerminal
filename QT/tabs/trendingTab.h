#ifndef TRENDINGTAB_H
#define TRENDINGTAB_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

class Card;

class TrendingTab : public QWidget {
    Q_OBJECT

public:
    TrendingTab(QWidget *parent = nullptr);
    ~TrendingTab();

private:
    QLabel *titleLabel;
    Card *card;
    QVBoxLayout *layout;

    void setupUI();
    void createCard();
};

#endif // TRENDINGTAB_H
