#include "smartlightingmodule.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFrame>
#include <QHeaderView>
#include <QGroupBox>
#include <QRandomGenerator>
#include <QScrollArea>
#include <QDateTime>
#include <QtCharts/QValueAxis>

SmartLightingModule::SmartLightingModule(QWidget *parent)
    : QWidget(parent)
    , m_energySavedPercentage(34.0)
    , m_currentMode("Auto Mode")
    , m_totalPoles(85)
    , m_activePoles(82)
    , m_manualIntensity(75)
{
    setupUI();
    applyStyles();
    
    // Initialize energy history (last 24 hours - percentage saved)
    for (int i = 0; i < 24; ++i) {
        m_energyHistory.append(25.0 + QRandomGenerator::global()->generateDouble() * 15.0);
    }
    
    // Setup auto-update timer
    m_updateTimer = new QTimer(this);
    connect(m_updateTimer, &QTimer::timeout, this, &SmartLightingModule::updateLightingData);
    m_updateTimer->start(7000); // Update every 7 seconds
    
    // Initial data population
    updateLightingData();
}

SmartLightingModule::~SmartLightingModule()
{
}

void SmartLightingModule::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(25);
    mainLayout->setContentsMargins(30, 30, 30, 30);
    
    // Title Header Frame
    QFrame *titleFrame = new QFrame();
    titleFrame->setStyleSheet("QFrame { background-color: " + COLOR_PANEL + "; border-radius: 8px; padding: 15px; }");
    QHBoxLayout *titleFrameLayout = new QHBoxLayout(titleFrame);
    titleFrameLayout->setContentsMargins(20, 15, 20, 15);
    
    QLabel *titleLabel = new QLabel("💡 Smart Sidewalk Lighting Management");
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
    contentLayout->addWidget(m_energySavedLabel->parentWidget());
    
    // Mode Control Section
    createModeControlSection();
    contentLayout->addWidget(m_modeSelector->parentWidget());
    
    // Main content grid: Energy Chart + Streetlight Table
    QWidget *mainContentWidget = new QWidget();
    QHBoxLayout *mainContentLayout = new QHBoxLayout(mainContentWidget);
    mainContentLayout->setSpacing(20);
    mainContentLayout->setContentsMargins(0, 0, 0, 0);
    
    // Left: Energy Consumption Chart
    createEnergyChart();
    mainContentLayout->addWidget(m_energyChartView->parentWidget(), 1);
    
    // Right: Streetlight Status Table
    createStreetlightTable();
    mainContentLayout->addWidget(m_streetlightTable->parentWidget(), 1);
    
    contentLayout->addWidget(mainContentWidget);
    
    // Control Buttons
    QWidget *controlWidget = new QWidget();
    QHBoxLayout *controlLayout = new QHBoxLayout(controlWidget);
    controlLayout->setSpacing(15);
    
    m_refreshBtn = new QPushButton("🔄 Refresh Data");
    m_exportBtn = new QPushButton("📊 Export Energy Report");
    m_simulateBtn = new QPushButton("⚡ Simulate Failure");
    
    connect(m_refreshBtn, &QPushButton::clicked, this, &SmartLightingModule::onRefreshData);
    connect(m_exportBtn, &QPushButton::clicked, this, &SmartLightingModule::onExportEnergyReport);
    connect(m_simulateBtn, &QPushButton::clicked, this, &SmartLightingModule::onSimulateFailure);
    
    m_refreshBtn->setStyleSheet(getButtonStyle(COLOR_ACCENT));
    m_exportBtn->setStyleSheet(getButtonStyle(COLOR_SUCCESS));
    m_simulateBtn->setStyleSheet(getButtonStyle(COLOR_CRITICAL));
    
    m_refreshBtn->setMinimumHeight(45);
    m_exportBtn->setMinimumHeight(45);
    m_simulateBtn->setMinimumHeight(45);
    
    controlLayout->addWidget(m_refreshBtn);
    controlLayout->addWidget(m_exportBtn);
    controlLayout->addWidget(m_simulateBtn);
    controlLayout->addStretch();
    
    contentLayout->addWidget(controlWidget);
    
    scrollArea->setWidget(scrollContent);
    mainLayout->addWidget(scrollArea);
}

