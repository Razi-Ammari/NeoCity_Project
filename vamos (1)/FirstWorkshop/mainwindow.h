#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QDial>
#include <QTimer>
#include <QPrinter>
#include <QPainter>
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>
#include <QDateTime>
#include <QHeaderView>
#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QStatusBar>
#include <QSplitter>
#include <QFrame>

// QtCharts
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QChart>

// Qt5 only needs this macro; Qt6 has no separate QtCharts namespace
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
QT_CHARTS_USE_NAMESPACE
#endif

// Serial port for Arduino
#include <QSerialPort>
#include <QSerialPortInfo>

    QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

// ─────────────────────────────────────────────
//  Bus data structure
// ─────────────────────────────────────────────
struct Bus {
    QString id;
    QString route;
    int     battery;      // 0-100 %
    double  speed;        // km/h
    double  distanceCovered; // km
    QString status;       // "Active", "Charging", "Retired"
    double  posX;         // map position X (0.0 – 1.0)
    double  posY;         // map position Y (0.0 – 1.0)
};

// ─────────────────────────────────────────────
//  Main Window
// ─────────────────────────────────────────────
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // CRUD
    void onAddBus();
    void onDeleteBus();
    void onUpdateBattery();

    // Search / filter
    void onSearchChanged(const QString &text);
    void onFilterBattery(int index);

    // PDF Export
    void onExportPDF();

    // Chart
    void refreshChart();

    // Route map
    void updateRouteMap();

    // Speed / Obstacle monitor (real-time timer)
    void onTimerTick();

    // Arduino serial
    void onSerialDataReady();

    // Table selection
    void onTableSelectionChanged();

private:
    // ── UI helpers ──────────────────────────────
    void setupTable();
    void setupChart();
    void setupRouteMap();
    void setupSpeedDial();
    void setupSerialPort();
    void populateSampleData();
    void refreshTable();
    void drawBusOnMap(const Bus &bus);
    QString batteryColor(int pct);

    // ── Data ────────────────────────────────────
    QList<Bus> m_buses;          // Master list
    QList<Bus> m_filtered;       // Current view after filter
    int        m_selectedRow = -1;

    // ── Widgets (created in code, no .ui needed) ─
    QWidget        *m_central      = nullptr;

    // Left panel – fleet table
    QTableWidget   *m_table        = nullptr;
    QLineEdit      *m_searchBox    = nullptr;
    QComboBox      *m_battFilter   = nullptr;
    QPushButton    *m_btnAdd       = nullptr;
    QPushButton    *m_btnDelete    = nullptr;
    QPushButton    *m_btnUpdate    = nullptr;
    QPushButton    *m_btnExport    = nullptr;

    // Right panel – charts + map + dial
    QChartView     *m_chartView    = nullptr;
    QChart         *m_chart        = nullptr;

    QLabel         *m_mapLabel     = nullptr;   // Route map canvas
    QPixmap         m_mapPixmap;

    QDial          *m_speedDial     = nullptr;
    QLabel         *m_speedLabel    = nullptr;
    QLabel         *m_obstacleLabel = nullptr;
    QLabel         *m_battBar       = nullptr;   // shows selected bus info

    // ── Timer & Serial ───────────────────────────
    QTimer         *m_timer        = nullptr;
    QSerialPort    *m_serial       = nullptr;
    double          m_currentSpeed = 0.0;
    bool            m_obstacleDetected = false;

    // ── Internal ────────────────────────────────
    Ui::MainWindow *ui;          // kept for compatibility (can be nullptr)
};

#endif // MAINWINDOW_H
