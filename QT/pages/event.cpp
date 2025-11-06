#include "event.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

EventPage::EventPage(QWidget *parent)
    : QWidget(parent)
    tabWidget(nullptr),
    browseTab(nullptr),
    searchTab(nullptr),
    categoriesTab(nullptr),
    layout(nullptr),{
    setupUI();
}

EventPage::~EventPage()

void EventPage::setupUI(){
    layout = new QVBoxLayout(this);
    tabWidget = new QTabWidget(this);

    createBrowseTab();
    createSearchTab();
    createCategoriesTab();
};

void EventPage::createBrowseTab(){}

void EventPage::createSearchTab(){}

void EventPage::createCategoriesTab(){}
