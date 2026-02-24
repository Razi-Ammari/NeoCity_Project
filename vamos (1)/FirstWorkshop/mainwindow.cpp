#include "mainwindow.h"
#include "ui_mainwindow.h"   // auto-generated – if you have no .ui file, remove this line and set ui = nullptr

#include <QApplication>
#include <QFont>
#include <QGridLayout>
#include <QSizePolicy>
#include <QPrintDialog>
#include <QTextDocument>
#include <QRandomGenerator>
#include <cmath>

// ═══════════════════════════════════════════════════════════════
//  Constructor
// ═══════════════════════════════════════════════════════════════
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(nullptr)          // we build UI manually; change to "new Ui::MainWindow" if using .ui
{
    setWindowTitle("🚌 Transport Manager – Autonomous Bus Fleet");
    setMinimumSize(1300, 820);

    // ── Central widget + master horizontal splitter ──────────────
    m_central = new QWidget(this);
    setCentralWidget(m_central);

    QHBoxLayout *masterLayout = new QHBoxLayout(m_central);
    masterLayout->setContentsMargins(12, 12, 12, 12);
    masterLayout->setSpacing(12);

    // ────────────────────────────────────────────────────────────
    //  LEFT PANEL  (Fleet table + controls)
    // ────────────────────────────────────────────────────────────
    QWidget *leftPanel = new QWidget;
    leftPanel->setMinimumWidth(560);
    QVBoxLayout *leftLayout = new QVBoxLayout(leftPanel);
    leftLayout->setSpacing(8);

    // Title
    QLabel *titleLabel = new QLabel("🚌  Bus Fleet Dashboard");
    titleLabel->setStyleSheet("font-size:18px; font-weight:800; color:#00d4ff; padding:6px 0;");
    leftLayout->addWidget(titleLabel);

    // ── Search + Filter row ──────────────────────────────────────
    QHBoxLayout *filterRow = new QHBoxLayout;
    m_searchBox = new QLineEdit;
    m_searchBox->setPlaceholderText("🔍  Search by Bus ID or Route…");
    m_searchBox->setStyleSheet("padding:6px 10px; border-radius:8px; background:#1e2a3a; color:#e2e8f0; border:1px solid #2d3f55;");

    m_battFilter = new QComboBox;
    m_battFilter->addItem("All Batteries");
    m_battFilter->addItem("🔴  Battery < 20%");
    m_battFilter->addItem("🟡  Battery < 50%");
    m_battFilter->addItem("🟢  Battery ≥ 50%");
    m_battFilter->setStyleSheet("padding:6px 8px; border-radius:8px; background:#1e2a3a; color:#e2e8f0; border:1px solid #2d3f55;");

    filterRow->addWidget(m_searchBox, 2);
    filterRow->addWidget(m_battFilter, 1);
    leftLayout->addLayout(filterRow);

    // ── Table ────────────────────────────────────────────────────
    setupTable();
    leftLayout->addWidget(m_table);

    // ── CRUD buttons ────────────────────────────────────────────
    QHBoxLayout *crudRow = new QHBoxLayout;
    m_btnAdd    = new QPushButton("➕  Add Bus");
    m_btnDelete = new QPushButton("🗑️  Remove Bus");
    m_btnUpdate = new QPushButton("🔋  Update Battery");
    m_btnExport = new QPushButton("📄  Export PDF Log");

    QString btnBase = "padding:9px 14px; border-radius:9px; font-weight:700; font-size:13px;";
    m_btnAdd   ->setStyleSheet(btnBase + "background:#0ea5e9; color:#fff;");
    m_btnDelete->setStyleSheet(btnBase + "background:#ef4444; color:#fff;");
    m_btnUpdate->setStyleSheet(btnBase + "background:#f59e0b; color:#000;");
    m_btnExport->setStyleSheet(btnBase + "background:#22c55e; color:#fff;");

    crudRow->addWidget(m_btnAdd);
    crudRow->addWidget(m_btnDelete);
    crudRow->addWidget(m_btnUpdate);
    crudRow->addWidget(m_btnExport);
    leftLayout->addLayout(crudRow);

    masterLayout->addWidget(leftPanel, 3);

    // ────────────────────────────────────────────────────────────
    //  RIGHT PANEL  (Chart | Map | Speed Dial)
    // ────────────────────────────────────────────────────────────
    QWidget *rightPanel = new QWidget;
    rightPanel->setMinimumWidth(480);
    QVBoxLayout *rightLayout = new QVBoxLayout(rightPanel);
    rightLayout->setSpacing(10);

    // Bar chart
    QGroupBox *chartBox = new QGroupBox("📊  Distance Covered by Bus (km)");
    chartBox->setStyleSheet("QGroupBox{font-weight:700;color:#00d4ff;border:1px solid #1e2d45;border-radius:10px;margin-top:6px;padding-top:8px;}");
    QVBoxLayout *chartLayout = new QVBoxLayout(chartBox);
    setupChart();
    chartLayout->addWidget(m_chartView);
    rightLayout->addWidget(chartBox, 3);

    // Route Map
    QGroupBox *mapBox = new QGroupBox("🗺️  Live Route Map");
    mapBox->setStyleSheet("QGroupBox{font-weight:700;color:#7fff6e;border:1px solid #1e2d45;border-radius:10px;margin-top:6px;padding-top:8px;}");
    QVBoxLayout *mapLayout = new QVBoxLayout(mapBox);
    setupRouteMap();
    mapLayout->addWidget(m_mapLabel);
    rightLayout->addWidget(mapBox, 2);

    // Speed + Obstacle
    QGroupBox *dialBox = new QGroupBox("⚡  Speed & Obstacle Monitor");
    dialBox->setStyleSheet("QGroupBox{font-weight:700;color:#ff6b35;border:1px solid #1e2d45;border-radius:10px;margin-top:6px;padding-top:8px;}");
    QHBoxLayout *dialLayout = new QHBoxLayout(dialBox);
    setupSpeedDial();
    dialLayout->addWidget(m_speedDial);

    QVBoxLayout *infoLayout = new QVBoxLayout;
    m_speedLabel    = new QLabel("Speed: 0 km/h");
    m_obstacleLabel = new QLabel("Obstacle: CLEAR ✅");
    m_battBar       = new QLabel("Select a bus to see battery");

    m_speedLabel->setStyleSheet("font-size:16px;font-weight:700;color:#ff6b35;");
    m_obstacleLabel->setStyleSheet("font-size:14px;font-weight:700;color:#22c55e;");
    m_battBar->setStyleSheet("font-size:12px;color:#94a3b8;");

    infoLayout->addWidget(m_speedLabel);
    infoLayout->addWidget(m_obstacleLabel);
    infoLayout->addWidget(m_battBar);
    infoLayout->addStretch();
    dialLayout->addLayout(infoLayout);

    rightLayout->addWidget(dialBox, 2);

    masterLayout->addWidget(rightPanel, 2);

    // ────────────────────────────────────────────────────────────
    //  Status bar
    // ────────────────────────────────────────────────────────────
    statusBar()->setStyleSheet("color:#64748b; font-size:12px;");
    statusBar()->showMessage("Transport Manager ready  •  " +
                             QDateTime::currentDateTime().toString("dd/MM/yyyy  hh:mm"));

    // ── Signals / Slots ─────────────────────────────────────────
    connect(m_btnAdd,    &QPushButton::clicked, this, &MainWindow::onAddBus);
    connect(m_btnDelete, &QPushButton::clicked, this, &MainWindow::onDeleteBus);
    connect(m_btnUpdate, &QPushButton::clicked, this, &MainWindow::onUpdateBattery);
    connect(m_btnExport, &QPushButton::clicked, this, &MainWindow::onExportPDF);
    connect(m_searchBox, &QLineEdit::textChanged, this, &MainWindow::onSearchChanged);
    connect(m_battFilter, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onFilterBattery);
    connect(m_table, &QTableWidget::itemSelectionChanged,
            this, &MainWindow::onTableSelectionChanged);

    // ── Timer (simulates live sensor data at 1 Hz) ───────────────
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &MainWindow::onTimerTick);
    m_timer->start(1000);

    // ── Populate demo data ───────────────────────────────────────
    populateSampleData();
    refreshTable();
    refreshChart();
}

