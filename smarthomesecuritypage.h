#ifndef SMARTHOMESECURITYPAGE_H
#define SMARTHOMESECURITYPAGE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QFrame>
#include <QPushButton>
#include <QTableWidget>
#include <QHeaderView>
#include <QLineEdit>
#include <QComboBox>
#include <QTimer>
#include <QDateTime>
#include <QRandomGenerator>
#include <QDialog>
#include <QFormLayout>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QMessageBox>
#include <QSet>
#include <QScrollArea>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QtCharts/QValueAxis>
#include <QtCharts/QLegend>

class SmartHomeSecurityPage : public QWidget
{
    Q_OBJECT

public:
    explicit SmartHomeSecurityPage(QWidget *parent = nullptr);
    ~SmartHomeSecurityPage();

private slots:
    void onAddHomeClicked();
    void onEditHomeClicked();
    void onDeleteHomeClicked();
    void onSearchTextChanged(const QString &text);
    void onSortByRiskClicked();
    void onFilterAlertChanged(int index);
    void onEmergencyShutdownClicked();
    void onGenerateCertificateClicked();
    void updateSensorData();
    void updateEnvironmentalChart();
    void checkGasAlerts();

private:
    // UI Creation Methods
    void createUI();
    QFrame* createHeaderSection();
    QFrame* createKPISection();
    QFrame* createSearchSortSection();
    QFrame* createHomeMonitoringSection();
    QFrame* createEnvironmentalChartSection();
    QFrame* createEmergencySection();
    QFrame* createCertificateSection();
    
    // Helper Methods
    QFrame* createCard(const QString &title, const QString &value, const QString &color);
    void setupTable();
    void loadSampleData();
    void updateKPICards();
    void applyDarkTheme();
    QString getAlertStatusColor(const QString &status);
    int calculateRiskScore(double gas, double smoke, double temp, double humidity);
    
    // Main Layout
    QVBoxLayout *mainLayout;
    
    // KPI Labels
    QLabel *totalHomesLabel;
    QLabel *activeAlertsLabel;
    QLabel *safeHomesLabel;
    QLabel *warningHomesLabel;
    
    // Home Management
    QTableWidget *homeTable;
    QPushButton *addHomeBtn;
    QPushButton *editHomeBtn;
    QPushButton *deleteHomeBtn;
    
    // Search & Filter
    QLineEdit *searchBox;
    QPushButton *sortRiskBtn;
    QComboBox *filterAlertCombo;
    
    // Charts
    QChartView *environmentalChartView;
    QLineSeries *temperatureSeries;
    QLineSeries *humiditySeries;
    QList<double> temperatureHistory;
    QList<double> humidityHistory;
    
    // Emergency Controls
    QPushButton *emergencyShutdownBtn;
    bool emergencyMode;
    
    // Certificate
    QPushButton *generateCertBtn;
    
    // Update Timers
    QTimer *sensorUpdateTimer;
    QTimer *chartUpdateTimer;
    QTimer *gasAlertTimer;
    
    // Data Storage
    struct Home {
        QString id;
        QString ownerName;
        QString contact;
        QString address;
        double gasLevel;
        double smokeLevel;
        double temperature;
        double humidity;
        QString alertStatus;
    };
    
    QList<Home> homes;
    int nextHomeId;
    
    // Alert tracking to prevent repeated alerts
    QSet<QString> alertedHomes;
    
    // Thresholds
    const double GAS_CRITICAL_THRESHOLD = 500.0;  // ppm
    const double SMOKE_WARNING_THRESHOLD = 300.0; // ppm
    const double TEMP_HIGH_THRESHOLD = 40.0;      // °C
    const double HUMIDITY_HIGH_THRESHOLD = 80.0;  // %
};

#endif // SMARTHOMESECURITYPAGE_H