void SmartLightingModule::createKPISection()
{
    QFrame *kpiCard = new QFrame();
    kpiCard->setStyleSheet(getCardStyle());
    kpiCard->setFixedHeight(180);
    
    QHBoxLayout *kpiLayout = new QHBoxLayout(kpiCard);
    kpiLayout->setSpacing(30);
    kpiLayout->setContentsMargins(35, 25, 35, 25);
    
    // Energy Saved Today - Enhanced
    QWidget *energyWidget = new QWidget();
    energyWidget->setFixedHeight(130);
    QVBoxLayout *energyLayout = new QVBoxLayout(energyWidget);
    energyLayout->setSpacing(6);
    energyLayout->setContentsMargins(0, 0, 0, 0);
    
    QLabel *energyIcon = new QLabel("⚡");
    energyIcon->setStyleSheet("font-size: 32px;");
    energyIcon->setAlignment(Qt::AlignLeft);
    energyIcon->setFixedHeight(35);
    
    QLabel *energyTitle = new QLabel("ENERGY SAVED TODAY");
    energyTitle->setStyleSheet("font-size: 11px; color: " + COLOR_TEXT + "; font-weight: bold; letter-spacing: 1px;");
    energyTitle->setAlignment(Qt::AlignLeft);
    energyTitle->setFixedHeight(16);
    
    m_energySavedLabel = new QLabel("34 %");
    m_energySavedLabel->setStyleSheet("font-size: 44px; font-weight: bold; color: " + COLOR_SUCCESS + "; margin-top: 5px;");
    m_energySavedLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_energySavedLabel->setFixedHeight(52);
    
    m_energyBar = new QProgressBar();
    m_energyBar->setRange(0, 100);
    m_energyBar->setValue(34);
    m_energyBar->setTextVisible(false);
    m_energyBar->setFixedHeight(14);
    m_energyBar->setStyleSheet(
        "QProgressBar {"
        "    border: 2px solid " + COLOR_BORDER + ";"
        "    border-radius: 5px;"
        "    background-color: " + COLOR_PANEL + ";"
        "}"
        "QProgressBar::chunk {"
        "    background-color: " + COLOR_SUCCESS + ";"
        "    border-radius: 3px;"
        "}"
    );
    
    energyLayout->addWidget(energyIcon);
    energyLayout->addWidget(energyTitle);
    energyLayout->addWidget(m_energySavedLabel);
    energyLayout->addWidget(m_energyBar);
    kpiLayout->addWidget(energyWidget, 1);
    
    // Current Mode - Enhanced
    QWidget *modeWidget = new QWidget();
    modeWidget->setFixedHeight(130);
    QVBoxLayout *modeLayout = new QVBoxLayout(modeWidget);
    modeLayout->setSpacing(6);
    modeLayout->setContentsMargins(0, 0, 0, 0);
    
    QLabel *modeIcon = new QLabel("🌙");
    modeIcon->setStyleSheet("font-size: 32px;");
    modeIcon->setAlignment(Qt::AlignLeft);
    modeIcon->setFixedHeight(35);
    
    QLabel *modeTitle = new QLabel("LIGHTING MODE");
    modeTitle->setStyleSheet("font-size: 11px; color: " + COLOR_TEXT + "; font-weight: bold; letter-spacing: 1px;");
    modeTitle->setAlignment(Qt::AlignLeft);
    modeTitle->setFixedHeight(16);
    
    m_currentModeLabel = new QLabel("Auto Mode");
    m_currentModeLabel->setStyleSheet("font-size: 26px; font-weight: bold; color: " + COLOR_ACCENT + "; margin-top: 5px;");
    m_currentModeLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_currentModeLabel->setFixedHeight(48);
    
    QLabel *modeSubtext = new QLabel("Adaptive brightness");
    modeSubtext->setStyleSheet("font-size: 11px; color: " + COLOR_TEXT_DIM + ";");
    modeSubtext->setAlignment(Qt::AlignLeft);
    
    modeLayout->addWidget(modeIcon);
    modeLayout->addWidget(modeTitle);
    modeLayout->addWidget(m_currentModeLabel);
    modeLayout->addWidget(modeSubtext);
    kpiLayout->addWidget(modeWidget, 1);
    
    // Total Poles - Enhanced
    QWidget *totalWidget = new QWidget();
    totalWidget->setFixedHeight(130);
    QVBoxLayout *totalLayout = new QVBoxLayout(totalWidget);
    totalLayout->setSpacing(8);
    totalLayout->setContentsMargins(0, 0, 0, 0);
    
    QLabel *totalIcon = new QLabel("💡");
    totalIcon->setStyleSheet("font-size: 32px;");
    totalIcon->setAlignment(Qt::AlignLeft);
    totalIcon->setFixedHeight(35);
    
    QLabel *totalTitle = new QLabel("TOTAL POLES");
    totalTitle->setStyleSheet("font-size: 11px; color: " + COLOR_TEXT + "; font-weight: bold; letter-spacing: 1px;");
    totalTitle->setAlignment(Qt::AlignLeft);
    totalTitle->setFixedHeight(18);
    
    m_totalPolesLabel = new QLabel("85");
    m_totalPolesLabel->setStyleSheet("font-size: 46px; font-weight: bold; color: " + COLOR_ACCENT + "; margin-top: 5px;");
    m_totalPolesLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_totalPolesLabel->setFixedHeight(55);
    
    QLabel *totalSubtext = new QLabel("Citywide");
    totalSubtext->setStyleSheet("font-size: 11px; color: " + COLOR_TEXT_DIM + ";");
    totalSubtext->setAlignment(Qt::AlignLeft);
    
    totalLayout->addWidget(totalIcon);
    totalLayout->addWidget(totalTitle);
    totalLayout->addWidget(m_totalPolesLabel);
    totalLayout->addWidget(totalSubtext);
    kpiLayout->addWidget(totalWidget, 1);
    
    // Active Poles - Enhanced
    QWidget *activeWidget = new QWidget();
    activeWidget->setFixedHeight(130);
    QVBoxLayout *activeLayout = new QVBoxLayout(activeWidget);
    activeLayout->setSpacing(8);
    activeLayout->setContentsMargins(0, 0, 0, 0);
    
    QLabel *activeIcon = new QLabel("✅");
    activeIcon->setStyleSheet("font-size: 32px;");
    activeIcon->setAlignment(Qt::AlignLeft);
    activeIcon->setFixedHeight(35);
    
    QLabel *activeTitle = new QLabel("ACTIVE POLES");
    activeTitle->setStyleSheet("font-size: 11px; color: " + COLOR_TEXT + "; font-weight: bold; letter-spacing: 1px;");
    activeTitle->setAlignment(Qt::AlignLeft);
    activeTitle->setFixedHeight(18);
    
    m_activePolesLabel = new QLabel("82");
    m_activePolesLabel->setStyleSheet("font-size: 46px; font-weight: bold; color: " + COLOR_SUCCESS + "; margin-top: 5px;");
    m_activePolesLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_activePolesLabel->setFixedHeight(55);
    
    QLabel *activeSubtext = new QLabel("Operational");
    activeSubtext->setStyleSheet("font-size: 11px; color: " + COLOR_TEXT_DIM + ";");
    activeSubtext->setAlignment(Qt::AlignLeft);
    
    activeLayout->addWidget(activeIcon);
    activeLayout->addWidget(activeTitle);
    activeLayout->addWidget(m_activePolesLabel);
    activeLayout->addWidget(activeSubtext);
    kpiLayout->addWidget(activeWidget, 1);
}

