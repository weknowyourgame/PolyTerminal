#ifndef NEWSTAB_H
#define NEWSTAB_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

class Card;

class NewsTab : public QWidget {
    Q_OBJECT

public:
    NewsTab(QWidget *parent = nullptr);
    ~NewsTab();

private:
    QLabel *titleLabel;
    Card *card;
    QVBoxLayout *layout;

    void setupUI();
    void createCard();
};

#endif // NEWSTAB_H
