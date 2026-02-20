#include "cityintelligencemodule.h"
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

CityIntelligenceModule::CityIntelligenceModule(QWidget *parent)
    : QWidget(parent)
    , m_cityStabilityScore(87)
    , m_currentRecommendationIndex(0)
    , m_infrastructureRisk(12)
    , m_environmentalRisk(18)
    , m_socialRisk(15)
    , m_economicRisk(10)
{
    setupUI();
    applyStyles();
    
    // Initialize risk forecast
    for (int i = 0; i < 48; ++i) {  // Next 48 hours
        m_riskForecast.append(15.0 + QRandomGenerator::global()->generateDouble() * 20.0);
    }
    
    // Initialize AI recommendations
    m_aiRecommendations << "Increase lighting intensity in high-traffic zones during peak hours to reduce safety incidents by 15%"
                       << "Deploy additional recycling bins in residential areas to improve participation by 20%"
                       << "Implement dynamic speed limits near school crosswalks during morning/afternoon peaks"
                       << "Optimize energy consumption by switching to Eco Mode during 2AM-5AM low-activity period"
                       << "Schedule preventive maintenance for streetlights in Zone B to avoid critical failures"
                       << "Enhance pedestrian crossing signals at CW-003 and CW-007 based on incident patterns";
    
    // Setup auto-update timer
    m_updateTimer = new QTimer(this);
    connect(m_updateTimer, &QTimer::timeout, this, &CityIntelligenceModule::updateIntelligenceData);
    m_updateTimer->start(9000); // Update every 9 seconds
    
    // Initial data
    generateNewRecommendation();
    addDecisionLog("🧠 City Intelligence Module initialized");
    addDecisionLog("📊 Analyzing city data patterns...");
}

CityIntelligenceModule::~CityIntelligenceModule()
{
}

void CityIntelligenceModule::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(25, 25, 25, 25);
    
    // Title
    QLabel *titleLabel = new QLabel("🧠 Smart City Intelligence & Decision Engine");
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: " + COLOR_ACCENT + "; padding: 10px;");
    mainLayout->addWidget(titleLabel);
    
    // Create scroll area
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setStyleSheet("QScrollArea { background-color: " + COLOR_BACKGROUND + "; border: none; }");
    
    QWidget *scrollContent = new QWidget();
    QVBoxLayout *contentLayout = new QVBoxLayout(scrollContent);
    contentLayout->setSpacing(20);
    contentLayout->setContentsMargins(0, 0, 10, 0);
    
    // City Stability Section
    createCityStabilitySection();
    contentLayout->addWidget(m_stabilityScoreLabel->parentWidget());
    
    // Main content grid
    QWidget *mainContentWidget = new QWidget();
    QHBoxLayout *mainContentLayout = new QHBoxLayout(mainContentWidget);
    mainContentLayout->setSpacing(20);
    mainContentLayout->setContentsMargins(0, 0, 0, 0);
    
    // Left: Risk Forecast Chart
    createRiskForecastChart();
    mainContentLayout->addWidget(m_riskChartView->parentWidget(), 1);
    
    // Right: Predictive Alerts
    createPredictiveAlertsTable();
    mainContentLayout->addWidget(m_predictiveAlertsTable->parentWidget(), 1);
    
    contentLayout->addWidget(mainContentWidget);
    
    // AI Recommendations Panel
    createAIRecommendationsPanel();
    contentLayout->addWidget(m_currentRecommendationLabel->parentWidget());
    
    // Decision Log
    createDecisionLog();
    contentLayout->addWidget(m_decisionLog->parentWidget());
    
    // Control Buttons
    QWidget *controlWidget = new QWidget();
    QHBoxLayout *controlLayout = new QHBoxLayout(controlWidget);
    controlLayout->setSpacing(15);
    
    m_predictBtn = new QPushButton("🔮 Generate Prediction");
    m_refreshBtn = new QPushButton("🔄 Refresh Forecasts");
    m_simulateBtn = new QPushButton("⚡ Run Scenario Simulation");
    
    connect(m_predictBtn, &QPushButton::clicked, this, &CityIntelligenceModule::onGeneratePrediction);
    connect(m_refreshBtn, &QPushButton::clicked, this, &CityIntelligenceModule::onRefreshForecasts);
    connect(m_simulateBtn, &QPushButton::clicked, this, &CityIntelligenceModule::onRunSimulation);
    
    m_predictBtn->setStyleSheet(getButtonStyle(COLOR_ACCENT));
    m_refreshBtn->setStyleSheet(getButtonStyle(COLOR_SUCCESS));
    m_simulateBtn->setStyleSheet(getButtonStyle(COLOR_WARNING));
    
    m_predictBtn->setMinimumHeight(45);
    m_refreshBtn->setMinimumHeight(45);
    m_simulateBtn->setMinimumHeight(45);
    
    controlLayout->addWidget(m_predictBtn);
    controlLayout->addWidget(m_refreshBtn);
    controlLayout->addWidget(m_simulateBtn);
    controlLayout->addStretch();
    
    contentLayout->addWidget(controlWidget);
    
    scrollArea->setWidget(scrollContent);
    mainLayout->addWidget(scrollArea);
}

