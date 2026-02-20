#include "pedestriansafetymodule.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFrame>
#include <QHeaderView>
#include <QGroupBox>
#include <QRandomGenerator>
#include <QScrollArea>
#include <QDateTime>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>

PedestrianSafetyModule::PedestrianSafetyModule(QWidget *parent)
    : QWidget(parent)
    , m_totalAlerts(12)
    , m_totalViolations(7)
    , m_currentRiskLevel("Medium")
    , m_activeCrosswalks(18)
{
    setupUI();
    applyStyles();
    
    // Initialize alert history (last 7 days)
    m_alertHistory = {8, 5, 10, 12, 9, 15, 12};
    
    // Setup auto-update timer
    m_updateTimer = new QTimer(this);
    connect(m_updateTimer, &QTimer::timeout, this, &PedestrianSafetyModule::updateSafetyData);
    m_updateTimer->start(6000); // Update every 6 seconds
    
    // Initial data population
    updateSafetyData();
}

PedestrianSafetyModule::~PedestrianSafetyModule()
{
}

void PedestrianSafetyModule::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(25);
    mainLayout->setContentsMargins(30, 30, 30, 30);
    
    // Title Header Frame
    QFrame *titleFrame = new QFrame();
    titleFrame->setStyleSheet("QFrame { background-color: " + COLOR_PANEL + "; border-radius: 8px; padding: 15px; }");
    QHBoxLayout *titleFrameLayout = new QHBoxLayout(titleFrame);
    titleFrameLayout->setContentsMargins(20, 15, 20, 15);
    
    QLabel *titleLabel = new QLabel("🚸 Pedestrian Safety & Monitoring System");
    titleLabel->setStyleSheet("font-size: 28px; font-weight: bold; color: " + COLOR_ACCENT + "; background: transparent;");
    titleFrameLayout->addWidget(titleLabel);
    titleFrameLayout->addStretch();
    
    mainLayout->addWidget(titleFrame);
    
    // Create scroll area
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setStyleSheet("QScrollArea { background-color: " + COLOR_BACKGROUND + "; border: none; }");
    
    QWidget *scrollContent = new QWidget();
    QVBoxLayout *contentLayout = new QVBoxLayout(scrollContent);
    contentLayout->setSpacing(20);
    contentLayout->setContentsMargins(0, 0, 10, 0);
    
    // KPI Section
    createKPISection();
    contentLayout->addWidget(m_totalAlertsLabel->parentWidget());
    
    // Main content grid: Chart + Crosswalk Status
    QWidget *mainContentWidget = new QWidget();
    QHBoxLayout *mainContentLayout = new QHBoxLayout(mainContentWidget);
    mainContentLayout->setSpacing(20);
    mainContentLayout->setContentsMargins(0, 0, 0, 0);
    
    // Left: Alerts Chart
    createAlertsChart();
    mainContentLayout->addWidget(m_alertsChartView->parentWidget(), 1);
    
    // Right: Crosswalk Status
    createCrosswalkStatusTable();
    mainContentLayout->addWidget(m_crosswalkStatusTable->parentWidget(), 1);
    
    contentLayout->addWidget(mainContentWidget);
    
    // Violations Table
    createViolationsTable();
    contentLayout->addWidget(m_violationsTable->parentWidget());
    
    // Control Buttons
    QWidget *controlWidget = new QWidget();
    QHBoxLayout *controlLayout = new QHBoxLayout(controlWidget);
    controlLayout->setSpacing(15);
    
    m_refreshBtn = new QPushButton("🔄 Refresh Data");
    m_exportBtn = new QPushButton("📊 Export Violations");
    m_dispatchBtn = new QPushButton("🚔 Dispatch Patrol");
    m_simulateBtn = new QPushButton("⚡ Simulate Violation");
    
    connect(m_refreshBtn, &QPushButton::clicked, this, &PedestrianSafetyModule::onRefreshData);
    connect(m_exportBtn, &QPushButton::clicked, this, &PedestrianSafetyModule::onExportViolations);
    connect(m_dispatchBtn, &QPushButton::clicked, this, &PedestrianSafetyModule::onDispatchPatrol);
    connect(m_simulateBtn, &QPushButton::clicked, this, &PedestrianSafetyModule::onSimulateViolation);
    
    m_refreshBtn->setStyleSheet(getButtonStyle(COLOR_ACCENT));
    m_exportBtn->setStyleSheet(getButtonStyle(COLOR_SUCCESS));
    m_dispatchBtn->setStyleSheet(getButtonStyle(COLOR_WARNING));
    m_simulateBtn->setStyleSheet(getButtonStyle(COLOR_CRITICAL));
    
    m_refreshBtn->setMinimumHeight(45);
    m_exportBtn->setMinimumHeight(45);
    m_dispatchBtn->setMinimumHeight(45);
    m_simulateBtn->setMinimumHeight(45);
    
    controlLayout->addWidget(m_refreshBtn);
    controlLayout->addWidget(m_exportBtn);
    controlLayout->addWidget(m_dispatchBtn);
    controlLayout->addWidget(m_simulateBtn);
    
    contentLayout->addWidget(controlWidget);
    
    scrollArea->setWidget(scrollContent);
    mainLayout->addWidget(scrollArea);
}

