#include "securityintelligencecenter.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFrame>
#include <QHeaderView>
#include <QGroupBox>
#include <QRandomGenerator>
#include <QScrollBar>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChartView>
#include <QtCharts/QValueAxis>

SecurityIntelligenceCenter::SecurityIntelligenceCenter(QWidget *parent)
    : QWidget(parent)
    , m_currentStabilityScore(85)
    , m_simulationMode(false)
    , m_wasteRisk(10)
    , m_safetyRisk(15)
    , m_energyRisk(8)
    , m_cyberThreatLevel(5)
{
    setupUI();
    applyStyles();
    
    // Initialize risk history with baseline data
    for (int i = 0; i < 24; ++i) {
        m_riskHistory.append(15.0 + (QRandomGenerator::global()->bounded(10)));
    }
    
    // Setup auto-update timer
    m_updateTimer = new QTimer(this);
    connect(m_updateTimer, &QTimer::timeout, this, &SecurityIntelligenceCenter::updateRiskMetrics);
    m_updateTimer->start(5000); // Update every 5 seconds
    
    // Initial log entries
    addLogEntry("INFO", "Security Intelligence Center initialized");
    addLogEntry("INFO", "All systems operational");
    addLogEntry("INFO", "Monitoring 6 city infrastructure modules");
    
    // Generate initial recommendation
    generateStrategicRecommendation();
}

SecurityIntelligenceCenter::~SecurityIntelligenceCenter()
{
}

void SecurityIntelligenceCenter::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    
    // Title
    QLabel *titleLabel = new QLabel("🔐 Security & Intelligence Command Center");
    titleLabel->setStyleSheet("font-size: 22px; font-weight: bold; color: #1E90FF; padding: 8px;");
    mainLayout->addWidget(titleLabel);
    
    // Create scroll area for content
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setStyleSheet("QScrollArea { background-color: #121212; border: none; }");
    
    QWidget *scrollContent = new QWidget();
    QGridLayout *gridLayout = new QGridLayout(scrollContent);
    gridLayout->setSpacing(15);
    gridLayout->setContentsMargins(0, 0, 0, 0);
    
    // Row 1: System Health (col 0-1) | Threat Detection (col 2-3)
    createSystemHealthPanel();
    createThreatDetectionPanel();
    gridLayout->addWidget(m_systemHealthTable->parentWidget(), 0, 0, 1, 2);
    gridLayout->addWidget(m_threatDetectionTable->parentWidget(), 0, 2, 1, 2);
    
    // Row 2: Security Log (col 0-1) | Predictive Intelligence (col 2-3)
    createSecurityLogPanel();
    createPredictiveIntelligencePanel();
    gridLayout->addWidget(m_securityLog->parentWidget(), 1, 0, 1, 2);
    gridLayout->addWidget(m_predictiveTable->parentWidget(), 1, 2, 1, 2);
    
    // Row 3: Stability Score (col 0-2, spans 3 cols) | Simulation Control (col 3)
    createStabilityScorePanel();
    createSimulationControlPanel();
    gridLayout->addWidget(m_stabilityBar->parentWidget(), 2, 0, 1, 3);
    gridLayout->addWidget(m_simulationModeCheckbox->parentWidget(), 2, 3, 1, 1);
    
    scrollArea->setWidget(scrollContent);
    mainLayout->addWidget(scrollArea);
}

void SecurityIntelligenceCenter::createSystemHealthPanel()
{
    QFrame *card = new QFrame();
    card->setStyleSheet(getCardStyle());
    card->setMinimumHeight(260);
    card->setMaximumHeight(280);
    
    QVBoxLayout *layout = new QVBoxLayout(card);
    
    QLabel *title = new QLabel("1️⃣ System Health Overview");
    title->setStyleSheet("font-size: 16px; font-weight: bold; color: #1E90FF; padding: 5px;");
    layout->addWidget(title);
    
    m_systemHealthTable = new QTableWidget(6, 4);
    m_systemHealthTable->setStyleSheet(getTableStyle());
    m_systemHealthTable->setHorizontalHeaderLabels({"Module", "Operational Status", "Last Signal", "Risk Level"});
    m_systemHealthTable->horizontalHeader()->setStretchLastSection(true);
    m_systemHealthTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_systemHealthTable->verticalHeader()->setVisible(false);
    m_systemHealthTable->verticalHeader()->setDefaultSectionSize(35);
    m_systemHealthTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_systemHealthTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_systemHealthTable->setMaximumHeight(240);
    
    // Populate initial data
    QStringList modules = {"Recycling Infrastructure", "Pedestrian Safety Network", 
                          "Smart Lighting Network", "Communication Layer", 
                          "Database & Storage", "Authentication System"};
    QStringList statuses = {"✓ Operational", "✓ Operational", "✓ Operational", 
                           "⚠ Warning", "✓ Operational", "✓ Operational"};
    QStringList signalTimes = {"2s ago", "1s ago", "3s ago", "10s ago", "1s ago", "2s ago"};
    QStringList risks = {"Low", "Medium", "Low", "Medium", "Low", "Low"};
    
    for (int i = 0; i < 6; ++i) {
        m_systemHealthTable->setItem(i, 0, new QTableWidgetItem(modules[i]));
        
        QTableWidgetItem *statusItem = new QTableWidgetItem(statuses[i]);
        if (statuses[i].contains("✓")) {
            statusItem->setForeground(QBrush(QColor(COLOR_SUCCESS)));
        } else {
            statusItem->setForeground(QBrush(QColor(COLOR_WARNING)));
        }
        m_systemHealthTable->setItem(i, 1, statusItem);
        
        m_systemHealthTable->setItem(i, 2, new QTableWidgetItem(signalTimes[i]));
        
        QTableWidgetItem *riskItem = new QTableWidgetItem(risks[i]);
        if (risks[i] == "Low") {
            riskItem->setForeground(QBrush(QColor(COLOR_SUCCESS)));
        } else if (risks[i] == "Medium") {
            riskItem->setForeground(QBrush(QColor(COLOR_WARNING)));
        } else {
            riskItem->setForeground(QBrush(QColor(COLOR_CRITICAL)));
        }
        m_systemHealthTable->setItem(i, 3, riskItem);
    }
    
    layout->addWidget(m_systemHealthTable);
}