// ═══════════════════════════════════════════════════════════════
//  Destructor
// ═══════════════════════════════════════════════════════════════
MainWindow::~MainWindow()
{
    if (ui) delete ui;
    if (m_serial && m_serial->isOpen()) m_serial->close();
}

// ═══════════════════════════════════════════════════════════════
//  SETUP HELPERS
// ═══════════════════════════════════════════════════════════════

void MainWindow::setupTable()
{
    m_table = new QTableWidget(0, 7);
    m_table->setHorizontalHeaderLabels({
        "Bus ID", "Route", "Battery %", "Speed (km/h)",
        "Distance (km)", "Status", "Position"
    });
    m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_table->setSelectionBehavior(QTableWidget::SelectRows);
    m_table->setEditTriggers(QTableWidget::NoEditTriggers);
    m_table->setAlternatingRowColors(true);
    m_table->verticalHeader()->setVisible(false);
    m_table->setStyleSheet(
        "QTableWidget{background:#0d1117;color:#e2e8f0;gridline-color:#1e2d45;border:1px solid #1e2d45;border-radius:8px;}"
        "QTableWidget::item:selected{background:#0ea5e9;color:#fff;}"
        "QHeaderView::section{background:#111827;color:#00d4ff;font-weight:700;padding:6px;border:1px solid #1e2d45;}"
        "QTableWidget::item:alternate{background:#0f172a;}"
        );
}