void PedestrianSafetyModule::createKPISection()
{
    QFrame *kpiCard = new QFrame();
    kpiCard->setStyleSheet(getCardStyle());
    kpiCard->setFixedHeight(180);
    
    QHBoxLayout *kpiLayout = new QHBoxLayout(kpiCard);
    kpiLayout->setSpacing(30);
    kpiLayout->setContentsMargins(35, 25, 35, 25);
    
    // Total Alerts Today - Enhanced
    QWidget *alertsWidget = new QWidget();
    alertsWidget->setFixedHeight(130);
    QVBoxLayout *alertsLayout = new QVBoxLayout(alertsWidget);
    alertsLayout->setSpacing(8);
    alertsLayout->setContentsMargins(0, 0, 0, 0);
    
    QLabel *alertIcon = new QLabel("🚸");
    alertIcon->setStyleSheet("font-size: 32px;");
    alertIcon->setAlignment(Qt::AlignLeft);
    alertIcon->setFixedHeight(35);
    
    QLabel *alertsTitle = new QLabel("PEDESTRIAN ALERTS TODAY");
    alertsTitle->setStyleSheet("font-size: 11px; color: " + COLOR_TEXT + "; font-weight: bold; letter-spacing: 1px;");
    alertsTitle->setAlignment(Qt::AlignLeft);
    alertsTitle->setFixedHeight(18);
    
    m_totalAlertsLabel = new QLabel("12");
    m_totalAlertsLabel->setStyleSheet("font-size: 46px; font-weight: bold; color: " + COLOR_WARNING + "; margin-top: 5px;");
    m_totalAlertsLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_totalAlertsLabel->setFixedHeight(55);
    
    QLabel *alertsSubtext = new QLabel("Active Alerts");
    alertsSubtext->setStyleSheet("font-size: 11px; color: " + COLOR_TEXT_DIM + ";");
    alertsSubtext->setAlignment(Qt::AlignLeft);
    
    alertsLayout->addWidget(alertIcon);
    alertsLayout->addWidget(alertsTitle);
    alertsLayout->addWidget(m_totalAlertsLabel);
    alertsLayout->addWidget(alertsSubtext);
    kpiLayout->addWidget(alertsWidget, 1);
    
    // Speed Violations - Enhanced
    QWidget *violationsWidget = new QWidget();
    violationsWidget->setFixedHeight(130);
    QVBoxLayout *violationsLayout = new QVBoxLayout(violationsWidget);
    violationsLayout->setSpacing(8);
    violationsLayout->setContentsMargins(0, 0, 0, 0);
    
    QLabel *violIcon = new QLabel("⚠️");
    violIcon->setStyleSheet("font-size: 32px;");
    violIcon->setAlignment(Qt::AlignLeft);
    violIcon->setFixedHeight(35);
    
    QLabel *violationsTitle = new QLabel("SPEED VIOLATIONS");
    violationsTitle->setStyleSheet("font-size: 11px; color: " + COLOR_TEXT + "; font-weight: bold; letter-spacing: 1px;");
    violationsTitle->setAlignment(Qt::AlignLeft);
    violationsTitle->setFixedHeight(18);
    
    violationsTitle->setFixedHeight(18);
    
    m_violationsLabel = new QLabel("7");
    m_violationsLabel->setStyleSheet("font-size: 46px; font-weight: bold; color: " + COLOR_CRITICAL + "; margin-top: 5px;");
    m_violationsLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_violationsLabel->setFixedHeight(55);
    
    QLabel *violationsSubtext = new QLabel("Today");
    violationsSubtext->setStyleSheet("font-size: 11px; color: " + COLOR_TEXT_DIM + ";");
    violationsSubtext->setAlignment(Qt::AlignLeft);
    
    violationsLayout->addWidget(violIcon);
    violationsLayout->addWidget(violationsTitle);
    violationsLayout->addWidget(m_violationsLabel);
    violationsLayout->addWidget(violationsSubtext);
    kpiLayout->addWidget(violationsWidget, 1);
    
    // Risk Level - Enhanced
    QWidget *riskWidget = new QWidget();
    riskWidget->setFixedHeight(130);
    QVBoxLayout *riskLayout = new QVBoxLayout(riskWidget);
    riskLayout->setSpacing(6);
    riskLayout->setContentsMargins(0, 0, 0, 0);
    
    QLabel *riskIcon = new QLabel("🛡️");
    riskIcon->setStyleSheet("font-size: 32px;");
    riskIcon->setAlignment(Qt::AlignLeft);
    riskIcon->setFixedHeight(35);
    
    QLabel *riskTitle = new QLabel("PEDESTRIAN RISK LEVEL");
    riskTitle->setStyleSheet("font-size: 11px; color: " + COLOR_TEXT + "; font-weight: bold; letter-spacing: 1px;");
    riskTitle->setAlignment(Qt::AlignLeft);
    riskTitle->setFixedHeight(16);
    
    m_riskLevelLabel = new QLabel("MEDIUM");
    m_riskLevelLabel->setStyleSheet("font-size: 28px; font-weight: bold; color: " + COLOR_WARNING + "; margin-top: 5px;");
    m_riskLevelLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_riskLevelLabel->setFixedHeight(40);
    
    m_riskBar = new QProgressBar();
    m_riskBar->setRange(0, 100);
    m_riskBar->setValue(55);
    m_riskBar->setTextVisible(false);
    m_riskBar->setFixedHeight(18);
    m_riskBar->setStyleSheet(
        "QProgressBar {"
        "    border: 2px solid " + COLOR_BORDER + ";"
        "    border-radius: 5px;"
        "    background-color: " + COLOR_PANEL + ";"
        "}"
        "QProgressBar::chunk {"
        "    background-color: " + COLOR_WARNING + ";"
        "    border-radius: 3px;"
        "}"
    );
    
    riskLayout->addWidget(riskIcon);
    riskLayout->addWidget(riskTitle);
    riskLayout->addWidget(m_riskLevelLabel);
    riskLayout->addWidget(m_riskBar);
    kpiLayout->addWidget(riskWidget, 1);
    
    // Active Crosswalks - Enhanced
    QWidget *crosswalksWidget = new QWidget();
    crosswalksWidget->setFixedHeight(130);
    QVBoxLayout *crosswalksLayout = new QVBoxLayout(crosswalksWidget);
    crosswalksLayout->setSpacing(8);
    crosswalksLayout->setContentsMargins(0, 0, 0, 0);
    
    QLabel *crossIcon = new QLabel("🚶");
    crossIcon->setStyleSheet("font-size: 32px;");
    crossIcon->setAlignment(Qt::AlignLeft);
    crossIcon->setFixedHeight(35);
    
    QLabel *crosswalksTitle = new QLabel("ACTIVE CROSSWALKS");
    crosswalksTitle->setStyleSheet("font-size: 11px; color: " + COLOR_TEXT + "; font-weight: bold; letter-spacing: 1px;");
    crosswalksTitle->setAlignment(Qt::AlignLeft);
    crosswalksTitle->setFixedHeight(18);
    
    crosswalksTitle->setFixedHeight(18);
    
    m_activeCrosswalksLabel = new QLabel("18");
    m_activeCrosswalksLabel->setStyleSheet("font-size: 46px; font-weight: bold; color: " + COLOR_SUCCESS + "; margin-top: 5px;");
    m_activeCrosswalksLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_activeCrosswalksLabel->setFixedHeight(55);
    
    QLabel *crosswalksSubtext = new QLabel("Operational");
    crosswalksSubtext->setStyleSheet("font-size: 11px; color: " + COLOR_TEXT_DIM + ";");
    crosswalksSubtext->setAlignment(Qt::AlignLeft);
    
    crosswalksLayout->addWidget(crossIcon);
    crosswalksLayout->addWidget(crosswalksTitle);
    crosswalksLayout->addWidget(m_activeCrosswalksLabel);
    crosswalksLayout->addWidget(crosswalksSubtext);
    kpiLayout->addWidget(crosswalksWidget, 1);
}