void SecurityIntelligenceCenter::createThreatDetectionPanel()
{
    QFrame *card = new QFrame();
    card->setStyleSheet(getCardStyle());
    card->setMinimumHeight(260);
    card->setMaximumHeight(280);
    
    QVBoxLayout *layout = new QVBoxLayout(card);
    
    QLabel *title = new QLabel("2️⃣ Intrusion & Threat Detection Panel");
    title->setStyleSheet("font-size: 16px; font-weight: bold; color: #FF9800; padding: 5px;");
    layout->addWidget(title);
    
    m_threatDetectionTable = new QTableWidget(0, 4);
    m_threatDetectionTable->setStyleSheet(getTableStyle());
    m_threatDetectionTable->setHorizontalHeaderLabels({"Alert Type", "Affected Module", "Severity", "Timestamp"});
    m_threatDetectionTable->horizontalHeader()->setStretchLastSection(true);
    m_threatDetectionTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_threatDetectionTable->verticalHeader()->setVisible(false);
    m_threatDetectionTable->verticalHeader()->setDefaultSectionSize(32);
    m_threatDetectionTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_threatDetectionTable->setMaximumHeight(180);
    
    layout->addWidget(m_threatDetectionTable);
    
    // Button row
    QHBoxLayout *btnLayout = new QHBoxLayout();
    m_clearAlertsBtn = new QPushButton("Clear Alerts");
    m_clearAlertsBtn->setStyleSheet(getButtonStyle(COLOR_ACCENT));
    m_clearAlertsBtn->setMinimumHeight(35);
    connect(m_clearAlertsBtn, &QPushButton::clicked, this, &SecurityIntelligenceCenter::onClearAlerts);
    
    m_triggerThreatBtn = new QPushButton("⚠ Trigger Simulated Threat");
    m_triggerThreatBtn->setStyleSheet(getButtonStyle(COLOR_WARNING));
    m_triggerThreatBtn->setMinimumHeight(35);
    connect(m_triggerThreatBtn, &QPushButton::clicked, this, &SecurityIntelligenceCenter::onTriggerSimulatedThreat);
    
    btnLayout->addWidget(m_clearAlertsBtn);
    btnLayout->addWidget(m_triggerThreatBtn);
    layout->addLayout(btnLayout);
}

void SecurityIntelligenceCenter::createSecurityLogPanel()
{
    QFrame *card = new QFrame();
    card->setStyleSheet(getCardStyle());
    card->setMinimumHeight(260);
    card->setMaximumHeight(280);
    
    QVBoxLayout *layout = new QVBoxLayout(card);
    
    QLabel *title = new QLabel("3️⃣ Live Security Log Monitor");
    title->setStyleSheet("font-size: 16px; font-weight: bold; color: #00C853; padding: 5px;");
    layout->addWidget(title);
    
    m_securityLog = new QPlainTextEdit();
    m_securityLog->setReadOnly(true);
    m_securityLog->setMaximumHeight(220);
    m_securityLog->setStyleSheet(
        "QPlainTextEdit {"
        "   background-color: #000000;"
        "   color: #00FF00;"
        "   font-family: 'Courier New', monospace;"
        "   font-size: 10px;"
        "   border: 1px solid #2A2A2A;"
        "   border-radius: 5px;"
        "   padding: 8px;"
        "}"
    );
    
    layout->addWidget(m_securityLog);
}

