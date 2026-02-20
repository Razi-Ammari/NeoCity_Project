#ifndef SMARTRECYCLINGMODULE_H
#define SMARTRECYCLINGMODULE_H

#include <QWidget>
#include <QTableWidget>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QVector>
#include <QtCharts/QChart>
#include <QtCharts/QPieSeries>
#include <QtCharts/QChartView>

class SmartRecyclingModule : public QWidget
{
    Q_OBJECT

public:
    explicit SmartRecyclingModule(QWidget *parent = nullptr);
    ~SmartRecyclingModule();

signals:
    void binStatusChanged(QString binId, QString status);
    void rewardIssued(QString citizen, double amount);
    void wasteCollected(double amount);

private slots:
    void updateRecyclingData();
    void onRefreshData();
    void onExportReport();
    void onNotifyCollection();
    void onSimulateBinFull();

private:
    // UI Creation Methods
    void setupUI();
    void createKPISection();
    void createWasteTypesChart();
    void createBinStatusTable();
    void createCitizenRewardsTable();
    void applyStyles();
    
    // Helper methods
    QString getCardStyle();
    QString getTableStyle();
    QString getButtonStyle(const QString &color);
    void updateBinStatus(const QString &binId, const QString &location, 
                        int fillLevel, const QString &status);
    void updateCitizenReward(const QString &name, double totalRecycled, double earned);
    void addLogMessage(const QString &message);
    int calculateFillLevel(const QString &binId);
    
    // KPI Components
    QLabel *m_totalRecycledLabel;
    QLabel *m_plasticTotalLabel;
    QLabel *m_metalTotalLabel;
    QLabel *m_glassTotalLabel;
    QLabel *m_activeBinsLabel;
    
    // Waste Types Chart
    QChart *m_wasteChart;
    QPieSeries *m_wasteSeries;
    QChartView *m_wasteChartView;
    
    // Tables
    QTableWidget *m_binStatusTable;
    QTableWidget *m_citizenRewardsTable;
    
    // Control Buttons
    QPushButton *m_refreshBtn;
    QPushButton *m_exportBtn;
    QPushButton *m_notifyBtn;
    QPushButton *m_simulateBtn;
    
    // Data tracking
    QTimer *m_updateTimer;
    double m_totalRecycled;
    double m_plasticTotal;
    double m_metalTotal;
    double m_glassTotal;
    int m_activeBins;
    
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

#endif // SMARTRECYCLINGMODULE_H
