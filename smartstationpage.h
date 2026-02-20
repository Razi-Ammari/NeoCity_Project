#ifndef SMARTSTATIONPAGE_H
#define SMARTSTATIONPAGE_H

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
#include <QMessageBox>
#include <QScrollArea>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QChart>
#include <QtCharts/QLegend>

class SmartStationPage : public QWidget
{
    Q_OBJECT

public:
    explicit SmartStationPage(QWidget *parent = nullptr);
    ~SmartStationPage();

private slots:
    void onAddStationClicked();
    void onEditStationClicked();
    void onDeleteStationClicked();
    void onSearchTextChanged(const QString &text);
    void onSortByVolumeClicked();
    void onFilterStatusChanged(int index);
    void onExportExcelClicked();
    void onExportPDFClicked();
    void updateRFIDLog();
    void updateBusArrival();
    void updateStatistics();

private:
    // UI Creation Methods
    void createUI();
    QFrame* createHeaderSection();
    QFrame* createKPISection();
    QFrame* createStationManagementSection();
    QFrame* createSearchSortSection();
    QFrame* createOccupancyChartSection();
    QFrame* createRFIDLogSection();
    QFrame* createBusArrivalSection();
    QFrame* createExportSection();
    
    // Helper Methods
    QFrame* createCard(const QString &title, const QString &value, const QString &color);
    void setupTable();
    void loadSampleData();
    void updateKPICards();
    void updateOccupancyChart();
    void applyDarkTheme();
    QString getStatusColor(const QString &status);
    
    // Main Layout
    QVBoxLayout *mainLayout;
    
    // KPI Labels
    QLabel *totalStationsLabel;
    QLabel *stationsFullLabel;
    QLabel *totalPassengersLabel;
    QLabel *avgOccupancyLabel;
    
    // Station Management
    QTableWidget *stationTable;
    QPushButton *addStationBtn;
    QPushButton *editStationBtn;
    QPushButton *deleteStationBtn;
    
    // Search & Filter
    QLineEdit *searchBox;
    QPushButton *sortVolumeBtn;
    QComboBox *filterStatusCombo;
    
    // Charts
    QChartView *occupancyChartView;
    QPieSeries *occupancySeries;
    
    // RFID Log
    QTableWidget *rfidLogTable;
    
    // Bus Arrival
    QLabel *busArrivalLabel;
    QTimer *busArrivalTimer;
    int busArrivalSeconds;
    
    // Update Timers
    QTimer *rfidUpdateTimer;
    QTimer *statsUpdateTimer;
    
    // Data Storage
    struct Station {
        QString id;
        QString location;
        int capacity;
        int currentPassengers;
        QString status;
    };
    
    QList<Station> stations;
    int nextStationId;
};

#endif // SMARTSTATIONPAGE_H
