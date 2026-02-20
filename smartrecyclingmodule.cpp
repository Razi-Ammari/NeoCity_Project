#include "smartrecyclingmodule.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFrame>
#include <QHeaderView>
#include <QGroupBox>
#include <QRandomGenerator>
#include <QScrollArea>
#include <QtCharts/QPieSlice>
#include <QDateTime>

SmartRecyclingModule::SmartRecyclingModule(QWidget *parent)
    : QWidget(parent)
    , m_totalRecycled(847.0)
    , m_plasticTotal(320.0)
    , m_metalTotal(285.0)
    , m_glassTotal(242.0)
    , m_activeBins(12)
{
    setupUI();
    applyStyles();
    
    // Setup auto-update timer
    m_updateTimer = new QTimer(this);
    connect(m_updateTimer, &QTimer::timeout, this, &SmartRecyclingModule::updateRecyclingData);
    m_updateTimer->start(8000); // Update every 8 seconds
    
    // Initial data population
    updateRecyclingData();
}

SmartRecyclingModule::~SmartRecyclingModule()
{
}

void SmartRecyclingModule::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(25);
    mainLayout->setContentsMargins(30, 30, 30, 30);
    
    // Title Header Frame
    QFrame *titleFrame = new QFrame();
    titleFrame->setStyleSheet("QFrame { background-color: " + COLOR_PANEL + "; border-radius: 8px; padding: 15px; }");
    QHBoxLayout *titleFrameLayout = new QHBoxLayout(titleFrame);
    titleFrameLayout->setContentsMargins(20, 15, 20, 15);
    
    QLabel *titleLabel = new QLabel("♻️ Smart Recycling Management System");
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
    contentLayout->addWidget(m_totalRecycledLabel->parentWidget());
    
    // Main content grid: Chart + Tables
    QWidget *mainContentWidget = new QWidget();
    QHBoxLayout *mainContentLayout = new QHBoxLayout(mainContentWidget);
    mainContentLayout->setSpacing(20);
    mainContentLayout->setContentsMargins(0, 0, 0, 0);
    
    // Left: Waste Types Chart
    createWasteTypesChart();
    mainContentLayout->addWidget(m_wasteChartView->parentWidget(), 1);
    
    // Right: Bin Status Table
    createBinStatusTable();
    mainContentLayout->addWidget(m_binStatusTable->parentWidget(), 1);
    
    contentLayout->addWidget(mainContentWidget);
    
    // Citizen Rewards Table
    createCitizenRewardsTable();
    contentLayout->addWidget(m_citizenRewardsTable->parentWidget());
    
    // Control Buttons
    QWidget *controlWidget = new QWidget();
    QHBoxLayout *controlLayout = new QHBoxLayout(controlWidget);
    controlLayout->setSpacing(15);
    
    m_refreshBtn = new QPushButton("🔄 Refresh Data");
    m_exportBtn = new QPushButton("📊 Export Report");
    m_notifyBtn = new QPushButton("📢 Notify Collection");
    m_simulateBtn = new QPushButton("⚡ Simulate Bin Full");
    
    connect(m_refreshBtn, &QPushButton::clicked, this, &SmartRecyclingModule::onRefreshData);
    connect(m_exportBtn, &QPushButton::clicked, this, &SmartRecyclingModule::onExportReport);
    connect(m_notifyBtn, &QPushButton::clicked, this, &SmartRecyclingModule::onNotifyCollection);
    connect(m_simulateBtn, &QPushButton::clicked, this, &SmartRecyclingModule::onSimulateBinFull);
    
    m_refreshBtn->setStyleSheet(getButtonStyle(COLOR_ACCENT));
    m_exportBtn->setStyleSheet(getButtonStyle(COLOR_SUCCESS));
    m_notifyBtn->setStyleSheet(getButtonStyle(COLOR_WARNING));
    m_simulateBtn->setStyleSheet(getButtonStyle(COLOR_CRITICAL));
    
    m_refreshBtn->setMinimumHeight(45);
    m_exportBtn->setMinimumHeight(45);
    m_notifyBtn->setMinimumHeight(45);
    m_simulateBtn->setMinimumHeight(45);
    
    controlLayout->addWidget(m_refreshBtn);
    controlLayout->addWidget(m_exportBtn);
    controlLayout->addWidget(m_notifyBtn);
    controlLayout->addWidget(m_simulateBtn);
    
    contentLayout->addWidget(controlWidget);
    
    scrollArea->setWidget(scrollContent);
    mainLayout->addWidget(scrollArea);
}