void CityIntelligenceModule::createCityStabilitySection()
{
    QFrame *stabilityCard = new QFrame();
    stabilityCard->setStyleSheet(getCardStyle());
    stabilityCard->setMinimumHeight(180);
    
    QHBoxLayout *stabilityLayout = new QHBoxLayout(stabilityCard);
    stabilityLayout->setSpacing(30);
    stabilityLayout->setContentsMargins(25, 20, 25, 20);
    
    // Stability Score
    QVBoxLayout *scoreLayout = new QVBoxLayout();
    QLabel *scoreTitle = new QLabel("CITY STABILITY SCORE");
    scoreTitle->setStyleSheet("font-size: 13px; color: " + COLOR_TEXT_DIM + "; font-weight: bold;");
    m_stabilityScoreLabel = new QLabel("87");
    m_stabilityScoreLabel->setStyleSheet("font-size: 58px; font-weight: bold; color: " + COLOR_SUCCESS + ";");
    
    m_stabilityBar = new QProgressBar();
    m_stabilityBar->setRange(0, 100);
    m_stabilityBar->setValue(87);
    m_stabilityBar->setTextVisible(false);
    m_stabilityBar->setStyleSheet(
        "QProgressBar {"
        "    border: 2px solid " + COLOR_BORDER + ";"
        "    border-radius: 5px;"
        "    background-color: " + COLOR_PANEL + ";"
        "    height: 20px;"
        "}"
        "QProgressBar::chunk {"
        "    background-color: " + COLOR_SUCCESS + ";"
        "    border-radius: 3px;"
        "}"
    );
    
    m_stabilityStatusLabel = new QLabel("🟢 STABLE");
    m_stabilityStatusLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: " + COLOR_SUCCESS + ";");
    
    scoreLayout->addWidget(scoreTitle);
    scoreLayout->addWidget(m_stabilityScoreLabel);
    scoreLayout->addWidget(m_stabilityBar);
    scoreLayout->addWidget(m_stabilityStatusLabel);
    scoreLayout->addStretch();
    stabilityLayout->addLayout(scoreLayout, 1);
    
    // Risk Factors Breakdown
    QVBoxLayout *factorsLayout = new QVBoxLayout();
    QLabel *factorsTitle = new QLabel("RISK FACTORS ANALYSIS");
    factorsTitle->setStyleSheet("font-size: 13px; color: " + COLOR_TEXT_DIM + "; font-weight: bold;");
    factorsLayout->addWidget(factorsTitle);
    
    QGridLayout *factorsGrid = new QGridLayout();
    factorsGrid->setSpacing(12);
    
    QLabel *infraIcon = new QLabel("🏗️");
    infraIcon->setStyleSheet("font-size: 20px;");
    QLabel *infraLabel = new QLabel("Infrastructure:");
    infraLabel->setStyleSheet("font-size: 12px; color: " + COLOR_TEXT_DIM + ";");
    QLabel *infraValue = new QLabel(QString::number(m_infrastructureRisk) + "%");
    infraValue->setStyleSheet("font-size: 14px; font-weight: bold; color: " + COLOR_SUCCESS + ";");
    
    QLabel *envIcon = new QLabel("🌍");
    envIcon->setStyleSheet("font-size: 20px;");
    QLabel *envLabel = new QLabel("Environmental:");
    envLabel->setStyleSheet("font-size: 12px; color: " + COLOR_TEXT_DIM + ";");
    QLabel *envValue = new QLabel(QString::number(m_environmentalRisk) + "%");
    envValue->setStyleSheet("font-size: 14px; font-weight: bold; color: " + COLOR_WARNING + ";");
    
    QLabel *socialIcon = new QLabel("👥");
    socialIcon->setStyleSheet("font-size: 20px;");
    QLabel *socialLabel = new QLabel("Social:");
    socialLabel->setStyleSheet("font-size: 12px; color: " + COLOR_TEXT_DIM + ";");
    QLabel *socialValue = new QLabel(QString::number(m_socialRisk) + "%");
    socialValue->setStyleSheet("font-size: 14px; font-weight: bold; color: " + COLOR_WARNING + ";");
    
    QLabel *econIcon = new QLabel("💰");
    econIcon->setStyleSheet("font-size: 20px;");
    QLabel *econLabel = new QLabel("Economic:");
    econLabel->setStyleSheet("font-size: 12px; color: " + COLOR_TEXT_DIM + ";");
    QLabel *econValue = new QLabel(QString::number(m_economicRisk) + "%");
    econValue->setStyleSheet("font-size: 14px; font-weight: bold; color: " + COLOR_SUCCESS + ";");
    
    factorsGrid->addWidget(infraIcon, 0, 0);
    factorsGrid->addWidget(infraLabel, 0, 1);
    factorsGrid->addWidget(infraValue, 0, 2);
    
    factorsGrid->addWidget(envIcon, 1, 0);
    factorsGrid->addWidget(envLabel, 1, 1);
    factorsGrid->addWidget(envValue, 1, 2);
    
    factorsGrid->addWidget(socialIcon, 2, 0);
    factorsGrid->addWidget(socialLabel, 2, 1);
    factorsGrid->addWidget(socialValue, 2, 2);
    
    factorsGrid->addWidget(econIcon, 3, 0);
    factorsGrid->addWidget(econLabel, 3, 1);
    factorsGrid->addWidget(econValue, 3, 2);
    
    factorsLayout->addLayout(factorsGrid);
    factorsLayout->addStretch();
    stabilityLayout->addLayout(factorsLayout, 1);
    
    // Overall Assessment
    QVBoxLayout *assessmentLayout = new QVBoxLayout();
    QLabel *assessmentTitle = new QLabel("AI ASSESSMENT");
    assessmentTitle->setStyleSheet("font-size: 13px; color: " + COLOR_TEXT_DIM + "; font-weight: bold;");
    QLabel *assessmentText = new QLabel("City operations are performing within optimal parameters. Minor adjustments recommended for environmental and social factors.");
    assessmentText->setWordWrap(true);
    assessmentText->setStyleSheet("font-size: 13px; color: " + COLOR_TEXT + "; line-height: 1.5;");
    assessmentLayout->addWidget(assessmentTitle);
    assessmentLayout->addWidget(assessmentText);
    assessmentLayout->addStretch();
    stabilityLayout->addLayout(assessmentLayout, 1);
}