void MainWindow::setupChart()
{
    m_chart = new QChart();
    m_chart->setTitle("Distance per Bus");
    m_chart->setBackgroundBrush(QBrush(QColor("#0d1117")));
    m_chart->setTitleBrush(QBrush(QColor("#00d4ff")));
    m_chart->legend()->setLabelColor(QColor("#94a3b8"));

    m_chartView = new QChartView(m_chart);
    m_chartView->setRenderHint(QPainter::Antialiasing);
    m_chartView->setStyleSheet("border-radius:8px; background:#0d1117;");
    m_chartView->setMinimumHeight(200);
}

void MainWindow::setupRouteMap()
{
    m_mapLabel = new QLabel;
    m_mapLabel->setMinimumSize(420, 180);
    m_mapLabel->setStyleSheet("background:#0d1117; border-radius:8px; border:1px solid #1e2d45;");
    m_mapLabel->setAlignment(Qt::AlignCenter);

    // Draw a blank city grid map as base
    m_mapPixmap = QPixmap(420, 180);
    m_mapPixmap.fill(QColor("#0d1117"));

    QPainter p(&m_mapPixmap);
    p.setRenderHint(QPainter::Antialiasing);

    // Draw simple city grid roads
    p.setPen(QPen(QColor("#1e2d45"), 1));
    for (int x = 0; x <= 420; x += 42)
        p.drawLine(x, 0, x, 180);
    for (int y = 0; y <= 180; y += 36)
        p.drawLine(0, y, 420, y);

    // Main roads (highlighted)
    p.setPen(QPen(QColor("#2d3f55"), 2));
    p.drawLine(0, 90, 420, 90);   // horizontal main road
    p.drawLine(210, 0, 210, 180); // vertical main road

    // Labels
    p.setPen(QColor("#64748b"));
    p.setFont(QFont("Arial", 7));
    p.drawText(5, 12, "CITY MAP – Route Tracker");

    p.end();
    m_mapLabel->setPixmap(m_mapPixmap);
}

void MainWindow::setupSpeedDial()
{
    m_speedDial = new QDial;
    m_speedDial->setRange(0, 120);
    m_speedDial->setValue(0);
    m_speedDial->setNotchesVisible(true);
    m_speedDial->setFixedSize(110, 110);
    m_speedDial->setStyleSheet(
        "QDial{background:#1e2a3a; border-radius:55px;}"
        );
}

void MainWindow::setupSerialPort()
{
    m_serial = new QSerialPort(this);
    // Auto-detect Arduino port
    const auto ports = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : ports) {
        if (info.description().contains("Arduino", Qt::CaseInsensitive) ||
            info.portName().contains("ttyUSB") ||
            info.portName().contains("COM")) {
            m_serial->setPort(info);
            break;
        }
    }
    if (m_serial->portName().isEmpty()) return; // no Arduino found

    m_serial->setBaudRate(QSerialPort::Baud9600);
    m_serial->setDataBits(QSerialPort::Data8);
    m_serial->setParity(QSerialPort::NoParity);
    m_serial->setStopBits(QSerialPort::OneStop);

    if (m_serial->open(QIODevice::ReadOnly)) {
        connect(m_serial, &QSerialPort::readyRead,
                this, &MainWindow::onSerialDataReady);
        statusBar()->showMessage("✅ Arduino connected: " + m_serial->portName());
    }
}