void PedestrianSafetyModule::createAlertsChart()
{
    QFrame *chartCard = new QFrame();
    chartCard->setStyleSheet(getCardStyle());
    
    QVBoxLayout *chartLayout = new QVBoxLayout(chartCard);
    chartLayout->setContentsMargins(20, 20, 20, 20);
    
    QLabel *chartTitle = new QLabel("📊 Pedestrian Safety Alerts (7 Days)");
    chartTitle->setStyleSheet("font-size: 16px; font-weight: bold; color: " + COLOR_TEXT + ";");
    chartLayout->addWidget(chartTitle);
    
    // Create bar chart
    QBarSet *alertSet = new QBarSet("Alerts");
    for (int count : m_alertHistory) {
        *alertSet << count;
    }
    alertSet->setColor(QColor(COLOR_WARNING));
    
    m_alertsSeries = new QBarSeries();
    m_alertsSeries->append(alertSet);
    
    m_alertsChart = new QChart();
    m_alertsChart->addSeries(m_alertsSeries);
    m_alertsChart->setTitle("");
    m_alertsChart->setAnimationOptions(QChart::SeriesAnimations);
    m_alertsChart->setBackgroundBrush(QBrush(QColor(COLOR_PANEL)));
    m_alertsChart->setTitleBrush(QBrush(QColor(COLOR_TEXT)));
    
    QStringList categories;
    categories << "Mon" << "Tue" << "Wed" << "Thu" << "Fri" << "Sat" << "Sun";
    
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    axisX->setLabelsColor(QColor(COLOR_TEXT));
    m_alertsChart->addAxis(axisX, Qt::AlignBottom);
    m_alertsSeries->attachAxis(axisX);
    
    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0, 20);
    axisY->setLabelsColor(QColor(COLOR_TEXT));
    m_alertsChart->addAxis(axisY, Qt::AlignLeft);
    m_alertsSeries->attachAxis(axisY);
    
    m_alertsChart->legend()->setVisible(false);
    
    m_alertsChartView = new QChartView(m_alertsChart);
    m_alertsChartView->setRenderHint(QPainter::Antialiasing);
    m_alertsChartView->setMinimumHeight(300);
    
    chartLayout->addWidget(m_alertsChartView);
}

