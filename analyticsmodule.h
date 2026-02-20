#ifndef ANALYTICSMODULE_H
#define ANALYTICSMODULE_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QDateEdit>
#include <QTimer>
#include <QVector>
#include <QtCharts/QChart>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLineSeries>
#include <QtCharts/QPieSeries>
#include <QtCharts/QChartView>

class AnalyticsModule : public QWidget
{
    Q_OBJECT

public:
    explicit AnalyticsModule(QWidget *parent = nullptr);
    ~AnalyticsModule();

signals:
    void reportGenerated(QString reportType);
    void dataExported(QString format);

private slots:
    void updateAnalyticsData();
    void onRefreshData();
    void onExportPDF();
    void onExportExcel();
    void onGenerateReport();
    void onDateRangeChanged();

private:
    // UI Creation Methods
    void setupUI();
    void createControlSection();
    void createRecyclingAnalytics();
    void createSafetyAnalytics();
    void createEnergyAnalytics();
    void createSystemOverview();
    void applyStyles();
    
    // Helper methods
    QString getCardStyle();
    QString getButtonStyle(const QString &color);
    void generateRecyclingData();
    void generateSafetyData();
    void generateEnergyData();
    void addLogMessage(const QString &message);
    
    // Control Components
    QComboBox *m_reportTypeSelector;
    QDateEdit *m_startDateEdit;
    QDateEdit *m_endDateEdit;
    QPushButton *m_generateBtn;
    QPushButton *m_exportPdfBtn;
    QPushButton *m_exportExcelBtn;
    QPushButton *m_refreshBtn;
    
    // Chart 1: Recycling Analytics
    QChart *m_recyclingChart;
    QBarSeries *m_recyclingSeries;
    QChartView *m_recyclingChartView;
    QLabel *m_recyclingInsightLabel;
    
    // Chart 2: Safety Analytics
    QChart *m_safetyChart;
    QLineSeries *m_safetySeries;
    QChartView *m_safetyChartView;
    QLabel *m_safetyInsightLabel;
    
    // Chart 3: Energy Analytics
    QChart *m_energyChart;
    QLineSeries *m_energySeries;
    QChartView *m_energyChartView;
    QLabel *m_energyInsightLabel;
    
    // Chart 4: System Overview (Pie)
    QChart *m_overviewChart;
    QPieSeries *m_overviewSeries;
    QChartView *m_overviewChartView;
    
    // Data tracking
    QTimer *m_updateTimer;
    QVector<double> m_recyclingData;
    QVector<double> m_safetyData;
    QVector<double> m_energyData;
    
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

#endif // ANALYTICSMODULE_H