// ═══════════════════════════════════════════════════════════════
//  SAMPLE DATA
// ═══════════════════════════════════════════════════════════════
void MainWindow::populateSampleData()
{
    m_buses = {
               {"BUS-001", "Route A – City Center",    85,  42.5, 128.3, "Active",   0.30, 0.40},
               {"BUS-002", "Route B – Airport",         17,   0.0, 305.1, "Charging", 0.70, 0.25},
               {"BUS-003", "Route C – University",      62,  55.0, 210.7, "Active",   0.50, 0.60},
               {"BUS-004", "Route A – City Center",      8,   0.0,  98.4, "Charging", 0.20, 0.75},
               {"BUS-005", "Route D – Stadium",         44,  38.0, 450.2, "Active",   0.80, 0.35},
               {"BUS-006", "Route B – Airport",         91,  60.0,  33.8, "Active",   0.60, 0.55},
               {"BUS-007", "Route E – Suburbs",         29,  15.0, 520.9, "Active",   0.40, 0.70},
               {"BUS-008", "Route C – University",       0,   0.0, 890.0, "Retired",  0.10, 0.90},
               };
}

// ═══════════════════════════════════════════════════════════════
//  REFRESH TABLE (applies m_filtered)
// ═══════════════════════════════════════════════════════════════
void MainWindow::refreshTable()
{
    m_filtered = m_buses;

    // Apply search filter
    QString searchText = m_searchBox->text().toLower();
    if (!searchText.isEmpty()) {
        m_filtered = QList<Bus>();
        for (const Bus &b : m_buses)
            if (b.id.toLower().contains(searchText) ||
                b.route.toLower().contains(searchText))
                m_filtered.append(b);
    }

    // Apply battery filter
    int battIdx = m_battFilter->currentIndex();
    if (battIdx == 1) {
        QList<Bus> tmp;
        for (const Bus &b : m_filtered) if (b.battery < 20)  tmp.append(b);
        m_filtered = tmp;
    } else if (battIdx == 2) {
        QList<Bus> tmp;
        for (const Bus &b : m_filtered) if (b.battery < 50)  tmp.append(b);
        m_filtered = tmp;
    } else if (battIdx == 3) {
        QList<Bus> tmp;
        for (const Bus &b : m_filtered) if (b.battery >= 50) tmp.append(b);
        m_filtered = tmp;
    }

    // Fill table
    m_table->setRowCount(0);
    for (int i = 0; i < m_filtered.size(); ++i) {
        const Bus &b = m_filtered[i];
        m_table->insertRow(i);

        auto item = [](const QString &text) -> QTableWidgetItem* {
            return new QTableWidgetItem(text);
        };

        m_table->setItem(i, 0, item(b.id));
        m_table->setItem(i, 1, item(b.route));

        // Battery cell with color
        QTableWidgetItem *battItem = item(QString::number(b.battery) + "%");
        battItem->setForeground(QColor(batteryColor(b.battery)));
        battItem->setFont(QFont("Arial", 9, QFont::Bold));
        m_table->setItem(i, 2, battItem);

        m_table->setItem(i, 3, item(QString::number(b.speed, 'f', 1)));
        m_table->setItem(i, 4, item(QString::number(b.distanceCovered, 'f', 1)));

        QTableWidgetItem *statusItem = item(b.status);
        if (b.status == "Active")    statusItem->setForeground(QColor("#22c55e"));
        else if (b.status == "Charging") statusItem->setForeground(QColor("#f59e0b"));
        else                         statusItem->setForeground(QColor("#ef4444"));
        m_table->setItem(i, 5, statusItem);

        m_table->setItem(i, 6, item(
                                   QString("(%1, %2)").arg(b.posX,0,'f',2).arg(b.posY,0,'f',2)));
    }

    statusBar()->showMessage(
        QString("Showing %1 / %2 buses  •  %3")
            .arg(m_filtered.size()).arg(m_buses.size())
            .arg(QDateTime::currentDateTime().toString("hh:mm:ss")));
}