void CityIntelligenceModule::createPredictiveAlertsTable()
{
    QFrame *tableCard = new QFrame();
    tableCard->setStyleSheet(getCardStyle());
    
    QVBoxLayout *tableLayout = new QVBoxLayout(tableCard);
    tableLayout->setContentsMargins(20, 20, 20, 20);
    
    QLabel *tableTitle = new QLabel("🔮 Predictive Intelligence Alerts");
    tableTitle->setStyleSheet("font-size: 16px; font-weight: bold; color: " + COLOR_TEXT + ";");
    tableLayout->addWidget(tableTitle);
    
    m_predictiveAlertsTable = new QTableWidget();
    m_predictiveAlertsTable->setColumnCount(4);
    m_predictiveAlertsTable->setHorizontalHeaderLabels({"Category", "Prediction", "Probability", "Timeframe"});
    m_predictiveAlertsTable->horizontalHeader()->setStretchLastSection(true);
    m_predictiveAlertsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_predictiveAlertsTable->verticalHeader()->setVisible(false);
    m_predictiveAlertsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_predictiveAlertsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    m_predictiveAlertsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_predictiveAlertsTable->setStyleSheet(getTableStyle());
    m_predictiveAlertsTable->setMinimumHeight(280);
    
    // Populate with predictive alerts
    addPredictiveAlert("Recycling", "Bin overflow in Zone A", "78%", "Next 6 hours");
    addPredictiveAlert("Safety", "Increased pedestrian activity", "85%", "Next 2 hours");
    addPredictiveAlert("Lighting", "Energy peak demand", "92%", "Next 4 hours");
    addPredictiveAlert("Safety", "Speed violation probability", "67%", "Next 3 hours");
    addPredictiveAlert("Infrastructure", "Maintenance required POLE-008", "88%", "Next 24 hours");
    addPredictiveAlert("Environmental", "High waste generation", "74%", "Next 12 hours");
    
    tableLayout->addWidget(m_predictiveAlertsTable);
}