void SmartRecyclingModule::createKPISection()
{
    QFrame *kpiCard = new QFrame();
    kpiCard->setStyleSheet(getCardStyle());
    kpiCard->setFixedHeight(180);
    
    QHBoxLayout *kpiLayout = new QHBoxLayout(kpiCard);
    kpiLayout->setSpacing(40);
    kpiLayout->setContentsMargins(35, 25, 35, 25);
    
    // Total Recycled Today - Enhanced visibility
    QWidget *totalWidget = new QWidget();
    totalWidget->setFixedHeight(130);
    QVBoxLayout *totalLayout = new QVBoxLayout(totalWidget);
    totalLayout->setSpacing(8);
    totalLayout->setContentsMargins(0, 0, 0, 0);
    
    QLabel *totalIcon = new QLabel("♻️");
    totalIcon->setStyleSheet("font-size: 32px;");
    totalIcon->setAlignment(Qt::AlignLeft);
    totalIcon->setFixedHeight(35);
    
    QLabel *totalTitle = new QLabel("TOTAL RECYCLED TODAY");
    totalTitle->setStyleSheet("font-size: 12px; color: " + COLOR_TEXT + "; font-weight: bold; letter-spacing: 1px;");
    totalTitle->setAlignment(Qt::AlignLeft);
    totalTitle->setFixedHeight(18);
    
    m_totalRecycledLabel = new QLabel("847 kg");
    m_totalRecycledLabel->setStyleSheet("font-size: 42px; font-weight: bold; color: " + COLOR_SUCCESS + "; margin-top: 5px;");
    m_totalRecycledLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_totalRecycledLabel->setFixedHeight(55);
    
    totalLayout->addWidget(totalIcon);
    totalLayout->addWidget(totalTitle);
    totalLayout->addWidget(m_totalRecycledLabel);
    totalLayout->addStretch();
    kpiLayout->addWidget(totalWidget, 2);
    
    // Waste Type Breakdown
    QGridLayout *wasteGrid = new QGridLayout();
    wasteGrid->setSpacing(15);
    
    QLabel *plasticIcon = new QLabel("🍶");
    plasticIcon->setStyleSheet("font-size: 24px;");
    QLabel *plasticTitle = new QLabel("Plastic");
    plasticTitle->setStyleSheet("font-size: 12px; color: " + COLOR_TEXT_DIM + ";");
    m_plasticTotalLabel = new QLabel("320 kg");
    m_plasticTotalLabel->setStyleSheet("font-size: 20px; font-weight: bold; color: " + COLOR_ACCENT + ";");
    
    QLabel *metalIcon = new QLabel("🔩");
    metalIcon->setStyleSheet("font-size: 24px;");
    QLabel *metalTitle = new QLabel("Metal");
    metalTitle->setStyleSheet("font-size: 12px; color: " + COLOR_TEXT_DIM + ";");
    m_metalTotalLabel = new QLabel("285 kg");
    m_metalTotalLabel->setStyleSheet("font-size: 20px; font-weight: bold; color: " + COLOR_WARNING + ";");
    
    QLabel *glassIcon = new QLabel("🍷");
    glassIcon->setStyleSheet("font-size: 24px;");
    QLabel *glassTitle = new QLabel("Glass");
    glassTitle->setStyleSheet("font-size: 12px; color: " + COLOR_TEXT_DIM + ";");
    m_glassTotalLabel = new QLabel("242 kg");
    m_glassTotalLabel->setStyleSheet("font-size: 20px; font-weight: bold; color: " + COLOR_SUCCESS + ";");
    
    wasteGrid->addWidget(plasticIcon, 0, 0);
    wasteGrid->addWidget(plasticTitle, 0, 1);
    wasteGrid->addWidget(m_plasticTotalLabel, 0, 2);
    
    wasteGrid->addWidget(metalIcon, 1, 0);
    wasteGrid->addWidget(metalTitle, 1, 1);
    wasteGrid->addWidget(m_metalTotalLabel, 1, 2);
    
    wasteGrid->addWidget(glassIcon, 2, 0);
    wasteGrid->addWidget(glassTitle, 2, 1);
    wasteGrid->addWidget(m_glassTotalLabel, 2, 2);
    
    kpiLayout->addLayout(wasteGrid, 2);
    
    // Active Bins
    QVBoxLayout *binsLayout = new QVBoxLayout();
    QLabel *binsTitle = new QLabel("ACTIVE BINS");
    binsTitle->setStyleSheet("font-size: 13px; color: " + COLOR_TEXT_DIM + "; font-weight: bold;");
    m_activeBinsLabel = new QLabel("12");
    m_activeBinsLabel->setStyleSheet("font-size: 48px; font-weight: bold; color: " + COLOR_ACCENT + ";");
    QLabel *binsSubtext = new QLabel("Operational");
    binsSubtext->setStyleSheet("font-size: 12px; color: " + COLOR_TEXT_DIM + ";");
    binsLayout->addWidget(binsTitle);
    binsLayout->addWidget(m_activeBinsLabel);
    binsLayout->addWidget(binsSubtext);
    binsLayout->addStretch();
    kpiLayout->addLayout(binsLayout, 1);
}