QString MainWindow::batteryColor(int pct)
{
    if (pct < 20) return "#ef4444";   // red
    if (pct < 50) return "#f59e0b";   // orange
    return "#22c55e";                  // green
}

// ═══════════════════════════════════════════════════════════════
//  CRUD  –  ADD BUS
// ═══════════════════════════════════════════════════════════════
void MainWindow::onAddBus()
{
    bool ok;
    QString route = QInputDialog::getText(this, "Add New Bus",
                                          "Enter route name:", QLineEdit::Normal, "Route X – New Line", &ok);
    if (!ok || route.trimmed().isEmpty()) return;

    int battery = QInputDialog::getInt(this, "Add New Bus",
                                       "Initial battery %:", 80, 0, 100, 1, &ok);
    if (!ok) return;

    Bus newBus;
    newBus.id      = QString("BUS-%1").arg(m_buses.size() + 1, 3, 10, QChar('0'));
    newBus.route   = route.trimmed();
    newBus.battery = battery;
    newBus.speed   = 0.0;
    newBus.distanceCovered = 0.0;
    newBus.status  = "Active";
    newBus.posX    = QRandomGenerator::global()->generateDouble();
    newBus.posY    = QRandomGenerator::global()->generateDouble();

    m_buses.append(newBus);
    refreshTable();
    refreshChart();

    statusBar()->showMessage("✅  Bus " + newBus.id + " added successfully.");
}

// ═══════════════════════════════════════════════════════════════
//  CRUD  –  DELETE BUS
// ═══════════════════════════════════════════════════════════════
void MainWindow::onDeleteBus()
{
    int row = m_table->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "No Selection", "Please select a bus row to remove.");
        return;
    }

    QString busId = m_filtered[row].id;
    int ret = QMessageBox::question(this, "Confirm Delete",
                                    "Remove bus " + busId + " from the fleet?",
                                    QMessageBox::Yes | QMessageBox::No);

    if (ret != QMessageBox::Yes) return;

    // Remove from master list
    for (int i = 0; i < m_buses.size(); ++i) {
        if (m_buses[i].id == busId) { m_buses.removeAt(i); break; }
    }

    refreshTable();
    refreshChart();
    statusBar()->showMessage("🗑️  Bus " + busId + " removed from fleet.");
}

// ═══════════════════════════════════════════════════════════════
//  CRUD  –  UPDATE BATTERY
// ═══════════════════════════════════════════════════════════════
void MainWindow::onUpdateBattery()
{
    int row = m_table->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "No Selection", "Please select a bus row first.");
        return;
    }

    QString busId = m_filtered[row].id;
    bool ok;
    int newBatt = QInputDialog::getInt(this, "Update Battery",
                                       "New battery % for " + busId + ":", m_filtered[row].battery, 0, 100, 1, &ok);
    if (!ok) return;

    for (Bus &b : m_buses) {
        if (b.id == busId) {
            b.battery = newBatt;
            b.status  = (newBatt == 0) ? "Retired" :
                           (newBatt < 15) ? "Charging" : b.status;
            break;
        }
    }

    refreshTable();
    refreshChart();
    statusBar()->showMessage("🔋  Battery updated for " + busId +
                             " → " + QString::number(newBatt) + "%");
}

// ═══════════════════════════════════════════════════════════════
//  SEARCH / FILTER
// ═══════════════════════════════════════════════════════════════
void MainWindow::onSearchChanged(const QString &)
{
    refreshTable();
}

void MainWindow::onFilterBattery(int)
{
    refreshTable();
}

