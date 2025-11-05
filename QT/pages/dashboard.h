#include <QWidget>
#include <QTabWidget>
#include <QVBoxLayout>

// DashboardPage - Main app page with actual tabs for different categories
class DashboardPage : public QWidget {
    Q_OBJECT

public:
    DashboardPage(QWidget *parent = nullptr);
    ~DashboardPage();

signals:
    void categorySelected(const QString& category);

private slots:
    void onTabChanged(int index);

private:
    // UI Elements - QTabWidget with QWidget tabs
    QTabWidget *tabWidget;
    QWidget *trendingTab;
    QWidget *guideTab;
    QWidget *newsTab;
    QWidget *politicsTab;
    QWidget *sportsTab;
    QWidget *worldTab;
    QWidget *techTab;

    QVBoxLayout *layout;

    void setupUI();
    void createTrendingTab();
    void createGuideTab();
    void createNewsTab();
    void createPoliticsTab();
    void createSportsTab();
    void createWorldTab();
    void createTechTab();
};