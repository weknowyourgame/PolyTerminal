class DashboardPage : public QWidget {
    Q_OBJECT
public:
    DashboardPage(QWidget *parent = nullptr);
    ~DashboardPage();

private:
    QTabWidget *tabWidget;
    QWidget *trendingTab;
    QWidget *guideTab;
    QWidget *newsTab;
    QWidget *politicsTab;
    QWidget *sportsTab;
    QWidget *worldTab;
    QWidget *techTab;
};