// ═══════════════════════════════════════════════════════════════
//  EXPORT PDF  (Daily Transit Log)
// ═══════════════════════════════════════════════════════════════
void MainWindow::onExportPDF()
{
    QString path = QFileDialog::getSaveFileName(
        this, "Export Daily Transit Log", "DailyTransitLog.pdf",
        "PDF Files (*.pdf)");
    if (path.isEmpty()) return;

    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(path);
    printer.setPageSize(QPageSize(QPageSize::A4));

    QPainter painter;
    if (!painter.begin(&printer)) {
        QMessageBox::critical(this, "Error", "Could not create PDF.");
        return;
    }

    // Page metrics
    int W = painter.viewport().width();
    int margin = 160;
    int y = 200;

    // ── Header ────────────────────────────────────────────
    painter.setPen(QColor("#0ea5e9"));
    painter.setFont(QFont("Arial", 28, QFont::Bold));
    painter.drawText(margin, y, "🚌  Daily Transit Log");
    y += 80;

    painter.setPen(QColor("#64748b"));
    painter.setFont(QFont("Arial", 14));
    painter.drawText(margin, y, "Generated: " +
                                    QDateTime::currentDateTime().toString("dd/MM/yyyy  hh:mm:ss"));
    y += 40;

    painter.drawText(margin, y,
                     QString("Total buses in fleet: %1").arg(m_buses.size()));
    y += 80;

    // ── Divider ───────────────────────────────────────────
    painter.setPen(QPen(QColor("#1e2d45"), 3));
    painter.drawLine(margin, y, W - margin, y);
    y += 60;

    // ── Column headers ─────────────────────────────────────
    painter.setFont(QFont("Arial", 13, QFont::Bold));
    painter.setPen(QColor("#00d4ff"));
    int cols[7] = {margin, 350, 650, 900, 1150, 1400, 1700};
    QStringList headers = {"Bus ID","Route","Battery","Speed","Distance","Status","Position"};
    for (int c = 0; c < 7; ++c)
        painter.drawText(cols[c], y, headers[c]);
    y += 20;
    painter.setPen(QPen(QColor("#1e2d45"), 2));
    painter.drawLine(margin, y, W - margin, y);
    y += 50;

    // ── Rows ──────────────────────────────────────────────
    painter.setFont(QFont("Arial", 11));
    for (const Bus &b : m_buses) {
        if (y > painter.viewport().height() - 200) {
            printer.newPage();
            y = 200;
        }

        // Battery color
        QColor rowColor = (b.battery < 20) ? QColor("#ef4444") :
                              (b.battery < 50) ? QColor("#f59e0b") :
                              QColor("#22c55e");
        painter.setPen(QColor("#e2e8f0"));
        painter.drawText(cols[0], y, b.id);
        painter.drawText(cols[1], y, b.route.left(22));
        painter.setPen(rowColor);
        painter.drawText(cols[2], y, QString::number(b.battery) + "%");
        painter.setPen(QColor("#e2e8f0"));
        painter.drawText(cols[3], y, QString::number(b.speed, 'f', 1));
        painter.drawText(cols[4], y, QString::number(b.distanceCovered, 'f', 1));

        QColor statusColor = (b.status == "Active") ? QColor("#22c55e") :
                                 (b.status == "Charging") ? QColor("#f59e0b") :
                                 QColor("#ef4444");
        painter.setPen(statusColor);
        painter.drawText(cols[5], y, b.status);
        painter.setPen(QColor("#94a3b8"));
        painter.drawText(cols[6], y, QString("(%1,%2)")
                                         .arg(b.posX,0,'f',2).arg(b.posY,0,'f',2));
        y += 55;
    }

    // ── Summary ───────────────────────────────────────────
    y += 40;
    painter.setPen(QPen(QColor("#1e2d45"), 2));
    painter.drawLine(margin, y, W - margin, y);
    y += 50;

    int activeCnt = 0, chargCnt = 0, retCnt = 0;
    double totalDist = 0;
    for (const Bus &b : m_buses) {
        if (b.status == "Active")    ++activeCnt;
        else if (b.status == "Charging") ++chargCnt;
        else ++retCnt;
        totalDist += b.distanceCovered;
    }

    painter.setFont(QFont("Arial", 13, QFont::Bold));
    painter.setPen(QColor("#00d4ff"));
    painter.drawText(margin, y, "SUMMARY");
    y += 50;
    painter.setFont(QFont("Arial", 12));
    painter.setPen(QColor("#e2e8f0"));
    painter.drawText(margin, y, QString("Active: %1   Charging: %2   Retired: %3")
                                    .arg(activeCnt).arg(chargCnt).arg(retCnt));
    y += 45;
    painter.drawText(margin, y, QString("Total distance covered today: %1 km")
                                    .arg(totalDist, 0, 'f', 1));

    painter.end();

    QMessageBox::information(this, "PDF Exported",
                             "Daily Transit Log saved to:\n" + path);
    statusBar()->showMessage("📄 PDF exported: " + path);
}