void SecurityIntelligenceCenter::createPredictiveIntelligencePanel()
{
    QFrame *card = new QFrame();
    card->setStyleSheet(getCardStyle());
    card->setMinimumHeight(320);
    card->setMaximumHeight(360);
    
    QVBoxLayout *layout = new QVBoxLayout(card);
    
    QLabel *title = new QLabel("4️⃣ Predictive Intelligence Engine");
    title->setStyleSheet("font-size: 16px; font-weight: bold; color: #1E90FF; padding: 5px;");
    layout->addWidget(title);
    
    m_predictiveTable = new QTableWidget(0, 4);
    m_predictiveTable->setStyleSheet(getTableStyle());
    m_predictiveTable->setHorizontalHeaderLabels({"Prediction", "Confidence (%)", "Related Module", "Recommended Action"});
    m_predictiveTable->horizontalHeader()->setStretchLastSection(true);
    m_predictiveTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    m_predictiveTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    m_predictiveTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    m_predictiveTable->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    m_predictiveTable->verticalHeader()->setVisible(false);
    m_predictiveTable->verticalHeader()->setDefaultSectionSize(35);
    m_predictiveTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_predictiveTable->setMinimumHeight(155);
    m_predictiveTable->setMaximumHeight(175);
    m_predictiveTable->setSelectionMode(QAbstractItemView::SingleSelection);
    m_predictiveTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    
    // Add initial predictions
    int row = m_predictiveTable->rowCount();
    m_predictiveTable->insertRow(row);
    m_predictiveTable->setItem(row, 0, new QTableWidgetItem("Bin 12 likely to overflow in 4 hours"));
    m_predictiveTable->setItem(row, 1, new QTableWidgetItem("87%"));
    m_predictiveTable->setItem(row, 2, new QTableWidgetItem("Recycling"));
    m_predictiveTable->setItem(row, 3, new QTableWidgetItem("Dispatch maintenance"));
    
    row = m_predictiveTable->rowCount();
    m_predictiveTable->insertRow(row);
    m_predictiveTable->setItem(row, 0, new QTableWidgetItem("Crosswalk 3 risk escalation detected"));
    m_predictiveTable->setItem(row, 1, new QTableWidgetItem("92%"));
    m_predictiveTable->setItem(row, 2, new QTableWidgetItem("Safety"));
    m_predictiveTable->setItem(row, 3, new QTableWidgetItem("Increase monitoring"));
    
    row = m_predictiveTable->rowCount();
    m_predictiveTable->insertRow(row);
    m_predictiveTable->setItem(row, 0, new QTableWidgetItem("Energy spike predicted in Sector B"));
    m_predictiveTable->setItem(row, 1, new QTableWidgetItem("78%"));
    m_predictiveTable->setItem(row, 2, new QTableWidgetItem("Energy"));
    m_predictiveTable->setItem(row, 3, new QTableWidgetItem("Optimize load distribution"));
    
    row = m_predictiveTable->rowCount();
    m_predictiveTable->insertRow(row);
    m_predictiveTable->setItem(row, 0, new QTableWidgetItem("Communication node intermittent connectivity"));
    m_predictiveTable->setItem(row, 1, new QTableWidgetItem("85%"));
    m_predictiveTable->setItem(row, 2, new QTableWidgetItem("Network"));
    m_predictiveTable->setItem(row, 3, new QTableWidgetItem("Schedule maintenance"));
    
    layout->addWidget(m_predictiveTable);
    
    layout->addSpacing(10);
    
    // Strategic Recommendation Panel - Simplified Clean Design
    QFrame *recFrame = new QFrame();
    recFrame->setStyleSheet(
        "QFrame {"
        "   background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #1E3A5F, stop:1 #1A1A2E);"
        "   border: 2px solid #1E90FF;"
        "   border-radius: 10px;"
        "   padding: 0px;"
        "}"
    );
    recFrame->setMinimumHeight(130);
    recFrame->setMaximumHeight(145);
    
    QVBoxLayout *recLayout = new QVBoxLayout(recFrame);
    recLayout->setSpacing(8);
    recLayout->setContentsMargins(12, 10, 12, 10);
    
    QLabel *recTitle = new QLabel("🧠 AI Strategic Recommendation");
    recTitle->setStyleSheet(
        "font-size: 14px; "
        "font-weight: bold; "
        "color: #4FC3F7; "
        "border: none; "
        "padding: 0px;"
    );
    recLayout->addWidget(recTitle);
    
    m_recommendationLabel = new QLabel(
        "Deploy additional monitoring resources to communication infrastructure. "
        "Predictive analysis indicates potential service degradation."
    );
    m_recommendationLabel->setWordWrap(true);
    m_recommendationLabel->setStyleSheet(
        "color: #FFFFFF; "
        "font-size: 11px; "
        "background-color: rgba(30, 144, 255, 0.15); "
        "border: 1px solid rgba(30, 144, 255, 0.3); "
        "border-radius: 6px; "
        "padding: 8px; "
        "line-height: 140%;"
    );
    m_recommendationLabel->setMinimumHeight(48);
    m_recommendationLabel->setMaximumHeight(52);
    recLayout->addWidget(m_recommendationLabel);
    
    // Metadata row
    QHBoxLayout *metaLayout = new QHBoxLayout();
    metaLayout->setSpacing(20);
    
    m_recommendationTypeLabel = new QLabel("📋 Type: Strategic");
    m_recommendationTypeLabel->setStyleSheet(
        "color: #4CAF50; "
        "font-weight: bold; "
        "font-size: 10px; "
        "border: none; "
        "padding: 2px;"
    );
    
    m_priorityLabel = new QLabel("⚡ Priority: High");
    m_priorityLabel->setStyleSheet(
        "color: #FF5722; "
        "font-weight: bold; "
        "font-size: 10px; "
        "border: none; "
        "padding: 2px;"
    );
    
    metaLayout->addWidget(m_recommendationTypeLabel);
    metaLayout->addWidget(m_priorityLabel);
    metaLayout->addStretch();
    recLayout->addLayout(metaLayout);
    
    // Action buttons
    QHBoxLayout *actionLayout = new QHBoxLayout();
    actionLayout->setSpacing(10);
    
    m_approveBtn = new QPushButton("✓ Approve & Execute");
    m_approveBtn->setStyleSheet(
        "QPushButton {"
        "   background-color: #4CAF50;"
        "   color: white;"
        "   border: none;"
        "   border-radius: 5px;"
        "   padding: 8px 16px;"
        "   font-weight: bold;"
        "   font-size: 11px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #66BB6A;"
        "}"
    );
    m_approveBtn->setMinimumHeight(32);
    m_approveBtn->setMaximumHeight(34);
    connect(m_approveBtn, &QPushButton::clicked, this, &SecurityIntelligenceCenter::onApproveRecommendation);
    
    m_rejectBtn = new QPushButton("✗ Dismiss");
    m_rejectBtn->setStyleSheet(
        "QPushButton {"
        "   background-color: #F44336;"
        "   color: white;"
        "   border: none;"
        "   border-radius: 5px;"
        "   padding: 8px 16px;"
        "   font-weight: bold;"
        "   font-size: 11px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #EF5350;"
        "}"
    );
    m_rejectBtn->setMinimumHeight(32);
    m_rejectBtn->setMaximumHeight(34);
    connect(m_rejectBtn, &QPushButton::clicked, this, &SecurityIntelligenceCenter::onRejectRecommendation);
    
    actionLayout->addWidget(m_approveBtn, 2);
    actionLayout->addWidget(m_rejectBtn, 1);
    recLayout->addLayout(actionLayout);
    
    layout->addWidget(recFrame);
}