void SmartRecyclingModule::createWasteTypesChart()
{
    QFrame *chartCard = new QFrame();
    chartCard->setStyleSheet(getCardStyle());
    
    QVBoxLayout *chartLayout = new QVBoxLayout(chartCard);
    chartLayout->setContentsMargins(20, 20, 20, 20);
    
    QLabel *chartTitle = new QLabel("📊 Waste Type Distribution");
    chartTitle->setStyleSheet("font-size: 16px; font-weight: bold; color: " + COLOR_TEXT + ";");
    chartLayout->addWidget(chartTitle);
    
    // Create pie chart
    m_wasteSeries = new QPieSeries();
    
    QPieSlice *plasticSlice = m_wasteSeries->append("Plastic", m_plasticTotal);
    plasticSlice->setColor(QColor(COLOR_ACCENT));
    plasticSlice->setLabelVisible(true);
    plasticSlice->setLabelColor(QColor(COLOR_TEXT));
    
    QPieSlice *metalSlice = m_wasteSeries->append("Metal", m_metalTotal);
    metalSlice->setColor(QColor(COLOR_WARNING));
    metalSlice->setLabelVisible(true);
    metalSlice->setLabelColor(QColor(COLOR_TEXT));
    
    QPieSlice *glassSlice = m_wasteSeries->append("Glass", m_glassTotal);
    glassSlice->setColor(QColor(COLOR_SUCCESS));
    glassSlice->setLabelVisible(true);
    glassSlice->setLabelColor(QColor(COLOR_TEXT));
    
    m_wasteChart = new QChart();
    m_wasteChart->addSeries(m_wasteSeries);
    m_wasteChart->setTitle("");
    m_wasteChart->legend()->setVisible(true);
    m_wasteChart->legend()->setAlignment(Qt::AlignBottom);
    m_wasteChart->legend()->setLabelColor(QColor(COLOR_TEXT));
    m_wasteChart->setBackgroundBrush(QBrush(QColor(COLOR_PANEL)));
    m_wasteChart->setTitleBrush(QBrush(QColor(COLOR_TEXT)));
    
    m_wasteChartView = new QChartView(m_wasteChart);
    m_wasteChartView->setRenderHint(QPainter::Antialiasing);
    m_wasteChartView->setMinimumHeight(300);
    
    chartLayout->addWidget(m_wasteChartView);
}

void SmartRecyclingModule::createBinStatusTable()
{
    QFrame *tableCard = new QFrame();
    tableCard->setStyleSheet(getCardStyle());
    
    QVBoxLayout *tableLayout = new QVBoxLayout(tableCard);
    tableLayout->setContentsMargins(20, 20, 20, 20);
    
    QLabel *tableTitle = new QLabel("🗑️ Bin Status Monitor");
    tableTitle->setStyleSheet("font-size: 16px; font-weight: bold; color: " + COLOR_TEXT + ";");
    tableLayout->addWidget(tableTitle);
    
    m_binStatusTable = new QTableWidget();
    m_binStatusTable->setColumnCount(4);
    m_binStatusTable->setHorizontalHeaderLabels({"Bin ID", "Location", "Fill Level", "Status"});
    m_binStatusTable->horizontalHeader()->setStretchLastSection(true);
    m_binStatusTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_binStatusTable->verticalHeader()->setVisible(false);
    m_binStatusTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_binStatusTable->setSelectionMode(QAbstractItemView::SingleSelection);
    m_binStatusTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_binStatusTable->setStyleSheet(getTableStyle());
    m_binStatusTable->setMinimumHeight(280);
    
    // Populate with sample data
    updateBinStatus("BIN-001", "Main Street Plaza", 35, "Operational");
    updateBinStatus("BIN-002", "Central Park East", 68, "Almost Full");
    updateBinStatus("BIN-003", "City Hall", 92, "Full");
    updateBinStatus("BIN-004", "Shopping District", 45, "Operational");
    updateBinStatus("BIN-005", "University Campus", 78, "Almost Full");
    updateBinStatus("BIN-006", "Residential Zone A", 22, "Operational");
    updateBinStatus("BIN-007", "Industrial Park", 88, "Full");
    updateBinStatus("BIN-008", "Sports Complex", 41, "Operational");
    
    tableLayout->addWidget(m_binStatusTable);
}