// ═══════════════════════════════════════════════════════════════
//  BAR CHART  –  Distance per Bus
// ═══════════════════════════════════════════════════════════════
void MainWindow::refreshChart()
{
    m_chart->removeAllSeries();
    QList<QAbstractAxis*> axes = m_chart->axes();
    for (QAbstractAxis *ax : axes) m_chart->removeAxis(ax);

    QBarSet *set = new QBarSet("km covered");
    set->setColor(QColor("#0ea5e9"));
    set->setBorderColor(QColor("#38bdf8"));
    set->setLabelColor(QColor("#e2e8f0"));

    QStringList categories;
    for (const Bus &b : m_buses) {
        *set << b.distanceCovered;
        categories << b.id;
    }

    QBarSeries *series = new QBarSeries;
    series->append(set);
    series->setLabelsVisible(true);
    series->setLabelsPosition(QAbstractBarSeries::LabelsOutsideEnd);
    m_chart->addSeries(series);

    QBarCategoryAxis *axisX = new QBarCategoryAxis;
    axisX->append(categories);
    axisX->setLabelsColor(QColor("#94a3b8"));
    axisX->setGridLineColor(QColor("#1e2d45"));
    m_chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis;
    axisY->setRange(0, 1000);
    axisY->setLabelFormat("%.0f");
    axisY->setLabelsColor(QColor("#94a3b8"));
    axisY->setGridLineColor(QColor("#1e2d45"));
    m_chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    m_chart->legend()->setVisible(false);
}

// ═══════════════════════════════════════════════════════════════
//  ROUTE MAP  –  Draw bus position using QPainter
// ═══════════════════════════════════════════════════════════════
void MainWindow::updateRouteMap()
{
    // Reset to base grid
    QPixmap fresh(m_mapLabel->size());
    fresh.fill(QColor("#0d1117"));
    QPainter p(&fresh);
    p.setRenderHint(QPainter::Antialiasing);

    int W = fresh.width();
    int H = fresh.height();

    // Draw grid roads
    p.setPen(QPen(QColor("#1e2d45"), 1));
    for (int x = 0; x <= W; x += W/10) p.drawLine(x, 0, x, H);
    for (int y = 0; y <= H; y += H/5)  p.drawLine(0, y, W, y);

    // Main roads
    p.setPen(QPen(QColor("#2d3f55"), 2));
    p.drawLine(0, H/2, W, H/2);
    p.drawLine(W/2, 0, W/2, H);

    // Draw each active bus
    for (const Bus &b : m_buses) {
        if (b.status == "Retired") continue;

        int bx = static_cast<int>(b.posX * W);
        int by = static_cast<int>(b.posY * H);

        // Glow ring
        QColor glowColor = (b.battery < 20) ? QColor(239,68,68,60)
                           : (b.battery < 50) ? QColor(245,158,11,60)
                                              : QColor(0,212,255,60);
        p.setPen(Qt::NoPen);
        p.setBrush(glowColor);
        p.drawEllipse(bx-10, by-10, 20, 20);

        // Bus dot
        QColor dotColor = (b.battery < 20) ? QColor("#ef4444")
                          : (b.battery < 50) ? QColor("#f59e0b")
                                             : QColor("#00d4ff");
        p.setBrush(dotColor);
        p.setPen(QPen(Qt::white, 1));
        p.drawEllipse(bx-5, by-5, 10, 10);

        // Label
        p.setPen(Qt::white);
        p.setFont(QFont("Arial", 6, QFont::Bold));
        p.drawText(bx+7, by-5, b.id);
    }

    // Legend
    p.setFont(QFont("Arial", 7));
    p.setPen(QColor("#00d4ff")); p.drawText(5, H-32, "● Active");
    p.setPen(QColor("#f59e0b")); p.drawText(55, H-32, "● <50%");
    p.setPen(QColor("#ef4444")); p.drawText(100, H-32, "● Critical");
    p.setPen(QColor("#64748b")); p.drawText(5, H-18, "Real-time GPS position");

    p.end();
    m_mapLabel->setPixmap(fresh);
}

