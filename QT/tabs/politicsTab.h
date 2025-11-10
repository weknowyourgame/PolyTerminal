#ifndef POLITICSTAB_H
#define POLITICSTAB_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

class Card;

class PoliticsTab : public QWidget {
    Q_OBJECT

public:
    PoliticsTab(QWidget *parent = nullptr);
    ~PoliticsTab();

private:
    QLabel *titleLabel;
    Card *card;
    QVBoxLayout *layout;

    void setupUI();
    void createCard();
};

#endif // POLITICSTAB_H