void SecurityIntelligenceCenter::createStabilityScorePanel()
{
    QFrame *card = new QFrame();
    card->setStyleSheet(getCardStyle());
    card->setMinimumHeight(300);
    card->setMaximumHeight(350);
    
    QVBoxLayout *layout = new QVBoxLayout(card);
    
    QLabel *title = new QLabel("5️⃣ City Stability & Risk Score");
    title->setStyleSheet("font-size: 16px; font-weight: bold; color: #00C853; padding: 5px;");
    layout->addWidget(title);
    
    // Score display
    QHBoxLayout *scoreLayout = new QHBoxLayout();
    
    QLabel *indexLabel = new QLabel("City Stability Index:");
    indexLabel->setStyleSheet("font-size: 14px; color: #CCCCCC; font-weight: bold;");
    scoreLayout->addWidget(indexLabel);
    
    m_stabilityScoreLabel = new QLabel(QString::number(m_currentStabilityScore));
    m_stabilityScoreLabel->setStyleSheet(
        "font-size: 42px; font-weight: bold; color: #00C853; padding: 5px;"
    );
    scoreLayout->addWidget(m_stabilityScoreLabel);
    
    scoreLayout->addStretch();
    layout->addLayout(scoreLayout);
    
    // Progress bar
    m_stabilityBar = new QProgressBar();
    m_stabilityBar->setRange(0, 100);
    m_stabilityBar->setValue(m_currentStabilityScore);
    m_stabilityBar->setTextVisible(true);
    m_stabilityBar->setFormat("%v / 100");
    m_stabilityBar->setMinimumHeight(25);
    m_stabilityBar->setMaximumHeight(30);
    m_stabilityBar->setStyleSheet(
        "QProgressBar {"
        "   border: 2px solid #2A2A2A;"
        "   border-radius: 5px;"
        "   background-color: #1E1E1E;"
        "   text-align: center;"
        "   color: white;"
        "   font-weight: bold;"
        "}"
        "QProgressBar::chunk {"
        "   background-color: #00C853;"
        "   border-radius: 3px;"
        "}"
    );
    layout->addWidget(m_stabilityBar);
    
    // Chart
    m_riskSeries = new QLineSeries();
    for (int i = 0; i < m_riskHistory.size(); ++i) {
        m_riskSeries->append(i, m_riskHistory[i]);
    }
    
    m_riskChart = new QChart();
    m_riskChart->addSeries(m_riskSeries);
    m_riskChart->setTitle("City Risk Trend – Last 24 Hours");
    m_riskChart->setTitleBrush(QBrush(QColor(COLOR_TEXT)));
    m_riskChart->setBackgroundBrush(QBrush(QColor(COLOR_PANEL)));
    m_riskChart->legend()->setVisible(false);
    
    QValueAxis *axisX = new QValueAxis();
    axisX->setRange(0, 24);
    axisX->setTitleText("Hours");
    axisX->setTitleBrush(QBrush(QColor(COLOR_TEXT_DIM)));
    axisX->setLabelFormat("%d");
    axisX->setLabelsColor(QColor(COLOR_TEXT_DIM));
    
    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0, 50);
    axisY->setTitleText("Risk Level");
    axisY->setTitleBrush(QBrush(QColor(COLOR_TEXT_DIM)));
    axisY->setLabelsColor(QColor(COLOR_TEXT_DIM));
    
    m_riskChart->addAxis(axisX, Qt::AlignBottom);
    m_riskChart->addAxis(axisY, Qt::AlignLeft);
    m_riskSeries->attachAxis(axisX);
    m_riskSeries->attachAxis(axisY);
    
    QPen seriesPen = QPen(QColor(COLOR_ACCENT));
    seriesPen.setWidth(3);
    m_riskSeries->setPen(seriesPen);
    
    m_chartView = new QChartView(m_riskChart);
    m_chartView->setRenderHint(QPainter::Antialiasing);
    m_chartView->setMinimumHeight(160);
    m_chartView->setMaximumHeight(180);
    
    layout->addWidget(m_chartView);
}

