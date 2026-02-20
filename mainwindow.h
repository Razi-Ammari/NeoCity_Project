#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStackedWidget>
#include <QPushButton>
#include <QLabel>
#include <QFrame>
#include <QTimer>
#include <QDateTime>
#include <QStyle>
#include "securityintelligencecenter.h"
#include "smartrecyclingmodule.h"
#include "pedestriansafetymodule.h"
#include "smartlightingmodule.h"
#include "analyticsmodule.h"
#include "cityintelligencemodule.h"
#include "smartstationpage.h"
#include "smarthomesecuritypage.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void updateDateTime();
    void navigateToPage(int index);
    void updateSystemStatus();
    void lazyCreatePage(int index);  // Lazy loading helper

private:
    Ui::MainWindow *ui;
    
    // Main layout components
    QWidget *centralWidget;
    QHBoxLayout *mainLayout;
    
    // Top bar components
    QWidget *topBar;
    QLabel *titleLabel;
    QLabel *subtitleLabel;
    QLabel *dateTimeLabel;
    QLabel *statusIndicator;
    
    // Navigation menu components
    QWidget *navigationMenu;
    QVBoxLayout *navLayout;
    QVector<QPushButton*> navButtons;
    
    // Content area
    QWidget *contentArea;
    QVBoxLayout *contentLayout;
    QStackedWidget *stackedWidget;
    
    // Pages
    QWidget *dashboardPage;
    QWidget *recyclingPage;
    QWidget *safetyPage;
    QWidget *lightingPage;
    QWidget *securityPage;
    QWidget *analyticsPage;
    QWidget *stationPage;
    QWidget *homeSecurityPage;
    QWidget *settingsPage;
    QWidget *aboutPage;
    
    // Dashboard components
    QWidget *kpiCardsContainer;
    QWidget *statusCardsContainer;
    QWidget *cityMapContainer;
    
    // Timer for updates
    QTimer *updateTimer;
    
    // Helper methods
    void setupUI();
    void createTopBar();
    void createNavigationMenu();
    void createDashboard();
    void createKPICards();
    void createStatusCards();
    void createCityMap();
    void applyStyles();
    QFrame* createKPICard(const QString &icon, const QString &title, const QString &value, const QString &subtext);
    QFrame* createStatusCard(const QString &title, const QString &status, const QString &colorClass);
};
#endif // MAINWINDOW_H