void PedestrianSafetyModule::createCrosswalkStatusTable()
{
    QFrame *tableCard = new QFrame();
    tableCard->setStyleSheet(getCardStyle());
    
    QVBoxLayout *tableLayout = new QVBoxLayout(tableCard);
    tableLayout->setContentsMargins(20, 20, 20, 20);
    
    QLabel *tableTitle = new QLabel("🚦 Crosswalk Status Monitor");
    tableTitle->setStyleSheet("font-size: 16px; font-weight: bold; color: " + COLOR_TEXT + ";");
    tableLayout->addWidget(tableTitle);
    
    m_crosswalkStatusTable = new QTableWidget();
    m_crosswalkStatusTable->setColumnCount(4);
    m_crosswalkStatusTable->setHorizontalHeaderLabels({"Crosswalk ID", "Location", "Risk Level", "Status"});
    m_crosswalkStatusTable->horizontalHeader()->setStretchLastSection(true);
    m_crosswalkStatusTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_crosswalkStatusTable->verticalHeader()->setVisible(false);
    m_crosswalkStatusTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_crosswalkStatusTable->setSelectionMode(QAbstractItemView::SingleSelection);
    m_crosswalkStatusTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_crosswalkStatusTable->setStyleSheet(getTableStyle());
    m_crosswalkStatusTable->setMinimumHeight(280);
    
    // Populate with sample data
    updateCrosswalkStatus("CW-001", "Main St & 1st Ave", "Low", "Operational");
    updateCrosswalkStatus("CW-002", "Central Plaza", "Medium", "Operational");
    updateCrosswalkStatus("CW-003", "School Zone", "High", "Critical");
    updateCrosswalkStatus("CW-004", "Shopping District", "Low", "Operational");
    updateCrosswalkStatus("CW-005", "University Campus", "Medium", "Operational");
    updateCrosswalkStatus("CW-006", "Hospital Area", "Medium", "Operational");
    updateCrosswalkStatus("CW-007", "Industrial Park", "High", "Critical");
    updateCrosswalkStatus("CW-008", "Residential Zone", "Low", "Operational");
    
    tableLayout->addWidget(m_crosswalkStatusTable);
}