void SmartLightingModule::createModeControlSection()
{
    QFrame *controlCard = new QFrame();
    controlCard->setStyleSheet(getCardStyle());
    controlCard->setMinimumHeight(120);
    
    QHBoxLayout *controlLayout = new QHBoxLayout(controlCard);
    controlLayout->setSpacing(25);
    controlLayout->setContentsMargins(25, 20, 25, 20);
    
    // Mode Selection
    QVBoxLayout *modeSelectLayout = new QVBoxLayout();
    QLabel *modeLabel = new QLabel("⚙️ Select Lighting Mode:");
    modeLabel->setStyleSheet("font-size: 14px; font-weight: bold; color: " + COLOR_TEXT + ";");
    
    m_modeSelector = new QComboBox();
    m_modeSelector->addItem("🌟 Auto Mode (Adaptive)");
    m_modeSelector->addItem("🌿 Eco Mode (Energy Saving)");
    m_modeSelector->addItem("🔧 Manual Override");
    m_modeSelector->setStyleSheet(
        "QComboBox {"
        "    background-color: " + COLOR_PANEL + ";"
        "    color: " + COLOR_TEXT + ";"
        "    border: 2px solid " + COLOR_ACCENT + ";"
        "    border-radius: 6px;"
        "    padding: 10px;"
        "    font-size: 14px;"
        "    min-width: 250px;"
        "}"
        "QComboBox::drop-down {"
        "    border: none;"
        "}"
        "QComboBox QAbstractItemView {"
        "    background-color: " + COLOR_PANEL + ";"
        "    color: " + COLOR_TEXT + ";"
        "    selection-background-color: " + COLOR_ACCENT + ";"
        "    border: 1px solid " + COLOR_BORDER + ";"
        "}"
    );
    m_modeSelector->setCurrentIndex(0);
    connect(m_modeSelector, QOverload<int>::of(&QComboBox::currentIndexChanged), 
            this, &SmartLightingModule::onModeChanged);
    
    modeSelectLayout->addWidget(modeLabel);
    modeSelectLayout->addWidget(m_modeSelector);
    modeSelectLayout->addStretch();
    controlLayout->addLayout(modeSelectLayout, 1);
    
    // Manual Intensity Control
    QVBoxLayout *intensityLayout = new QVBoxLayout();
    QLabel *intensityLabel = new QLabel("💡 Manual Intensity Control:");
    intensityLabel->setStyleSheet("font-size: 14px; font-weight: bold; color: " + COLOR_TEXT + ";");
    
    QHBoxLayout *sliderLayout = new QHBoxLayout();
    m_manualIntensitySlider = new QSlider(Qt::Horizontal);
    m_manualIntensitySlider->setRange(0, 100);
    m_manualIntensitySlider->setValue(75);
    m_manualIntensitySlider->setEnabled(false); // Enabled only in manual mode
    m_manualIntensitySlider->setStyleSheet(
        "QSlider::groove:horizontal {"
        "    border: 1px solid " + COLOR_BORDER + ";"
        "    height: 10px;"
        "    background: " + COLOR_PANEL + ";"
        "    border-radius: 5px;"
        "}"
        "QSlider::handle:horizontal {"
        "    background: " + COLOR_ACCENT + ";"
        "    border: 2px solid " + COLOR_ACCENT + ";"
        "    width: 20px;"
        "    height: 20px;"
        "    margin: -5px 0;"
        "    border-radius: 10px;"
        "}"
        "QSlider::handle:horizontal:hover {"
        "    background: " + COLOR_WARNING + ";"
        "}"
    );
    connect(m_manualIntensitySlider, &QSlider::valueChanged, 
            this, &SmartLightingModule::onManualIntensityChanged);
    
    m_intensityValueLabel = new QLabel("75%");
    m_intensityValueLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: " + COLOR_ACCENT + "; min-width: 50px;");
    m_intensityValueLabel->setAlignment(Qt::AlignCenter);
    
    sliderLayout->addWidget(m_manualIntensitySlider, 1);
    sliderLayout->addWidget(m_intensityValueLabel);
    
    intensityLayout->addWidget(intensityLabel);
    intensityLayout->addLayout(sliderLayout);
    intensityLayout->addStretch();
    controlLayout->addLayout(intensityLayout, 2);
}