void SecurityIntelligenceCenter::createSimulationControlPanel()
{
    QFrame *card = new QFrame();
    card->setStyleSheet(getCardStyle());
    card->setMinimumHeight(300);
    card->setMaximumHeight(350);
    
    QVBoxLayout *layout = new QVBoxLayout(card);
    
    QLabel *title = new QLabel("6️⃣ Simulation & Control");
    title->setStyleSheet("font-size: 16px; font-weight: bold; color: #FF9800; padding: 5px;");
    layout->addWidget(title);
    
    m_simulationModeCheckbox = new QCheckBox("Enable Simulation Mode");
    m_simulationModeCheckbox->setStyleSheet(
        "QCheckBox {"
        "   color: #FFFFFF;"
        "   font-size: 13px;"
        "   font-weight: bold;"
        "   spacing: 8px;"
        "}"
        "QCheckBox::indicator {"
        "   width: 20px;"
        "   height: 20px;"
        "}"
    );
    connect(m_simulationModeCheckbox, &QCheckBox::toggled, this, &SecurityIntelligenceCenter::onSimulationModeToggled);
    layout->addWidget(m_simulationModeCheckbox);
    
    QLabel *simLabel = new QLabel("Scenario Triggers:");
    simLabel->setStyleSheet("color: #CCCCCC; font-weight: bold; margin-top: 10px;");
    layout->addWidget(simLabel);
    
    m_simSpeedBtn = new QPushButton("⚡ Simulate Speed Violations");
    m_simSpeedBtn->setStyleSheet(getButtonStyle(COLOR_WARNING));
    m_simSpeedBtn->setMinimumHeight(32);
    m_simSpeedBtn->setEnabled(false);
    connect(m_simSpeedBtn, &QPushButton::clicked, this, &SecurityIntelligenceCenter::onSimulateSpeedViolation);
    layout->addWidget(m_simSpeedBtn);
    
    m_simWasteBtn = new QPushButton("🗑️ Simulate Waste Overload");
    m_simWasteBtn->setStyleSheet(getButtonStyle(COLOR_WARNING));
    m_simWasteBtn->setMinimumHeight(32);
    m_simWasteBtn->setEnabled(false);
    connect(m_simWasteBtn, &QPushButton::clicked, this, &SecurityIntelligenceCenter::onSimulateWasteOverload);
    layout->addWidget(m_simWasteBtn);
    
    m_simLightingBtn = new QPushButton("💡 Simulate Lighting Failure");
    m_simLightingBtn->setStyleSheet(getButtonStyle(COLOR_WARNING));
    m_simLightingBtn->setMinimumHeight(32);
    m_simLightingBtn->setEnabled(false);
    connect(m_simLightingBtn, &QPushButton::clicked, this, &SecurityIntelligenceCenter::onSimulateLightingFailure);
    layout->addWidget(m_simLightingBtn);
    
    m_simCyberBtn = new QPushButton("🔓 Simulate Cyber Attack");
    m_simCyberBtn->setStyleSheet(getButtonStyle(COLOR_CRITICAL));
    m_simCyberBtn->setMinimumHeight(32);
    m_simCyberBtn->setEnabled(false);
    connect(m_simCyberBtn, &QPushButton::clicked, this, &SecurityIntelligenceCenter::onSimulateCyberAttack);
    layout->addWidget(m_simCyberBtn);
    
    layout->addSpacing(10);
    
    m_resetBtn = new QPushButton("🔄 Reset System State");
    m_resetBtn->setStyleSheet(getButtonStyle(COLOR_ACCENT));
    m_resetBtn->setMinimumHeight(35);
    connect(m_resetBtn, &QPushButton::clicked, this, &SecurityIntelligenceCenter::onResetSystemState);
    layout->addWidget(m_resetBtn);
    
    layout->addStretch();
}

void SecurityIntelligenceCenter::applyStyles()
{
    setStyleSheet(
        QString("QWidget {"
        "   background-color: %1;"
        "   color: %2;"
        "   font-family: 'Segoe UI', 'Roboto', sans-serif;"
        "}").arg(COLOR_BACKGROUND).arg(COLOR_TEXT)
    );
}

QString SecurityIntelligenceCenter::getCardStyle()
{
    return QString(
        "QFrame {"
        "   background-color: %1;"
        "   border: 1px solid %2;"
        "   border-radius: 10px;"
        "   padding: 15px;"
        "}"
    ).arg(COLOR_PANEL).arg(COLOR_BORDER);
}

QString SecurityIntelligenceCenter::getTableStyle()
{
    return QString(
        "QTableWidget {"
        "   background-color: %1;"
        "   alternate-background-color: #252525;"
        "   color: %2;"
        "   gridline-color: %3;"
        "   border: 1px solid %3;"
        "   border-radius: 5px;"
        "   selection-background-color: %4;"
        "}"
        "QHeaderView::section {"
        "   background-color: #2A2A2A;"
        "   color: %2;"
        "   padding: 8px;"
        "   border: none;"
        "   font-weight: bold;"
        "}"
    ).arg(COLOR_PANEL).arg(COLOR_TEXT).arg(COLOR_BORDER).arg(COLOR_ACCENT);
}