void PedestrianSafetyModule::createViolationsTable()
{
    QFrame *tableCard = new QFrame();
    tableCard->setStyleSheet(getCardStyle());
    
    QVBoxLayout *tableLayout = new QVBoxLayout(tableCard);
    tableLayout->setContentsMargins(20, 20, 20, 20);
    
    QLabel *tableTitle = new QLabel("⚠️ Speed Violations Log");
    tableTitle->setStyleSheet("font-size: 16px; font-weight: bold; color: " + COLOR_TEXT + ";");
    tableLayout->addWidget(tableTitle);
    
    m_violationsTable = new QTableWidget();
    m_violationsTable->setColumnCount(4);
    m_violationsTable->setHorizontalHeaderLabels({"Crosswalk ID", "Vehicle ID", "Speed (km/h)", "Timestamp"});
    m_violationsTable->horizontalHeader()->setStretchLastSection(true);
    m_violationsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_violationsTable->verticalHeader()->setVisible(false);
    m_violationsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_violationsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    m_violationsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_violationsTable->setStyleSheet(getTableStyle());
    m_violationsTable->setMinimumHeight(250);
    
    // Populate with sample violations
    QDateTime now = QDateTime::currentDateTime();
    addViolation("CW-003", "VEH-7892", 65, now.addSecs(-3600).toString("hh:mm:ss"));
    addViolation("CW-007", "VEH-4521", 72, now.addSecs(-2400).toString("hh:mm:ss"));
    addViolation("CW-002", "VEH-8934", 58, now.addSecs(-1800).toString("hh:mm:ss"));
    addViolation("CW-003", "VEH-2156", 68, now.addSecs(-1200).toString("hh:mm:ss"));
    addViolation("CW-007", "VEH-6789", 75, now.addSecs(-600).toString("hh:mm:ss"));
    
    tableLayout->addWidget(m_violationsTable);
}

