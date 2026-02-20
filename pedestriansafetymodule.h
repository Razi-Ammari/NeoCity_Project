#ifndef PEDESTRIANSAFETYMODULE_H
#define PEDESTRIANSAFETYMODULE_H

#include <QWidget>
#include <QTableWidget>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QVector>
#include <QProgressBar>
#include <QtCharts/QChart>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QChartView>

class PedestrianSafetyModule : public QWidget
{
    Q_OBJECT

public:
    explicit PedestrianSafetyModule(QWidget *parent = nullptr);
    ~PedestrianSafetyModule();

signals:
    void alertTriggered(QString crosswalkId, QString alertType);
    void violationDetected(QString vehicleId, int speed);
    void riskLevelChanged(QString level);

private slots:
    void updateSafetyData();
    void onRefreshData();
    void onExportViolations();
    void onDispatchPatrol();
    void onSimulateViolation();

private:
    // UI Creation Methods
    void setupUI();
    void createKPISection();
    void createAlertsChart();
    void createViolationsTable();
    void createCrosswalkStatusTable();
    void applyStyles();
    
    // Helper methods
    QString getCardStyle();
    QString getTableStyle();
    QString getButtonStyle(const QString &color);
    void updateCrosswalkStatus(const QString &crosswalkId, const QString &location, 
                              const QString &riskLevel, const QString &status);
    void addViolation(const QString &crosswalkId, const QString &vehicleId, 
                     int speed, const QString &timestamp);
    void updateRiskLevel();
    QString getRiskColor(const QString &riskLevel);
    void addLogMessage(const QString &message);
    
    // KPI Components
    QLabel *m_totalAlertsLabel;
    QLabel *m_violationsLabel;
    QLabel *m_riskLevelLabel;
    QLabel *m_activeCrosswalksLabel;
    QProgressBar *m_riskBar;
    
    // Charts
    QChart *m_alertsChart;
    QBarSeries *m_alertsSeries;
    QChartView *m_alertsChartView;
    
    // Tables
    QTableWidget *m_violationsTable;
    QTableWidget *m_crosswalkStatusTable;
    
    // Control Buttons
    QPushButton *m_refreshBtn;
    QPushButton *m_exportBtn;
    QPushButton *m_dispatchBtn;
    QPushButton *m_simulateBtn;
    
    // Data tracking
    QTimer *m_updateTimer;
    int m_totalAlerts;
    int m_totalViolations;
    QString m_currentRiskLevel;
    int m_activeCrosswalks;
    QVector<int> m_alertHistory; // Last 7 days
    
    // Color definitions
    const QString COLOR_BACKGROUND = "#121212";
    const QString COLOR_PANEL = "#1E1E1E";
    const QString COLOR_BORDER = "#2A2A2A";
    const QString COLOR_ACCENT = "#1E90FF";
    const QString COLOR_SUCCESS = "#00C853";
    const QString COLOR_WARNING = "#FF9800";
    const QString COLOR_CRITICAL = "#D32F2F";
    const QString COLOR_TEXT = "#FFFFFF";
    const QString COLOR_TEXT_DIM = "#CCCCCC";
};

#endif // PEDESTRIANSAFETYMODULE_H