QString SecurityIntelligenceCenter::getButtonStyle(const QString &color)
{
    // Helper lambda to adjust color brightness
    auto adjustColor = [](const QString &colorStr, int amount) -> QString {
        QColor c(colorStr);
        int r = qBound(0, c.red() + amount, 255);
        int g = qBound(0, c.green() + amount, 255);
        int b = qBound(0, c.blue() + amount, 255);
        return QString("#%1%2%3")
            .arg(r, 2, 16, QChar('0'))
            .arg(g, 2, 16, QChar('0'))
            .arg(b, 2, 16, QChar('0'));
    };
    
    return QString(
        "QPushButton {"
        "   background-color: %1;"
        "   color: white;"
        "   border: none;"
        "   border-radius: 5px;"
        "   padding: 8px 16px;"
        "   font-weight: bold;"
        "   font-size: 12px;"
        "}"
        "QPushButton:hover {"
        "   background-color: %2;"
        "}"
        "QPushButton:pressed {"
        "   background-color: %3;"
        "}"
        "QPushButton:disabled {"
        "   background-color: #555555;"
        "   color: #888888;"
        "}"
    ).arg(color).arg(adjustColor(color, 20)).arg(adjustColor(color, -20));
}

void SecurityIntelligenceCenter::addLogEntry(const QString &level, const QString &message)
{
    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    QString colorCode;
    
    if (level == "INFO") colorCode = "#00FF00";
    else if (level == "WARNING") colorCode = "#FF9800";
    else if (level == "CRITICAL") colorCode = "#FF0000";
    else colorCode = "#FFFFFF";
    
    QString logLine = QString("<span style='color: %1;'>[%2] %3 - %4</span>")
        .arg(colorCode).arg(level).arg(timestamp).arg(message);
    
    m_securityLog->appendHtml(logLine);
    
    // Auto-scroll to bottom
    QScrollBar *scrollBar = m_securityLog->verticalScrollBar();
    scrollBar->setValue(scrollBar->maximum());
}

void SecurityIntelligenceCenter::addThreatAlert(const QString &type, const QString &module, 
                                                const QString &severity, const QString &timestamp)
{
    int row = m_threatDetectionTable->rowCount();
    m_threatDetectionTable->insertRow(row);
    
    m_threatDetectionTable->setItem(row, 0, new QTableWidgetItem(type));
    m_threatDetectionTable->setItem(row, 1, new QTableWidgetItem(module));
    
    QTableWidgetItem *severityItem = new QTableWidgetItem(severity);
    if (severity == "INFO") {
        severityItem->setForeground(QBrush(QColor(COLOR_ACCENT)));
    } else if (severity == "WARNING") {
        severityItem->setForeground(QBrush(QColor(COLOR_WARNING)));
    } else {
        severityItem->setForeground(QBrush(QColor(COLOR_CRITICAL)));
    }
    m_threatDetectionTable->setItem(row, 2, severityItem);
    
    m_threatDetectionTable->setItem(row, 3, new QTableWidgetItem(timestamp));
    
    // Scroll to bottom
    m_threatDetectionTable->scrollToBottom();
    
    // Emit signal
    int severityLevel = (severity == "CRITICAL") ? 3 : (severity == "WARNING") ? 2 : 1;
    emit threatDetected(module, severityLevel);
}

void SecurityIntelligenceCenter::updateCityStabilityScore(int score)
{
    m_currentStabilityScore = qBound(0, score, 100);
    m_stabilityScoreLabel->setText(QString::number(m_currentStabilityScore));
    m_stabilityBar->setValue(m_currentStabilityScore);
    
    // Update color based on score
    QString color;
    if (m_currentStabilityScore > 75) {
        color = COLOR_SUCCESS;
    } else if (m_currentStabilityScore >= 50) {
        color = COLOR_WARNING;
    } else {
        color = COLOR_CRITICAL;
    }
    
    m_stabilityScoreLabel->setStyleSheet(
        QString("font-size: 48px; font-weight: bold; color: %1; padding: 10px;").arg(color)
    );
    
    m_stabilityBar->setStyleSheet(
        QString("QProgressBar {"
        "   border: 2px solid #2A2A2A;"
        "   border-radius: 5px;"
        "   background-color: #1E1E1E;"
        "   text-align: center;"
        "   color: white;"
        "   font-weight: bold;"
        "}"
        "QProgressBar::chunk {"
        "   background-color: %1;"
        "   border-radius: 3px;"
        "}").arg(color)
    );
    
    // Add to history
    addRiskDataPoint(100.0 - m_currentStabilityScore);
    
    emit riskUpdated(m_currentStabilityScore);
}

void SecurityIntelligenceCenter::addRiskDataPoint(double value)
{
    m_riskHistory.removeFirst();
    m_riskHistory.append(value);
    
    m_riskSeries->clear();
    for (int i = 0; i < m_riskHistory.size(); ++i) {
        m_riskSeries->append(i, m_riskHistory[i]);
    }
}