void SmartLightingModule::createEnergyChart()
{
    QFrame *chartCard = new QFrame();
    chartCard->setStyleSheet(getCardStyle());
    
    QVBoxLayout *chartLayout = new QVBoxLayout(chartCard);
    chartLayout->setContentsMargins(20, 20, 20, 20);
    
    QLabel *chartTitle = new QLabel("📊 Energy Savings Trend (24 Hours)");
    chartTitle->setStyleSheet("font-size: 16px; font-weight: bold; color: " + COLOR_TEXT + ";");
    chartLayout->addWidget(chartTitle);
    
    // Create line chart
    m_energySeries = new QLineSeries();
    for (int i = 0; i < m_energyHistory.size(); ++i) {
        m_energySeries->append(i, m_energyHistory[i]);
    }
    
    QPen pen{QColor(COLOR_SUCCESS)};
    pen.setWidth(3);
    m_energySeries->setPen(pen);
    
    m_energyChart = new QChart();
    m_energyChart->addSeries(m_energySeries);
    m_energyChart->setTitle("");
    m_energyChart->setAnimationOptions(QChart::SeriesAnimations);
    m_energyChart->setBackgroundBrush(QBrush(QColor(COLOR_PANEL)));
    m_energyChart->setTitleBrush(QBrush(QColor(COLOR_TEXT)));
    
    QValueAxis *axisX = new QValueAxis();
    axisX->setRange(0, 23);
    axisX->setLabelFormat("%d");
    axisX->setTitleText("Hour");
    axisX->setLabelsColor(QColor(COLOR_TEXT));
    axisX->setTitleBrush(QBrush(QColor(COLOR_TEXT)));
    m_energyChart->addAxis(axisX, Qt::AlignBottom);
    m_energySeries->attachAxis(axisX);
    
    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0, 50);
    axisY->setLabelFormat("%d%%");
    axisY->setTitleText("Energy Saved");
    axisY->setLabelsColor(QColor(COLOR_TEXT));
    axisY->setTitleBrush(QBrush(QColor(COLOR_TEXT)));
    m_energyChart->addAxis(axisY, Qt::AlignLeft);
    m_energySeries->attachAxis(axisY);
    
    m_energyChart->legend()->setVisible(false);
    
    m_energyChartView = new QChartView(m_energyChart);
    m_energyChartView->setRenderHint(QPainter::Antialiasing);
    m_energyChartView->setMinimumHeight(300);
    
    chartLayout->addWidget(m_energyChartView);
}