void CityIntelligenceModule::createRiskForecastChart()
{
    QFrame *chartCard = new QFrame();
    chartCard->setStyleSheet(getCardStyle());
    
    QVBoxLayout *chartLayout = new QVBoxLayout(chartCard);
    chartLayout->setContentsMargins(20, 20, 20, 20);
    
    QLabel *chartTitle = new QLabel("📈 City Risk Forecast (48 Hours)");
    chartTitle->setStyleSheet("font-size: 16px; font-weight: bold; color: " + COLOR_TEXT + ";");
    chartLayout->addWidget(chartTitle);
    
    // Create line chart
    m_riskSeries = new QLineSeries();
    for (int i = 0; i < m_riskForecast.size(); ++i) {
        m_riskSeries->append(i, m_riskForecast[i]);
    }
    
    QPen pen{QColor(COLOR_WARNING)};
    pen.setWidth(3);
    m_riskSeries->setPen(pen);
    
    m_riskChart = new QChart();
    m_riskChart->addSeries(m_riskSeries);
    m_riskChart->setTitle("");
    m_riskChart->setAnimationOptions(QChart::SeriesAnimations);
    m_riskChart->setBackgroundBrush(QBrush(QColor(COLOR_PANEL)));
    
    QValueAxis *axisX = new QValueAxis();
    axisX->setRange(0, 47);
    axisX->setLabelFormat("%d");
    axisX->setTitleText("Hours Ahead");
    axisX->setLabelsColor(QColor(COLOR_TEXT));
    axisX->setTitleBrush(QBrush(QColor(COLOR_TEXT)));
    m_riskChart->addAxis(axisX, Qt::AlignBottom);
    m_riskSeries->attachAxis(axisX);
    
    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0, 50);
    axisY->setLabelFormat("%d");
    axisY->setTitleText("Risk Level");
    axisY->setLabelsColor(QColor(COLOR_TEXT));
    axisY->setTitleBrush(QBrush(QColor(COLOR_TEXT)));
    m_riskChart->addAxis(axisY, Qt::AlignLeft);
    m_riskSeries->attachAxis(axisY);
    
    m_riskChart->legend()->setVisible(false);
    
    m_riskChartView = new QChartView(m_riskChart);
    m_riskChartView->setRenderHint(QPainter::Antialiasing);
    m_riskChartView->setMinimumHeight(280);
    
    chartLayout->addWidget(m_riskChartView);
    
    QLabel *forecastNote = new QLabel("📊 Forecast shows moderate risk increase during evening peak hours");
    forecastNote->setStyleSheet("font-size: 12px; color: " + COLOR_TEXT_DIM + "; font-style: italic;");
    chartLayout->addWidget(forecastNote);
}

void CityIntelligenceModule::createAIRecommendationsPanel()
{
    QFrame *recommendCard = new QFrame();
    recommendCard->setStyleSheet(getCardStyle());
    recommendCard->setMinimumHeight(150);
    
    QVBoxLayout *cardLayout = new QVBoxLayout(recommendCard);
    cardLayout->setSpacing(12);
    cardLayout->setContentsMargins(25, 20, 25, 20);
    
    QLabel *panelTitle = new QLabel("💡 AI Strategic Recommendations");
    panelTitle->setStyleSheet("font-size: 16px; font-weight: bold; color: " + COLOR_ACCENT + ";");
    cardLayout->addWidget(panelTitle);
    
    m_currentRecommendationLabel = new QLabel("");
    m_currentRecommendationLabel->setWordWrap(true);
    m_currentRecommendationLabel->setStyleSheet("font-size: 14px; color: " + COLOR_TEXT + "; padding: 10px; background-color: " + COLOR_BACKGROUND + "; border-radius: 5px;");
    cardLayout->addWidget(m_currentRecommendationLabel);
    
    QHBoxLayout *metaLayout = new QHBoxLayout();
    
    m_confidenceLabel = new QLabel("Confidence: 92%");
    m_confidenceLabel->setStyleSheet("font-size: 12px; color: " + COLOR_SUCCESS + "; font-weight: bold;");
    
    m_impactLabel = new QLabel("Impact: High");
    m_impactLabel->setStyleSheet("font-size: 12px; color: " + COLOR_WARNING + "; font-weight: bold;");
    
    metaLayout->addWidget(m_confidenceLabel);
    metaLayout->addWidget(m_impactLabel);
    metaLayout->addStretch();
    
    cardLayout->addLayout(metaLayout);
    
    QHBoxLayout *actionsLayout = new QHBoxLayout();
    
    m_applyBtn = new QPushButton("✅ Apply Recommendation");
    m_ignoreBtn = new QPushButton("❌ Ignore");
    
    connect(m_applyBtn, &QPushButton::clicked, this, &CityIntelligenceModule::onApplyRecommendation);
    connect(m_ignoreBtn, &QPushButton::clicked, this, &CityIntelligenceModule::onIgnoreRecommendation);
    
    m_applyBtn->setStyleSheet(getButtonStyle(COLOR_SUCCESS));
    m_ignoreBtn->setStyleSheet(getButtonStyle(COLOR_CRITICAL));
    
    m_applyBtn->setMinimumHeight(40);
    m_ignoreBtn->setMinimumHeight(40);
    
    actionsLayout->addWidget(m_applyBtn);
    actionsLayout->addWidget(m_ignoreBtn);
    actionsLayout->addStretch();
    
    cardLayout->addLayout(actionsLayout);
}

