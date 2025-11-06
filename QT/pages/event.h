#ifndef EVENTPAGE_H
#define EVENTPAGE_H

#include <QWidget>
#include <QTabWidget>
#include <QVBoxLayout>

// EventPage - Browse and explore events/markets with tabs
class EventPage : public QWidget {
    Q_OBJECT

public:
    EventPage(QWidget *parent = nullptr);
    ~EventPage();

signals:
    void eventSelected(const QString& eventId);

private slots:
    void onTabChanged(int index);

private:
    // UI Elements - QTabWidget with QWidget tabs
    QTabWidget *tabWidget;
    QWidget *browseTab;
    QWidget *searchTab;
    QWidget *categoriesTab;

    QVBoxLayout *layout;

    void setupUI();
    void createBrowseTab();
    void createSearchTab();
    void createCategoriesTab();
};

#endif // EVENTPAGE_H