// ═══════════════════════════════════════════════════════════════
//  TIMER TICK  –  Simulate live sensor data (1 Hz)
// ═══════════════════════════════════════════════════════════════
void MainWindow::onTimerTick()
{
    // Simulate speed + obstacle from active buses
    bool anyActive = false;
    for (Bus &b : m_buses) {
        if (b.status != "Active") continue;
        anyActive = true;

        // Simulate small speed/position changes
        double delta = (QRandomGenerator::global()->generateDouble() - 0.5) * 5.0;
        b.speed = qMax(0.0, qMin(80.0, b.speed + delta));

        // Simulate distance accumulation
        b.distanceCovered += b.speed / 3600.0; // km per second (approximate)

        // Simulate slow battery drain
        if (QRandomGenerator::global()->bounded(100) < 3)
            b.battery = qMax(0, b.battery - 1);

        // Drift position slightly
        b.posX = qMax(0.05, qMin(0.95, b.posX + (QRandomGenerator::global()->generateDouble()-0.5)*0.02));
        b.posY = qMax(0.05, qMin(0.95, b.posY + (QRandomGenerator::global()->generateDouble()-0.5)*0.02));
    }

    // Pick the selected bus for the dial, or first active
    Bus *displayBus = nullptr;
    if (m_selectedRow >= 0 && m_selectedRow < m_filtered.size()) {
        QString id = m_filtered[m_selectedRow].id;
        for (Bus &b : m_buses)
            if (b.id == id) { displayBus = &b; break; }
    }
    if (!displayBus && !m_buses.isEmpty()) {
        for (Bus &b : m_buses)
            if (b.status == "Active") { displayBus = &b; break; }
    }

    if (displayBus) {
        m_currentSpeed = displayBus->speed;
        m_speedDial->setValue(static_cast<int>(m_currentSpeed));
        m_speedLabel->setText(QString("Speed: %1 km/h").arg(m_currentSpeed, 0, 'f', 1));
        m_battBar->setText(QString("%1  |  Battery: %2%")
                               .arg(displayBus->id).arg(displayBus->battery));

        // Simulate obstacle detection
        m_obstacleDetected = (QRandomGenerator::global()->bounded(100) < 8);
        if (m_obstacleDetected) {
            m_obstacleLabel->setText("Obstacle: DETECTED ⚠️");
            m_obstacleLabel->setStyleSheet("font-size:14px;font-weight:700;color:#ef4444;");
        } else {
            m_obstacleLabel->setText("Obstacle: CLEAR ✅");
            m_obstacleLabel->setStyleSheet("font-size:14px;font-weight:700;color:#22c55e;");
        }
    }

    // Refresh table and map every tick
    refreshTable();
    updateRouteMap();
}

// ═══════════════════════════════════════════════════════════════
//  ARDUINO SERIAL DATA
// ═══════════════════════════════════════════════════════════════
void MainWindow::onSerialDataReady()
{
    if (!m_serial) return;
    QByteArray data = m_serial->readAll();
    // Expected format from Arduino: "SPEED:45.2,OBS:0\n"
    QString line = QString::fromUtf8(data).trimmed();
    QStringList parts = line.split(',');
    for (const QString &part : parts) {
        QStringList kv = part.split(':');
        if (kv.size() < 2) continue;
        if (kv[0] == "SPEED") {
            m_currentSpeed = kv[1].toDouble();
            m_speedDial->setValue(static_cast<int>(m_currentSpeed));
            m_speedLabel->setText(QString("Speed: %1 km/h").arg(m_currentSpeed, 0, 'f', 1));
        } else if (kv[0] == "OBS") {
            bool obs = kv[1].trimmed() == "1";
            if (obs) {
                m_obstacleLabel->setText("Obstacle: DETECTED ⚠️");
                m_obstacleLabel->setStyleSheet("font-size:14px;font-weight:700;color:#ef4444;");
            } else {
                m_obstacleLabel->setText("Obstacle: CLEAR ✅");
                m_obstacleLabel->setStyleSheet("font-size:14px;font-weight:700;color:#22c55e;");
            }
        }
    }
}

// ═══════════════════════════════════════════════════════════════
//  TABLE SELECTION
// ═══════════════════════════════════════════════════════════════
void MainWindow::onTableSelectionChanged()
{
    m_selectedRow = m_table->currentRow();
    if (m_selectedRow >= 0 && m_selectedRow < m_filtered.size()) {
        const Bus &b = m_filtered[m_selectedRow];
        m_battBar->setText(QString("%1  |  Battery: %2%  |  Route: %3")
                               .arg(b.id).arg(b.battery).arg(b.route));
        updateRouteMap();
    }
}