void PedestrianSafetyModule::updateSafetyData()
{
    // Randomly generate new alerts
    if (QRandomGenerator::global()->bounded(100) < 15) { // 15% chance
        m_totalAlerts++;
        m_totalAlertsLabel->setText(QString::number(m_totalAlerts));
        
        // Update alert history
        m_alertHistory.removeLast();
        m_alertHistory.prepend(m_totalAlerts);
        
        // Update chart
        m_alertsSeries->clear();
        QBarSet *alertSet = new QBarSet("Alerts");
        for (int count : m_alertHistory) {
            *alertSet << count;
        }
        alertSet->setColor(QColor(COLOR_WARNING));
        m_alertsSeries->append(alertSet);
    }
    
    // Update risk level
    updateRiskLevel();
}

void PedestrianSafetyModule::updateCrosswalkStatus(const QString &crosswalkId, const QString &location,
                                                   const QString &riskLevel, const QString &status)
{
    int row = m_crosswalkStatusTable->rowCount();
    m_crosswalkStatusTable->insertRow(row);
    
    m_crosswalkStatusTable->setItem(row, 0, new QTableWidgetItem(crosswalkId));
    m_crosswalkStatusTable->setItem(row, 1, new QTableWidgetItem(location));
    m_crosswalkStatusTable->setItem(row, 2, new QTableWidgetItem(riskLevel));
    m_crosswalkStatusTable->setItem(row, 3, new QTableWidgetItem(status));
    
    // Color-code risk level
    QString riskColor = getRiskColor(riskLevel);
    m_crosswalkStatusTable->item(row, 2)->setForeground(QBrush(QColor(riskColor)));
    
    // Color-code status
    if (status == "Critical") {
        m_crosswalkStatusTable->item(row, 3)->setForeground(QBrush(QColor(COLOR_CRITICAL)));
    } else {
        m_crosswalkStatusTable->item(row, 3)->setForeground(QBrush(QColor(COLOR_SUCCESS)));
    }
}

void PedestrianSafetyModule::addViolation(const QString &crosswalkId, const QString &vehicleId,
                                          int speed, const QString &timestamp)
{
    int row = m_violationsTable->rowCount();
    m_violationsTable->insertRow(row);
    
    m_violationsTable->setItem(row, 0, new QTableWidgetItem(crosswalkId));
    m_violationsTable->setItem(row, 1, new QTableWidgetItem(vehicleId));
    m_violationsTable->setItem(row, 2, new QTableWidgetItem(QString::number(speed)));
    m_violationsTable->setItem(row, 3, new QTableWidgetItem(timestamp));
    
    // Highlight severe violations (>70 km/h)
    if (speed > 70) {
        m_violationsTable->item(row, 2)->setForeground(QBrush(QColor(COLOR_CRITICAL)));
    } else if (speed > 60) {
        m_violationsTable->item(row, 2)->setForeground(QBrush(QColor(COLOR_WARNING)));
    }
}

void PedestrianSafetyModule::updateRiskLevel()
{
    // Calculate risk based on alerts and violations
    int riskScore = (m_totalAlerts * 4) + (m_totalViolations * 6);
    
    QString level;
    QString color;
    if (riskScore < 40) {
        level = "LOW";
        color = COLOR_SUCCESS;
        m_currentRiskLevel = "Low";
    } else if (riskScore < 80) {
        level = "MEDIUM";
        color = COLOR_WARNING;
        m_currentRiskLevel = "Medium";
    } else {
        level = "HIGH";
        color = COLOR_CRITICAL;
        m_currentRiskLevel = "High";
    }
    
    m_riskLevelLabel->setText(level);
    m_riskLevelLabel->setStyleSheet("font-size: 28px; font-weight: bold; color: " + color + ";");
    m_riskBar->setValue(qMin(100, riskScore));
    m_riskBar->setStyleSheet(
        "QProgressBar {"
        "    border: 2px solid " + COLOR_BORDER + ";"
        "    border-radius: 5px;"
        "    background-color: " + COLOR_PANEL + ";"
        "    height: 20px;"
        "}"
        "QProgressBar::chunk {"
        "    background-color: " + color + ";"
        "    border-radius: 3px;"
        "}"
    );
    
    emit riskLevelChanged(m_currentRiskLevel);
}