void CityIntelligenceModule::createDecisionLog()
{
    QFrame *logCard = new QFrame();
    logCard->setStyleSheet(getCardStyle());
    
    QVBoxLayout *logLayout = new QVBoxLayout(logCard);
    logLayout->setContentsMargins(20, 20, 20, 20);
    
    QLabel *logTitle = new QLabel("📝 AI Decision Log");
    logTitle->setStyleSheet("font-size: 16px; font-weight: bold; color: " + COLOR_TEXT + ";");
    logLayout->addWidget(logTitle);
    
    m_decisionLog = new QPlainTextEdit();
    m_decisionLog->setReadOnly(true);
    m_decisionLog->setStyleSheet(
        "QPlainTextEdit {"
        "    background-color: " + COLOR_BACKGROUND + ";"
        "    color: " + COLOR_TEXT + ";"
        "    border: 1px solid " + COLOR_BORDER + ";"
        "    border-radius: 5px;"
        "    padding: 10px;"
        "    font-family: 'Consolas', 'Courier New', monospace;"
        "    font-size: 12px;"
        "}"
    );
    m_decisionLog->setMinimumHeight(200);
    m_decisionLog->setMaximumHeight(250);
    
    logLayout->addWidget(m_decisionLog);
}

void CityIntelligenceModule::updateIntelligenceData()
{
    // Update city stability score
    int change = QRandomGenerator::global()->bounded(-2, 3);
    m_cityStabilityScore = qBound(70, m_cityStabilityScore + change, 100);
    updateCityStability();
    
    // Update risk forecast
    addRiskDataPoint();
    
    // Occasionally generate new predictions
    if (QRandomGenerator::global()->bounded(100) < 20) { // 20% chance
        addDecisionLog("🔮 New prediction generated based on pattern analysis");
    }
}