int SecurityIntelligenceCenter::calculateCityStability()
{
    double score = 100.0;
    score -= (m_wasteRisk * 0.3);
    score -= (m_safetyRisk * 0.4);
    score -= (m_energyRisk * 0.3);
    score -= (m_cyberThreatLevel * 0.5);
    
    return qBound(0, static_cast<int>(score), 100);
}

void SecurityIntelligenceCenter::generateStrategicRecommendation()
{
    QStringList recommendations = {
        "Increase monitoring frequency for waste collection points in Zone A to prevent overflow incidents.",
        "Deploy additional pedestrian safety monitoring at high-traffic intersections during peak hours.",
        "Optimize lighting schedule to reduce energy consumption while maintaining safety standards.",
        "Implement enhanced authentication protocols for critical infrastructure access points.",
        "Schedule preventive maintenance for communication nodes showing intermittent connectivity.",
        "Activate backup power systems for smart lighting network to ensure continuous operation."
    };
    
    QStringList types = {"Preventive", "Operational", "Strategic"};
    QStringList priorities = {"Low", "Medium", "High"};
    
    int idx = QRandomGenerator::global()->bounded(recommendations.size());
    m_recommendationLabel->setText(recommendations[idx]);
    
    QString type = types[QRandomGenerator::global()->bounded(types.size())];
    QString priority = priorities[QRandomGenerator::global()->bounded(priorities.size())];
    
    m_recommendationTypeLabel->setText("Type: " + type);
    m_priorityLabel->setText("Priority: " + priority);
    
    QString typeColor = (type == "Preventive") ? COLOR_SUCCESS : 
                       (type == "Operational") ? COLOR_ACCENT : COLOR_WARNING;
    QString priorityColor = (priority == "Low") ? COLOR_SUCCESS : 
                           (priority == "Medium") ? COLOR_WARNING : COLOR_CRITICAL;
    
    m_recommendationTypeLabel->setStyleSheet(QString("color: %1; font-weight: bold;").arg(typeColor));
    m_priorityLabel->setStyleSheet(QString("color: %1; font-weight: bold;").arg(priorityColor));
    
    emit recommendationGenerated(recommendations[idx]);
}

// Slot implementations
void SecurityIntelligenceCenter::onSimulateSpeedViolation()
{
    addLogEntry("WARNING", "Speed violation detected at Crosswalk 3 - Vehicle: 85 km/h in 50 km/h zone");
    addThreatAlert("Speed Data Anomaly", "Pedestrian Safety", "WARNING", 
                   QDateTime::currentDateTime().toString("HH:mm:ss"));
    
    m_safetyRisk += 10;
    updateCityStabilityScore(calculateCityStability());
    generatePrediction();
}

void SecurityIntelligenceCenter::onSimulateWasteOverload()
{
    addLogEntry("CRITICAL", "Bin 8 overflow detected - Capacity exceeded by 35%");
    addThreatAlert("Waste Overload", "Recycling Infrastructure", "CRITICAL", 
                   QDateTime::currentDateTime().toString("HH:mm:ss"));
    
    m_wasteRisk += 15;
    updateCityStabilityScore(calculateCityStability());
    generatePrediction();
}

void SecurityIntelligenceCenter::onSimulateLightingFailure()
{
    addLogEntry("CRITICAL", "Lighting Pole 7 communication lost - Potential hardware failure");
    addThreatAlert("Sensor Offline", "Smart Lighting", "CRITICAL", 
                   QDateTime::currentDateTime().toString("HH:mm:ss"));
    
    m_energyRisk += 12;
    updateSystemHealth("Smart Lighting Network", "⚠ Warning", "High");
    updateCityStabilityScore(calculateCityStability());
    generatePrediction();
}

void SecurityIntelligenceCenter::onSimulateCyberAttack()
{
    addLogEntry("CRITICAL", "Suspicious login attempt detected from unknown IP - Potential intrusion");
    addThreatAlert("Unauthorized Access Attempt", "Authentication System", "CRITICAL", 
                   QDateTime::currentDateTime().toString("HH:mm:ss"));
    
    m_cyberThreatLevel += 20;
    updateSystemHealth("Authentication System", "⛔ Critical", "High");
    updateCityStabilityScore(calculateCityStability());
    generatePrediction();
}

void SecurityIntelligenceCenter::onResetSystemState()
{
    addLogEntry("INFO", "System reset initiated - Restoring baseline parameters");
    
    m_wasteRisk = 10;
    m_safetyRisk = 15;
    m_energyRisk = 8;
    m_cyberThreatLevel = 5;
    
    updateCityStabilityScore(calculateCityStability());
    
    // Reset system health table
    for (int i = 0; i < m_systemHealthTable->rowCount(); ++i) {
        QTableWidgetItem *statusItem = new QTableWidgetItem("✓ Operational");
        statusItem->setForeground(QBrush(QColor(COLOR_SUCCESS)));
        m_systemHealthTable->setItem(i, 1, statusItem);
        
        QTableWidgetItem *riskItem = new QTableWidgetItem("Low");
        riskItem->setForeground(QBrush(QColor(COLOR_SUCCESS)));
        m_systemHealthTable->setItem(i, 3, riskItem);
    }
    
    addLogEntry("INFO", "All systems restored to operational status");
}

