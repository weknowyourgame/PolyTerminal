class PortfolioPage : public QWidget {
    Q_OBJECT
public:
    PortfolioPage(QWidget *parent = nullptr);
    ~PortfolioPage();

private:
    QTabWidget *tabWidget;
    QWidget *profileTab;
    QWidget *betsTab;
    QWidget *historyTab;
    QWidget *settingsTab;
};