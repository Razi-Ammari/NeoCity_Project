#ifndef SECURITYINTELLIGENCECENTER_H
#define SECURITYINTELLIGENCECENTER_H

#include <QWidget>
#include <QTableWidget>
#include <QPlainTextEdit>
#include <QProgressBar>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QTimer>
#include <QDateTime>
#include <QVector>
#include <QScrollArea>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChartView>
#include <QtCharts/QValueAxis>

class SecurityIntelligenceCenter : public QWidget
{
    Q_OBJECT

public:
    explicit SecurityIntelligenceCenter(QWidget *parent = nullptr);
    ~SecurityIntelligenceCenter();

signals:
    void threatDetected(QString module, int severity);
    void riskUpdated(int newScore);
    void recommendationGenerated(QString text);

private slots:
    // Simulation triggers
    void onSimulateSpeedViolation();
    void onSimulateWasteOverload();
    void onSimulateLightingFailure();
    void onSimulateCyberAttack();
    void onResetSystemState();
    void onClearAlerts();
    void onTriggerSimulatedThreat();
    
    // Recommendation actions
    void onApproveRecommendation();
    void onRejectRecommendation();
    
    // Simulation mode toggle
    void onSimulationModeToggled(bool enabled);
    
    // Auto-update
    void updateRiskMetrics();
    void generatePrediction();

private:
    // UI Creation Methods
    void setupUI();
    void createSystemHealthPanel();
    void createThreatDetectionPanel();
    void createSecurityLogPanel();
    void createPredictiveIntelligencePanel();
    void createStabilityScorePanel();
    void createSimulationControlPanel();
    
    // Styling
    void applyStyles();
    QString getCardStyle();
    QString getTableStyle();
    QString getButtonStyle(const QString &color);
    
    // Logic methods
    void addLogEntry(const QString &level, const QString &message);
    void addThreatAlert(const QString &type, const QString &module, 
                        const QString &severity, const QString &timestamp);
    void updateSystemHealth(const QString &module, const QString &status, 
                           const QString &risk);
    void updateCityStabilityScore(int score);
    void addRiskDataPoint(double value);
    void generateStrategicRecommendation();
    int calculateCityStability();
    
    // Panel 1: System Health Overview
    QTableWidget *m_systemHealthTable;
    
    // Panel 2: Intrusion & Threat Detection
    QTableWidget *m_threatDetectionTable;
    QPushButton *m_clearAlertsBtn;
    QPushButton *m_triggerThreatBtn;
    
    // Panel 3: Live Security Log
    QPlainTextEdit *m_securityLog;
    
    // Panel 4: Predictive Intelligence
    QTableWidget *m_predictiveTable;
    QLabel *m_recommendationLabel;
    QLabel *m_recommendationTypeLabel;
    QLabel *m_priorityLabel;
    QPushButton *m_approveBtn;
    QPushButton *m_rejectBtn;
    
    // Panel 5: City Stability & Risk Score
    QProgressBar *m_stabilityBar;
    QLabel *m_stabilityScoreLabel;
    QChart *m_riskChart;
    QLineSeries *m_riskSeries;
    QChartView *m_chartView;
    
    // Panel 6: Simulation Control
    QCheckBox *m_simulationModeCheckbox;
    QPushButton *m_simSpeedBtn;
    QPushButton *m_simWasteBtn;
    QPushButton *m_simLightingBtn;
    QPushButton *m_simCyberBtn;
    QPushButton *m_resetBtn;
    
    // Data tracking
    QVector<double> m_riskHistory;
    int m_currentStabilityScore;
    bool m_simulationMode;
    QTimer *m_updateTimer;
    
    // Risk factors (simulated)
    int m_wasteRisk;
    int m_safetyRisk;
    int m_energyRisk;
    int m_cyberThreatLevel;
    
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

#endif // SECURITYINTELLIGENCECENTER_H