void SmartLightingModule::createStreetlightTable()
{
    QFrame *tableCard = new QFrame();
    tableCard->setStyleSheet(getCardStyle());
    
    QVBoxLayout *tableLayout = new QVBoxLayout(tableCard);
    tableLayout->setContentsMargins(20, 20, 20, 20);
    
    QLabel *tableTitle = new QLabel("🔦 Streetlight Pole Status");
    tableTitle->setStyleSheet("font-size: 16px; font-weight: bold; color: " + COLOR_TEXT + ";");
    tableLayout->addWidget(tableTitle);
    
    m_streetlightTable = new QTableWidget();
    m_streetlightTable->setColumnCount(5);
    m_streetlightTable->setHorizontalHeaderLabels({"Pole ID", "Location", "Intensity", "Presence", "Status"});
    m_streetlightTable->horizontalHeader()->setStretchLastSection(true);
    m_streetlightTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_streetlightTable->verticalHeader()->setVisible(false);
    m_streetlightTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_streetlightTable->setSelectionMode(QAbstractItemView::SingleSelection);
    m_streetlightTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_streetlightTable->setStyleSheet(getTableStyle());
    m_streetlightTable->setMinimumHeight(280);
    
    // Populate with sample data
    updateStreetlight("POLE-001", "Main Street North", 85, "Detected", "Active");
    updateStreetlight("POLE-002", "Main Street South", 45, "None", "Active");
    updateStreetlight("POLE-003", "Central Plaza", 90, "Detected", "Active");
    updateStreetlight("POLE-004", "Park Avenue", 60, "None", "Active");
    updateStreetlight("POLE-005", "University Gate", 95, "Detected", "Active");
    updateStreetlight("POLE-006", "Shopping District", 75, "Detected", "Active");
    updateStreetlight("POLE-007", "Residential Zone", 40, "None", "Active");
    updateStreetlight("POLE-008", "Industrial Area", 0, "None", "Maintenance");
    updateStreetlight("POLE-009", "Sports Complex", 80, "Detected", "Active");
    updateStreetlight("POLE-010", "Hospital Road", 100, "Detected", "Active");
    
    tableLayout->addWidget(m_streetlightTable);
}

void SmartLightingModule::updateLightingData()
{
    // Simulate energy savings fluctuation
    double newSaving = m_energySavedPercentage + (QRandomGenerator::global()->generateDouble() * 5.0 - 2.0);
    newSaving = qBound(20.0, newSaving, 50.0);
    m_energySavedPercentage = newSaving;
    
    m_energySavedLabel->setText(QString::number(m_energySavedPercentage, 'f', 1) + " %");
    m_energyBar->setValue(static_cast<int>(m_energySavedPercentage));
    
    // Update energy history
    m_energyHistory.removeFirst();
    m_energyHistory.append(m_energySavedPercentage);
    
    // Update chart
    m_energySeries->clear();
    for (int i = 0; i < m_energyHistory.size(); ++i) {
        m_energySeries->append(i, m_energyHistory[i]);
    }
    
    // Update streetlight intensities (simulate presence detection)
    for (int i = 0; i < m_streetlightTable->rowCount(); ++i) {
        QString status = m_streetlightTable->item(i, 4)->text();
        if (status == "Active") {
            // Randomly detect presence
            bool presenceDetected = QRandomGenerator::global()->bounded(100) < 40; // 40% chance
            m_streetlightTable->item(i, 3)->setText(presenceDetected ? "Detected" : "None");
            
            // Adjust intensity based on presence
            int currentIntensity = m_streetlightTable->item(i, 2)->text().remove("%").toInt();
            int newIntensity;
            if (m_currentMode == "Auto Mode") {
                newIntensity = presenceDetected ? qMin(100, currentIntensity + 10) : qMax(30, currentIntensity - 5);
            } else if (m_currentMode == "Eco Mode") {
                newIntensity = presenceDetected ? 70 : 25;
            } else {
                newIntensity = m_manualIntensity;
            }
            m_streetlightTable->item(i, 2)->setText(QString::number(newIntensity) + "%");
        }
    }
    
    emit energySaved(m_energySavedPercentage);
}