QString PedestrianSafetyModule::getRiskColor(const QString &riskLevel)
{
    if (riskLevel == "High") return COLOR_CRITICAL;
    if (riskLevel == "Medium") return COLOR_WARNING;
    return COLOR_SUCCESS;
}

void PedestrianSafetyModule::onRefreshData()
{
    updateSafetyData();
    addLogMessage("Safety data refreshed successfully");
}

void PedestrianSafetyModule::onExportViolations()
{
    addLogMessage("Exporting violations report... [PDF/Excel format]");
}

void PedestrianSafetyModule::onDispatchPatrol()
{
    addLogMessage("🚔 Patrol units dispatched to high-risk crosswalks");
    // Find critical crosswalks
    for (int i = 0; i < m_crosswalkStatusTable->rowCount(); ++i) {
        QString riskLevel = m_crosswalkStatusTable->item(i, 2)->text();
        if (riskLevel == "High") {
            QString crosswalkId = m_crosswalkStatusTable->item(i, 0)->text();
            addLogMessage("Patrol assigned to " + crosswalkId);
        }
    }
}

void PedestrianSafetyModule::onSimulateViolation()
{
    // Generate random violation
    QStringList crosswalks = {"CW-001", "CW-002", "CW-003", "CW-007"};
    QString randomCrosswalk = crosswalks[QRandomGenerator::global()->bounded(crosswalks.size())];
    QString vehicleId = QString("VEH-%1").arg(QRandomGenerator::global()->bounded(1000, 9999));
    int speed = QRandomGenerator::global()->bounded(55, 80);
    QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");
    
    addViolation(randomCrosswalk, vehicleId, speed, timestamp);
    m_totalViolations++;
    m_violationsLabel->setText(QString::number(m_totalViolations));
    
    addLogMessage("⚠️ VIOLATION: " + vehicleId + " exceeded speed at " + randomCrosswalk + " (" + QString::number(speed) + " km/h)");
    emit violationDetected(vehicleId, speed);
    
    updateRiskLevel();
}

void PedestrianSafetyModule::addLogMessage(const QString &message)
{
    qDebug() << "[PEDESTRIAN SAFETY]" << message;
}

QString PedestrianSafetyModule::getCardStyle()
{
    return QString(
        "QFrame {"
        "    background-color: %1;"
        "    border: 1px solid %2;"
        "    border-radius: 8px;"
        "    padding: 15px;"
        "}"
    ).arg(COLOR_PANEL, COLOR_BORDER);
}

QString PedestrianSafetyModule::getTableStyle()
{
    return QString(
        "QTableWidget {"
        "    background-color: %1;"
        "    border: 1px solid %2;"
        "    border-radius: 5px;"
        "    color: %3;"
        "    gridline-color: %2;"
        "}"
        "QTableWidget::item {"
        "    padding: 8px;"
        "    border-bottom: 1px solid %2;"
        "}"
        "QTableWidget::item:selected {"
        "    background-color: %4;"
        "    color: white;"
        "}"
        "QHeaderView::section {"
        "    background-color: %1;"
        "    color: %5;"
        "    padding: 10px;"
        "    border: none;"
        "    border-bottom: 2px solid %4;"
        "    font-weight: bold;"
        "    font-size: 13px;"
        "}"
    ).arg(COLOR_PANEL, COLOR_BORDER, COLOR_TEXT, COLOR_ACCENT, COLOR_TEXT_DIM);
}

QString PedestrianSafetyModule::getButtonStyle(const QString &color)
{
    return QString(
        "QPushButton {"
        "    background-color: %1;"
        "    color: white;"
        "    border: none;"
        "    border-radius: 6px;"
        "    padding: 12px 24px;"
        "    font-size: 14px;"
        "    font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "    background-color: %2;"
        "}"
        "QPushButton:pressed {"
        "    background-color: %3;"
        "}"
    ).arg(color, color + "DD", color + "AA");
}

void PedestrianSafetyModule::applyStyles()
{
    setStyleSheet("background-color: " + COLOR_BACKGROUND + ";");
}