void SecurityIntelligenceCenter::onClearAlerts()
{
    m_threatDetectionTable->setRowCount(0);
    addLogEntry("INFO", "Threat alerts cleared by operator");
}

void SecurityIntelligenceCenter::onTriggerSimulatedThreat()
{
    QStringList threats = {
        "Fake Speed Data Injection",
        "Unrealistic Waste Deposit",
        "Abnormal Energy Pattern",
        "Data Integrity Violation"
    };
    
    QStringList modules = {
        "Pedestrian Safety",
        "Recycling Infrastructure",
        "Smart Lighting",
        "Communication Layer"
    };
    
    int idx = QRandomGenerator::global()->bounded(threats.size());
    QString severity = QRandomGenerator::global()->bounded(2) ? "WARNING" : "CRITICAL";
    
    addThreatAlert(threats[idx], modules[idx], severity, 
                   QDateTime::currentDateTime().toString("HH:mm:ss"));
    addLogEntry(severity, QString("%1 detected in %2").arg(threats[idx]).arg(modules[idx]));
}

void SecurityIntelligenceCenter::onApproveRecommendation()
{
    addLogEntry("INFO", "Strategic recommendation approved by operator");
    addLogEntry("INFO", "Executing recommended action...");
    generateStrategicRecommendation();
}

void SecurityIntelligenceCenter::onRejectRecommendation()
{
    addLogEntry("WARNING", "Strategic recommendation rejected by operator");
    generateStrategicRecommendation();
}

void SecurityIntelligenceCenter::onSimulationModeToggled(bool enabled)
{
    m_simulationMode = enabled;
    
    m_simSpeedBtn->setEnabled(enabled);
    m_simWasteBtn->setEnabled(enabled);
    m_simLightingBtn->setEnabled(enabled);
    m_simCyberBtn->setEnabled(enabled);
    
    if (enabled) {
        addLogEntry("INFO", "Simulation mode activated - Training scenarios enabled");
    } else {
        addLogEntry("INFO", "Simulation mode deactivated - Live monitoring resumed");
    }
}

void SecurityIntelligenceCenter::updateRiskMetrics()
{
    // Gradually decrease risks over time (recovery)
    if (m_wasteRisk > 10) m_wasteRisk -= 1;
    if (m_safetyRisk > 15) m_safetyRisk -= 1;
    if (m_energyRisk > 8) m_energyRisk -= 1;
    if (m_cyberThreatLevel > 5) m_cyberThreatLevel -= 1;
    
    updateCityStabilityScore(calculateCityStability());
    
    // Random log entries
    if (QRandomGenerator::global()->bounded(10) < 2) {
        QStringList messages = {
            "Routine patrol scan completed - No anomalies",
            "Database backup completed successfully",
            "Network health check: All nodes responsive",
            "Sensor calibration verified for Zone C"
        };
        addLogEntry("INFO", messages[QRandomGenerator::global()->bounded(messages.size())]);
    }
}

void SecurityIntelligenceCenter::generatePrediction()
{
    QStringList predictions = {
        "Zone B lighting maintenance required within 48 hours",
        "Bin 15 approaching capacity threshold",
        "Increased pedestrian traffic predicted at Intersection 5",
        "Network latency spike anticipated during peak hours"
    };
    
    int row = m_predictiveTable->rowCount();
    if (row < 5) { // Limit predictions
        m_predictiveTable->insertRow(row);
        m_predictiveTable->setItem(row, 0, new QTableWidgetItem(
            predictions[QRandomGenerator::global()->bounded(predictions.size())]));
        m_predictiveTable->setItem(row, 1, new QTableWidgetItem(
            QString::number(60 + QRandomGenerator::global()->bounded(35)) + "%"));
        m_predictiveTable->setItem(row, 2, new QTableWidgetItem("Various"));
        m_predictiveTable->setItem(row, 3, new QTableWidgetItem("Monitor closely"));
    }
}

void SecurityIntelligenceCenter::updateSystemHealth(const QString &module, const QString &status, 
                                                    const QString &risk)
{
    for (int i = 0; i < m_systemHealthTable->rowCount(); ++i) {
        if (m_systemHealthTable->item(i, 0)->text() == module) {
            QTableWidgetItem *statusItem = new QTableWidgetItem(status);
            if (status.contains("✓")) {
                statusItem->setForeground(QBrush(QColor(COLOR_SUCCESS)));
            } else if (status.contains("⚠")) {
                statusItem->setForeground(QBrush(QColor(COLOR_WARNING)));
            } else {
                statusItem->setForeground(QBrush(QColor(COLOR_CRITICAL)));
            }
            m_systemHealthTable->setItem(i, 1, statusItem);
            
            QTableWidgetItem *riskItem = new QTableWidgetItem(risk);
            if (risk == "Low") {
                riskItem->setForeground(QBrush(QColor(COLOR_SUCCESS)));
            } else if (risk == "Medium") {
                riskItem->setForeground(QBrush(QColor(COLOR_WARNING)));
            } else {
                riskItem->setForeground(QBrush(QColor(COLOR_CRITICAL)));
            }
            m_systemHealthTable->setItem(i, 3, riskItem);
            
            break;
        }
    }
}