void SmartLightingModule::updateStreetlight(const QString &poleId, const QString &location,
                                           int intensity, const QString &presence, const QString &status)
{
    int row = m_streetlightTable->rowCount();
    m_streetlightTable->insertRow(row);
    
    m_streetlightTable->setItem(row, 0, new QTableWidgetItem(poleId));
    m_streetlightTable->setItem(row, 1, new QTableWidgetItem(location));
    m_streetlightTable->setItem(row, 2, new QTableWidgetItem(QString::number(intensity) + "%"));
    m_streetlightTable->setItem(row, 3, new QTableWidgetItem(presence));
    m_streetlightTable->setItem(row, 4, new QTableWidgetItem(status));
    
    // Color-code status
    QString statusColor = getStatusColor(status);
    m_streetlightTable->item(row, 4)->setForeground(QBrush(QColor(statusColor)));
    
    // Color-code presence
    if (presence == "Detected") {
        m_streetlightTable->item(row, 3)->setForeground(QBrush(QColor(COLOR_SUCCESS)));
    }
}

QString SmartLightingModule::getStatusColor(const QString &status)
{
    if (status == "Active") return COLOR_SUCCESS;
    if (status == "Maintenance") return COLOR_WARNING;
    return COLOR_CRITICAL;
}

void SmartLightingModule::onModeChanged(int index)
{
    switch (index) {
        case 0:
            m_currentMode = "Auto Mode";
            m_currentModeLabel->setText("Auto Mode");
            m_manualIntensitySlider->setEnabled(false);
            addLogMessage("Switched to Auto Mode - adaptive brightness");
            break;
        case 1:
            m_currentMode = "Eco Mode";
            m_currentModeLabel->setText("Eco Mode");
            m_manualIntensitySlider->setEnabled(false);
            addLogMessage("Switched to Eco Mode - maximum energy savings");
            break;
        case 2:
            m_currentMode = "Manual Override";
            m_currentModeLabel->setText("Manual");
            m_manualIntensitySlider->setEnabled(true);
            addLogMessage("Manual Override activated - intensity set to " + QString::number(m_manualIntensity) + "%");
            break;
    }
    
    emit lightingModeChanged(m_currentMode);
}

void SmartLightingModule::onManualIntensityChanged(int value)
{
    m_manualIntensity = value;
    m_intensityValueLabel->setText(QString::number(value) + "%");
    
    if (m_currentMode == "Manual Override") {
        addLogMessage("Manual intensity adjusted to " + QString::number(value) + "%");
    }
}

void SmartLightingModule::onRefreshData()
{
    updateLightingData();
    addLogMessage("Lighting data refreshed successfully");
}

void SmartLightingModule::onExportEnergyReport()
{
    addLogMessage("Exporting energy report... [PDF/Excel format]");
}

void SmartLightingModule::onSimulateFailure()
{
    if (m_streetlightTable->rowCount() > 0) {
        int randomRow = QRandomGenerator::global()->bounded(m_streetlightTable->rowCount());
        m_streetlightTable->item(randomRow, 2)->setText("0%");
        m_streetlightTable->item(randomRow, 4)->setText("Maintenance");
        m_streetlightTable->item(randomRow, 4)->setForeground(QBrush(QColor(COLOR_WARNING)));
        
        QString poleId = m_streetlightTable->item(randomRow, 0)->text();
        m_activePoles--;
        m_activePolesLabel->setText(QString::number(m_activePoles));
        
        addLogMessage("⚠️ ALERT: " + poleId + " requires maintenance - lighting failure detected!");
        emit streetlightStatusChanged(poleId, "Maintenance");
    }
}

void SmartLightingModule::addLogMessage(const QString &message)
{
    qDebug() << "[SMART LIGHTING]" << message;
}

QString SmartLightingModule::getCardStyle()
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

QString SmartLightingModule::getTableStyle()
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

QString SmartLightingModule::getButtonStyle(const QString &color)
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

void SmartLightingModule::applyStyles()
{
    setStyleSheet("background-color: " + COLOR_BACKGROUND + ";");
}