void SmartRecyclingModule::createCitizenRewardsTable()
{
    QFrame *tableCard = new QFrame();
    tableCard->setStyleSheet(getCardStyle());
    
    QVBoxLayout *tableLayout = new QVBoxLayout(tableCard);
    tableLayout->setContentsMargins(20, 20, 20, 20);
    
    QLabel *tableTitle = new QLabel("🏆 Top Citizen Recyclers & Rewards");
    tableTitle->setStyleSheet("font-size: 16px; font-weight: bold; color: " + COLOR_TEXT + ";");
    tableLayout->addWidget(tableTitle);
    
    m_citizenRewardsTable = new QTableWidget();
    m_citizenRewardsTable->setColumnCount(3);
    m_citizenRewardsTable->setHorizontalHeaderLabels({"Citizen Name", "Total Recycled (kg)", "Money Earned ($)"});
    m_citizenRewardsTable->horizontalHeader()->setStretchLastSection(true);
    m_citizenRewardsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_citizenRewardsTable->verticalHeader()->setVisible(false);
    m_citizenRewardsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_citizenRewardsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    m_citizenRewardsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_citizenRewardsTable->setStyleSheet(getTableStyle());
    m_citizenRewardsTable->setMinimumHeight(250);
    
    // Populate with sample data
    updateCitizenReward("Sarah Johnson", 245.5, 122.75);
    updateCitizenReward("Michael Chen", 198.3, 99.15);
    updateCitizenReward("Emma Williams", 176.2, 88.10);
    updateCitizenReward("David Martinez", 154.8, 77.40);
    updateCitizenReward("Lisa Anderson", 132.6, 66.30);
    updateCitizenReward("James Wilson", 118.4, 59.20);
    
    tableLayout->addWidget(m_citizenRewardsTable);
}

void SmartRecyclingModule::updateRecyclingData()
{
    // Simulate small increments in recycling data
    double increment = 1.0 + QRandomGenerator::global()->generateDouble() * 4.0;
    m_totalRecycled += increment;
    
    // Distribute increment among waste types
    m_plasticTotal += increment * 0.38;
    m_metalTotal += increment * 0.34;
    m_glassTotal += increment * 0.28;
    
    // Update KPI labels
    m_totalRecycledLabel->setText(QString::number(m_totalRecycled, 'f', 1) + " kg");
    m_plasticTotalLabel->setText(QString::number(m_plasticTotal, 'f', 1) + " kg");
    m_metalTotalLabel->setText(QString::number(m_metalTotal, 'f', 1) + " kg");
    m_glassTotalLabel->setText(QString::number(m_glassTotal, 'f', 1) + " kg");
    
    // Update pie chart
    m_wasteSeries->clear();
    QPieSlice *plasticSlice = m_wasteSeries->append("Plastic", m_plasticTotal);
    plasticSlice->setColor(QColor(COLOR_ACCENT));
    plasticSlice->setLabelVisible(true);
    plasticSlice->setLabelColor(QColor(COLOR_TEXT));
    
    QPieSlice *metalSlice = m_wasteSeries->append("Metal", m_metalTotal);
    metalSlice->setColor(QColor(COLOR_WARNING));
    metalSlice->setLabelVisible(true);
    metalSlice->setLabelColor(QColor(COLOR_TEXT));
    
    QPieSlice *glassSlice = m_wasteSeries->append("Glass", m_glassTotal);
    glassSlice->setColor(QColor(COLOR_SUCCESS));
    glassSlice->setLabelVisible(true);
    glassSlice->setLabelColor(QColor(COLOR_TEXT));
    
    // Update bin fill levels
    for (int i = 0; i < m_binStatusTable->rowCount(); ++i) {
        QString binId = m_binStatusTable->item(i, 0)->text();
        int fillLevel = calculateFillLevel(binId);
        m_binStatusTable->item(i, 2)->setText(QString::number(fillLevel) + "%");
        
        // Update status based on fill level
        QString status;
        if (fillLevel >= 90) {
            status = "Full";
            m_binStatusTable->item(i, 3)->setForeground(QBrush(QColor(COLOR_CRITICAL)));
        } else if (fillLevel >= 70) {
            status = "Almost Full";
            m_binStatusTable->item(i, 3)->setForeground(QBrush(QColor(COLOR_WARNING)));
        } else {
            status = "Operational";
            m_binStatusTable->item(i, 3)->setForeground(QBrush(QColor(COLOR_SUCCESS)));
        }
        m_binStatusTable->item(i, 3)->setText(status);
    }
    
    emit wasteCollected(increment);
}

