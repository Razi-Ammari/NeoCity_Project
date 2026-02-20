#ifndef SMARTLIGHTINGMODULE_H
#define SMARTLIGHTINGMODULE_H

#include <QWidget>
#include <QTableWidget>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QSlider>
#include <QTimer>
#include <QVector>
#include <QProgressBar>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChartView>

class SmartLightingModule : public QWidget
{
    Q_OBJECT

public:
    explicit SmartLightingModule(QWidget *parent = nullptr);
    ~SmartLightingModule();

signals:
    void lightingModeChanged(QString mode);
    void energySaved(double percentage);
    void streetlightStatusChanged(QString poleId, QString status);

private slots:
    void updateLightingData();
    void onModeChanged(int index);
    void onManualIntensityChanged(int value);
    void onRefreshData();
    void onExportEnergyReport();
    void onSimulateFailure();

private:
    // UI Creation Methods
    void setupUI();
    void createKPISection();
    void createModeControlSection();
    void createEnergyChart();
    void createStreetlightTable();
    void applyStyles();
    
    // Helper methods
    QString getCardStyle();
    QString getTableStyle();
    QString getButtonStyle(const QString &color);
    void updateStreetlight(const QString &poleId, const QString &location,
                          int intensity, const QString &presence, const QString &status);
    void updateEnergyData();
    void calculateEnergySavings();
    void addLogMessage(const QString &message);
    QString getStatusColor(const QString &status);
    
    // KPI Components
    QLabel *m_energySavedLabel;
    QLabel *m_currentModeLabel;
    QLabel *m_totalPolesLabel;
    QLabel *m_activePolesLabel;
    QProgressBar *m_energyBar;
    
    // Mode Control
    QComboBox *m_modeSelector;
    QSlider *m_manualIntensitySlider;
    QLabel *m_intensityValueLabel;
    QPushButton *m_applyModeBtn;
    
    // Energy Chart
    QChart *m_energyChart;
    QLineSeries *m_energySeries;
    QChartView *m_energyChartView;
    
    // Streetlight Table
    QTableWidget *m_streetlightTable;
    
    // Control Buttons
    QPushButton *m_refreshBtn;
    QPushButton *m_exportBtn;
    QPushButton *m_simulateBtn;
    
    // Data tracking
    QTimer *m_updateTimer;
    double m_energySavedPercentage;
    QString m_currentMode;
    int m_totalPoles;
    int m_activePoles;
    int m_manualIntensity;
    QVector<double> m_energyHistory; // Last 24 hours
    
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

#endif // SMARTLIGHTINGMODULE_H