void CityIntelligenceModule::updateCityStability()
{
    m_stabilityScoreLabel->setText(QString::number(m_cityStabilityScore));
    m_stabilityBar->setValue(m_cityStabilityScore);
    
    QString status;
    QString color;
    QString icon;
    
    if (m_cityStabilityScore >= 85) {
        status = "STABLE";
        color = COLOR_SUCCESS;
        icon = "🟢";
    } else if (m_cityStabilityScore >= 70) {
        status = "MODERATE";
        color = COLOR_WARNING;
        icon = "🟡";
    } else {
        status = "AT RISK";
        color = COLOR_CRITICAL;
        icon = "🔴";
    }
    
    m_stabilityStatusLabel->setText(icon + " " + status);
    m_stabilityStatusLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: " + color + ";");
    m_stabilityScoreLabel->setStyleSheet("font-size: 58px; font-weight: bold; color: " + color + ";");
    m_stabilityBar->setStyleSheet(
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
    
    emit cityStabilityChanged(m_cityStabilityScore);
}

void CityIntelligenceModule::addPredictiveAlert(const QString &category, const QString &prediction,
                                                const QString &probability, const QString &timeframe)
{
    int row = m_predictiveAlertsTable->rowCount();
    m_predictiveAlertsTable->insertRow(row);
    
    m_predictiveAlertsTable->setItem(row, 0, new QTableWidgetItem(category));
    m_predictiveAlertsTable->setItem(row, 1, new QTableWidgetItem(prediction));
    m_predictiveAlertsTable->setItem(row, 2, new QTableWidgetItem(probability));
    m_predictiveAlertsTable->setItem(row, 3, new QTableWidgetItem(timeframe));
    
    // Color-code by probability
    int prob = probability.left(probability.indexOf('%')).toInt();
    QString probColor;
    if (prob >= 80) probColor = COLOR_CRITICAL;
    else if (prob >= 60) probColor = COLOR_WARNING;
    else probColor = COLOR_SUCCESS;
    
    m_predictiveAlertsTable->item(row, 2)->setForeground(QBrush(QColor(probColor)));
}

void CityIntelligenceModule::generateNewRecommendation()
{
    if (m_aiRecommendations.isEmpty()) return;
    
    QString recommendation = m_aiRecommendations[m_currentRecommendationIndex];
    m_currentRecommendationLabel->setText("💡 " + recommendation);
    
    int confidence = 75 + QRandomGenerator::global()->bounded(20);
    m_confidenceLabel->setText("Confidence: " + QString::number(confidence) + "%");
    
    QStringList impacts = {"Low", "Medium", "High"};
    QString impact = impacts[QRandomGenerator::global()->bounded(3)];
    m_impactLabel->setText("Impact: " + impact);
    
    QString impactColor = (impact == "High") ? COLOR_CRITICAL : (impact == "Medium") ? COLOR_WARNING : COLOR_SUCCESS;
    m_impactLabel->setStyleSheet("font-size: 12px; color: " + impactColor + "; font-weight: bold;");
}

void CityIntelligenceModule::addRiskDataPoint()
{
    // Shift forecast forward
    m_riskForecast.removeFirst();
    m_riskForecast.append(15.0 + QRandomGenerator::global()->generateDouble() * 20.0);
    
    // Update chart
    m_riskSeries->clear();
    for (int i = 0; i < m_riskForecast.size(); ++i) {
        m_riskSeries->append(i, m_riskForecast[i]);
    }
}

void CityIntelligenceModule::addDecisionLog(const QString &decision)
{
    QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");
    m_decisionLog->appendPlainText("[" + timestamp + "] " + decision);
}

void CityIntelligenceModule::onGeneratePrediction()
{
    QStringList categories = {"Recycling", "Safety", "Lighting", "Infrastructure"};
    QString category = categories[QRandomGenerator::global()->bounded(categories.size())];
    int severity = QRandomGenerator::global()->bounded(1, 4);
    
    addDecisionLog("🔮 Generated prediction for " + category + " (Severity: " + QString::number(severity) + ")");
    emit predictionGenerated(category, severity);
}

void CityIntelligenceModule::onRefreshForecasts()
{
    // Regenerate risk forecast
    m_riskForecast.clear();
    for (int i = 0; i < 48; ++i) {
        m_riskForecast.append(15.0 + QRandomGenerator::global()->generateDouble() * 20.0);
    }
    
    addRiskDataPoint();
    addDecisionLog("🔄 Risk forecasts refreshed with latest data models");
}

void CityIntelligenceModule::onApplyRecommendation()
{
    QString recommendation = m_currentRecommendationLabel->text();
    addDecisionLog("✅ APPLIED: " + recommendation.mid(2)); // Remove emoji
    
    // Move to next recommendation
    m_currentRecommendationIndex = (m_currentRecommendationIndex + 1) % m_aiRecommendations.size();
    generateNewRecommendation();
    
    emit recommendationIssued(recommendation);
}

void CityIntelligenceModule::onIgnoreRecommendation()
{
    QString recommendation = m_currentRecommendationLabel->text();
    addDecisionLog("❌ IGNORED: " + recommendation.mid(2));
    
    // Move to next recommendation
    m_currentRecommendationIndex = (m_currentRecommendationIndex + 1) % m_aiRecommendations.size();
    generateNewRecommendation();
}

void CityIntelligenceModule::onRunSimulation()
{
    addDecisionLog("⚡ Running scenario simulation...");
    addDecisionLog("📊 Simulating peak traffic conditions...");
    addDecisionLog("🧪 Testing system resilience under stress...");
    addDecisionLog("✅ Simulation complete - System stable under projected conditions");
}

QString CityIntelligenceModule::getCardStyle()
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

QString CityIntelligenceModule::getTableStyle()
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

QString CityIntelligenceModule::getButtonStyle(const QString &color)
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

void CityIntelligenceModule::applyStyles()
{
    setStyleSheet("background-color: " + COLOR_BACKGROUND + ";");
}
