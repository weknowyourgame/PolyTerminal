#include <QWidget>
#include <QTabWidget>
#include <QVBoxLayout>

// PortfolioPage - User portfolio with tabs for profile, bets, history, settings
class PortfolioPage : public QWidget {
    Q_OBJECT

public:
    PortfolioPage(QWidget *parent = nullptr);
    ~PortfolioPage();

signals:
    void profileUpdated();
    void betPlaced();

private slots:
    void onTabChanged(int index);

private:
    // UI Elements - QTabWidget with QWidget tabs
    QTabWidget *tabWidget;
    QWidget *profileTab;
    QWidget *betsTab;
    QWidget *historyTab;
    QWidget *settingsTab;

    QVBoxLayout *layout;

    void setupUI();
    void createProfileTab();
    void createBetsTab();
    void createHistoryTab();
    void createSettingsTab();
};