void SmartRecyclingModule::updateBinStatus(const QString &binId, const QString &location, 
                                          int fillLevel, const QString &status)
{
    int row = m_binStatusTable->rowCount();
    m_binStatusTable->insertRow(row);
    
    m_binStatusTable->setItem(row, 0, new QTableWidgetItem(binId));
    m_binStatusTable->setItem(row, 1, new QTableWidgetItem(location));
    m_binStatusTable->setItem(row, 2, new QTableWidgetItem(QString::number(fillLevel) + "%"));
    m_binStatusTable->setItem(row, 3, new QTableWidgetItem(status));
    
    // Color-code the status
    if (fillLevel >= 90) {
        m_binStatusTable->item(row, 3)->setForeground(QBrush(QColor(COLOR_CRITICAL)));
    } else if (fillLevel >= 70) {
        m_binStatusTable->item(row, 3)->setForeground(QBrush(QColor(COLOR_WARNING)));
    } else {
        m_binStatusTable->item(row, 3)->setForeground(QBrush(QColor(COLOR_SUCCESS)));
    }
}

void SmartRecyclingModule::updateCitizenReward(const QString &name, double totalRecycled, double earned)
{
    int row = m_citizenRewardsTable->rowCount();
    m_citizenRewardsTable->insertRow(row);
    
    m_citizenRewardsTable->setItem(row, 0, new QTableWidgetItem(name));
    m_citizenRewardsTable->setItem(row, 1, new QTableWidgetItem(QString::number(totalRecycled, 'f', 1)));
    m_citizenRewardsTable->setItem(row, 2, new QTableWidgetItem(QString::number(earned, 'f', 2)));
    
    // Highlight high performers
    if (totalRecycled > 200) {
        m_citizenRewardsTable->item(row, 0)->setForeground(QBrush(QColor(COLOR_SUCCESS)));
    }
}

int SmartRecyclingModule::calculateFillLevel(const QString &binId)
{
    // Simulate gradual fill increase
    int currentLevel = 0;
    for (int i = 0; i < m_binStatusTable->rowCount(); ++i) {
        if (m_binStatusTable->item(i, 0)->text() == binId) {
            QString levelStr = m_binStatusTable->item(i, 2)->text();
            levelStr.remove("%");
            currentLevel = levelStr.toInt();
            break;
        }
    }
    
    // Small random increase
    int increase = QRandomGenerator::global()->bounded(0, 3);
    return qMin(100, currentLevel + increase);
}

void SmartRecyclingModule::onRefreshData()
{
    updateRecyclingData();
    addLogMessage("Data refreshed successfully");
}

void SmartRecyclingModule::onExportReport()
{
    addLogMessage("Exporting recycling report... [PDF/Excel format]");
    emit wasteCollected(0); // Trigger for logging purposes
}

void SmartRecyclingModule::onNotifyCollection()
{
    addLogMessage("Collection team notified for full bins");
    // Find and highlight full bins
    for (int i = 0; i < m_binStatusTable->rowCount(); ++i) {
        QString status = m_binStatusTable->item(i, 3)->text();
        if (status == "Full") {
            QString binId = m_binStatusTable->item(i, 0)->text();
            emit binStatusChanged(binId, "Collection Notified");
        }
    }
}

void SmartRecyclingModule::onSimulateBinFull()
{
    if (m_binStatusTable->rowCount() > 0) {
        int randomRow = QRandomGenerator::global()->bounded(m_binStatusTable->rowCount());
        m_binStatusTable->item(randomRow, 2)->setText("95%");
        m_binStatusTable->item(randomRow, 3)->setText("Full");
        m_binStatusTable->item(randomRow, 3)->setForeground(QBrush(QColor(COLOR_CRITICAL)));
        
        QString binId = m_binStatusTable->item(randomRow, 0)->text();
        addLogMessage("⚠️ ALERT: " + binId + " is now FULL - immediate collection required!");
        emit binStatusChanged(binId, "Full");
    }
}

void SmartRecyclingModule::addLogMessage(const QString &message)
{
    qDebug() << "[RECYCLING]" << message;
}

QString SmartRecyclingModule::getCardStyle()
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

QString SmartRecyclingModule::getTableStyle()
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

QString SmartRecyclingModule::getButtonStyle(const QString &color)
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

void SmartRecyclingModule::applyStyles()
{
    setStyleSheet("background-color: " + COLOR_BACKGROUND + ";");
}
