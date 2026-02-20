#ifndef CITYINTELLIGENCEMODULE_H
#define CITYINTELLIGENCEMODULE_H

#include <QWidget>
#include <QTableWidget>
#include <QPlainTextEdit>
#include <QLabel>
#include <QPushButton>
#include <QProgressBar>
#include <QTimer>
#include <QVector>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChartView>

class CityIntelligenceModule : public QWidget
{
    Q_OBJECT

public:
    explicit CityIntelligenceModule(QWidget *parent = nullptr);
    ~CityIntelligenceModule();

signals:
    void predictionGenerated(QString predictionType, int severity);
    void recommendationIssued(QString recommendation);
    void cityStabilityChanged(int score);

private slots:
    void updateIntelligenceData();
    void onGeneratePrediction();
    void onRefreshForecasts();
    void onApplyRecommendation();
    void onIgnoreRecommendation();
    void onRunSimulation();

private:
    // UI Creation Methods
    void setupUI();
    void createCityStabilitySection();
    void createPredictiveAlertsTable();
    void createRiskForecastChart();
    void createAIRecommendationsPanel();
    void createDecisionLog();
    void applyStyles();
    
    // Helper methods
    QString getCardStyle();
    QString getTableStyle();
    QString getButtonStyle(const QString &color);
    void addPredictiveAlert(const QString &category, const QString &prediction,
                           const QString &probability, const QString &timeframe);
    void generateNewRecommendation();
    void updateCityStability();
    void addDecisionLog(const QString &decision);
    void addRiskDataPoint();
    QString getRiskLevelColor(int score);
    
    // City Stability Components
    QLabel *m_stabilityScoreLabel;
    QProgressBar *m_stabilityBar;
    QLabel *m_stabilityStatusLabel;
    QLabel *m_riskFactorsLabel;
    
    // Predictive Alerts Table
    QTableWidget *m_predictiveAlertsTable;
    
    // Risk Forecast Chart
    QChart *m_riskChart;
    QLineSeries *m_riskSeries;
    QChartView *m_riskChartView;
    
    // AI Recommendations
    QLabel *m_currentRecommendationLabel;
    QLabel *m_confidenceLabel;
    QLabel *m_impactLabel;
    QPushButton *m_applyBtn;
    QPushButton *m_ignoreBtn;
    
    // Decision Log
    QPlainTextEdit *m_decisionLog;
    
    // Control Buttons
    QPushButton *m_predictBtn;
    QPushButton *m_refreshBtn;
    QPushButton *m_simulateBtn;
    
    // Data tracking
    QTimer *m_updateTimer;
    int m_cityStabilityScore;
    QVector<double> m_riskForecast;
    QStringList m_aiRecommendations;
    int m_currentRecommendationIndex;
    
    // Risk factors
    int m_infrastructureRisk;
    int m_environmentalRisk;
    int m_socialRisk;
    int m_economicRisk;
    
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

#endif // CITYINTELLIGENCEMODULE_H
