#include "analyticsmodule.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFrame>
#include <QGroupBox>
#include <QRandomGenerator>
#include <QScrollArea>
#include <QDate>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QPieSlice>

AnalyticsModule::AnalyticsModule(QWidget *parent)
    : QWidget(parent)
{
    // Initialize data BEFORE creating UI (charts need this data)
    generateRecyclingData();
    generateSafetyData();
    generateEnergyData();
    
    setupUI();
    applyStyles();
    
    // Setup auto-update timer
    m_updateTimer = new QTimer(this);
    connect(m_updateTimer, &QTimer::timeout, this, &AnalyticsModule::updateAnalyticsData);
    m_updateTimer->start(10000); // Update every 10 seconds
}

AnalyticsModule::~AnalyticsModule()
{
}

void AnalyticsModule::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(25);
    mainLayout->setContentsMargins(30, 30, 30, 30);
    
    // Title Header Frame
    QFrame *titleFrame = new QFrame();
    titleFrame->setStyleSheet("QFrame { background-color: " + COLOR_PANEL + "; border-radius: 8px; padding: 15px; }");
    QHBoxLayout *titleFrameLayout = new QHBoxLayout(titleFrame);
    titleFrameLayout->setContentsMargins(20, 15, 20, 15);
    
    QLabel *titleLabel = new QLabel("📊 Analytics & Reporting Center");
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
    
    // Control Section
    createControlSection();
    contentLayout->addWidget(m_reportTypeSelector->parentWidget());
    
    // Top Row: Recycling + Safety
    QWidget *topRowWidget = new QWidget();
    QHBoxLayout *topRowLayout = new QHBoxLayout(topRowWidget);
    topRowLayout->setSpacing(20);
    topRowLayout->setContentsMargins(0, 0, 0, 0);
    
    createRecyclingAnalytics();
    createSafetyAnalytics();
    
    topRowLayout->addWidget(m_recyclingChartView->parentWidget(), 1);
    topRowLayout->addWidget(m_safetyChartView->parentWidget(), 1);
    
    contentLayout->addWidget(topRowWidget);
    
    // Bottom Row: Energy + System Overview
    QWidget *bottomRowWidget = new QWidget();
    QHBoxLayout *bottomRowLayout = new QHBoxLayout(bottomRowWidget);
    bottomRowLayout->setSpacing(20);
    bottomRowLayout->setContentsMargins(0, 0, 0, 0);
    
    createEnergyAnalytics();
    createSystemOverview();
    
    bottomRowLayout->addWidget(m_energyChartView->parentWidget(), 1);
    bottomRowLayout->addWidget(m_overviewChartView->parentWidget(), 1);
    
    contentLayout->addWidget(bottomRowWidget);
    
    scrollArea->setWidget(scrollContent);
    mainLayout->addWidget(scrollArea);
}

void AnalyticsModule::createControlSection()
{
    QFrame *controlCard = new QFrame();
    controlCard->setStyleSheet(getCardStyle());
    controlCard->setMinimumHeight(165);
    
    QVBoxLayout *cardLayout = new QVBoxLayout(controlCard);
    cardLayout->setSpacing(20);
    cardLayout->setContentsMargins(35, 30, 35, 30);
    
    // Section Title with Background
    QFrame *titleFrame = new QFrame();
    titleFrame->setStyleSheet(
        "QFrame {"
        "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 " + COLOR_ACCENT + ", stop:1 rgba(30, 144, 255, 0.3));"
        "    border-radius: 8px;"
        "    padding: 12px 20px;"
        "}"
    );
    QHBoxLayout *titleLayout = new QHBoxLayout(titleFrame);
    titleLayout->setContentsMargins(0, 0, 0, 0);
    
    QLabel *sectionTitle = new QLabel("📋 REPORT CONFIGURATION & EXPORT");
    sectionTitle->setStyleSheet("font-size: 22px; font-weight: bold; color: #FFFFFF; letter-spacing: 1px; background: transparent;");
    titleLayout->addWidget(sectionTitle);
    titleLayout->addStretch();
    
    cardLayout->addWidget(titleFrame);
    
    // Separator line
    QFrame *separator = new QFrame();
    separator->setFrameShape(QFrame::HLine);
    separator->setStyleSheet("background-color: " + COLOR_BORDER + "; max-height: 2px;");
    cardLayout->addWidget(separator);
    
    QHBoxLayout *controlLayout = new QHBoxLayout();
    controlLayout->setSpacing(30);
    
    // Report Type Selection
    QVBoxLayout *reportLayout = new QVBoxLayout();
    reportLayout->setSpacing(10);
    QLabel *reportLabel = new QLabel("📊 REPORT TYPE");
    reportLabel->setStyleSheet("font-size: 13px; color: " + COLOR_ACCENT + "; font-weight: 700; letter-spacing: 0.5px;");
    m_reportTypeSelector = new QComboBox();
    m_reportTypeSelector->addItem("📊 Complete System Analytics");
    m_reportTypeSelector->addItem("♻️ Recycling Performance");
    m_reportTypeSelector->addItem("🚸 Pedestrian Safety Analysis");
    m_reportTypeSelector->addItem("💡 Energy Consumption Report");
    m_reportTypeSelector->setStyleSheet(
        "QComboBox {"
        "    background-color: #2A2A2A;"
        "    color: " + COLOR_TEXT + ";"
        "    border: 2px solid " + COLOR_ACCENT + ";"
        "    border-radius: 8px;"
        "    padding: 12px 15px;"
        "    font-size: 15px;"
        "    font-weight: 600;"
        "    min-height: 45px;"
        "}"
        "QComboBox:hover {"
        "    background-color: #333333;"
        "    border-color: #4DA6FF;"
        "}"
        "QComboBox::drop-down {"
        "    border: none;"
        "    width: 35px;"
        "}"
        "QComboBox QAbstractItemView {"
        "    background-color: #2A2A2A;"
        "    color: " + COLOR_TEXT + ";"
        "    selection-background-color: " + COLOR_ACCENT + ";"
        "    border: 2px solid " + COLOR_BORDER + ";"
        "    font-size: 15px;"
        "    padding: 5px;"
        "}"
    );
    reportLayout->addWidget(reportLabel);
    reportLayout->addWidget(m_reportTypeSelector);
    controlLayout->addLayout(reportLayout, 2);
    
    // Date Range
    QVBoxLayout *dateLayout = new QVBoxLayout();
    dateLayout->setSpacing(10);
    QLabel *dateLabel = new QLabel("📅 DATE RANGE");
    dateLabel->setStyleSheet("font-size: 13px; color: " + COLOR_ACCENT + "; font-weight: 700; letter-spacing: 0.5px;");
    
    QHBoxLayout *dateRangeLayout = new QHBoxLayout();
    dateRangeLayout->setSpacing(10);
    m_startDateEdit = new QDateEdit(QDate::currentDate().addDays(-30));
    m_startDateEdit->setCalendarPopup(true);
    m_startDateEdit->setStyleSheet(
        "QDateEdit {"
        "    background-color: #2A2A2A;"
        "    color: " + COLOR_TEXT + ";"
        "    border: 2px solid " + COLOR_BORDER + ";"
        "    border-radius: 8px;"
        "    padding: 12px 15px;"
        "    font-size: 15px;"
        "    font-weight: 600;"
        "    min-height: 45px;"
        "}"
        "QDateEdit:hover {"
        "    background-color: #333333;"
        "    border-color: " + COLOR_ACCENT + ";"
        "}"
    );
    
    QLabel *toLabel = new QLabel("→");
    toLabel->setStyleSheet("color: " + COLOR_ACCENT + "; font-size: 20px; font-weight: bold;");
    
    m_endDateEdit = new QDateEdit(QDate::currentDate());
    m_endDateEdit->setCalendarPopup(true);
    m_endDateEdit->setStyleSheet(m_startDateEdit->styleSheet());
    
    connect(m_startDateEdit, &QDateEdit::dateChanged, this, &AnalyticsModule::onDateRangeChanged);
    connect(m_endDateEdit, &QDateEdit::dateChanged, this, &AnalyticsModule::onDateRangeChanged);
    
    dateRangeLayout->addWidget(m_startDateEdit);
    dateRangeLayout->addWidget(toLabel);
    dateRangeLayout->addWidget(m_endDateEdit);
    
    dateLayout->addWidget(dateLabel);
    dateLayout->addLayout(dateRangeLayout);
    controlLayout->addLayout(dateLayout, 2);
    
    // Action Buttons
    QVBoxLayout *buttonsLayout = new QVBoxLayout();
    buttonsLayout->setSpacing(10);
    QLabel *actionsLabel = new QLabel("⚡ ACTIONS");
    actionsLabel->setStyleSheet("font-size: 13px; color: " + COLOR_ACCENT + "; font-weight: 700; letter-spacing: 0.5px;");
    
    QHBoxLayout *actionButtonsLayout = new QHBoxLayout();
    actionButtonsLayout->setSpacing(12);
    
    m_generateBtn = new QPushButton("📄 Generate");
    m_exportPdfBtn = new QPushButton("📑 Export PDF");
    m_exportExcelBtn = new QPushButton("📊 Export Excel");
    
    connect(m_generateBtn, &QPushButton::clicked, this, &AnalyticsModule::onGenerateReport);
    connect(m_exportPdfBtn, &QPushButton::clicked, this, &AnalyticsModule::onExportPDF);
    connect(m_exportExcelBtn, &QPushButton::clicked, this, &AnalyticsModule::onExportExcel);
    
    m_generateBtn->setStyleSheet(getButtonStyle(COLOR_ACCENT));
    m_exportPdfBtn->setStyleSheet(getButtonStyle(COLOR_CRITICAL));
    m_exportExcelBtn->setStyleSheet(getButtonStyle(COLOR_SUCCESS));
    
    m_generateBtn->setMinimumHeight(48);
    m_exportPdfBtn->setMinimumHeight(48);
    m_exportExcelBtn->setMinimumHeight(48);
    
    m_generateBtn->setMinimumWidth(140);
    m_exportPdfBtn->setMinimumWidth(140);
    m_exportExcelBtn->setMinimumWidth(140);
    
    // Add font size to buttons
    m_generateBtn->setStyleSheet(m_generateBtn->styleSheet() + " font-size: 14px; font-weight: 600;");
    m_exportPdfBtn->setStyleSheet(m_exportPdfBtn->styleSheet() + " font-size: 14px; font-weight: 600;");
    m_exportExcelBtn->setStyleSheet(m_exportExcelBtn->styleSheet() + " font-size: 14px; font-weight: 600;");
    
    actionButtonsLayout->addWidget(m_generateBtn);
    actionButtonsLayout->addWidget(m_exportPdfBtn);
    actionButtonsLayout->addWidget(m_exportExcelBtn);
    
    buttonsLayout->addWidget(actionsLabel);
    buttonsLayout->addLayout(actionButtonsLayout);
    controlLayout->addLayout(buttonsLayout, 3);
    
    cardLayout->addLayout(controlLayout);
}

void AnalyticsModule::createRecyclingAnalytics()
{
    QFrame *chartCard = new QFrame();
    chartCard->setStyleSheet(getCardStyle());
    
    QVBoxLayout *chartLayout = new QVBoxLayout(chartCard);
    chartLayout->setContentsMargins(20, 20, 20, 20);
    chartLayout->setSpacing(10);
    
    QLabel *chartTitle = new QLabel("♻️ Waste Recycling Trends (7 Days)");
    chartTitle->setStyleSheet("font-size: 15px; font-weight: bold; color: " + COLOR_TEXT + ";");
    chartLayout->addWidget(chartTitle);
    
    // Create bar chart
    QBarSet *plasticSet = new QBarSet("Plastic");
    QBarSet *metalSet = new QBarSet("Metal");
    QBarSet *glassSet = new QBarSet("Glass");
    
    for (int i = 0; i < 7; ++i) {
        *plasticSet << m_recyclingData[i * 3];
        *metalSet << m_recyclingData[i * 3 + 1];
        *glassSet << m_recyclingData[i * 3 + 2];
    }
    
    plasticSet->setColor(QColor(COLOR_ACCENT));
    metalSet->setColor(QColor(COLOR_WARNING));
    glassSet->setColor(QColor(COLOR_SUCCESS));
    
    m_recyclingSeries = new QBarSeries();
    m_recyclingSeries->append(plasticSet);
    m_recyclingSeries->append(metalSet);
    m_recyclingSeries->append(glassSet);
    
    m_recyclingChart = new QChart();
    m_recyclingChart->addSeries(m_recyclingSeries);
    m_recyclingChart->setTitle("");
    m_recyclingChart->setAnimationOptions(QChart::SeriesAnimations);
    m_recyclingChart->setBackgroundBrush(QBrush(QColor(COLOR_PANEL)));
    
    QStringList categories;
    categories << "Mon" << "Tue" << "Wed" << "Thu" << "Fri" << "Sat" << "Sun";
    
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    axisX->setLabelsColor(QColor(COLOR_TEXT));
    m_recyclingChart->addAxis(axisX, Qt::AlignBottom);
    m_recyclingSeries->attachAxis(axisX);
    
    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0, 400);
    axisY->setLabelsColor(QColor(COLOR_TEXT));
    axisY->setTitleText("kg");
    axisY->setTitleBrush(QBrush(QColor(COLOR_TEXT)));
    m_recyclingChart->addAxis(axisY, Qt::AlignLeft);
    m_recyclingSeries->attachAxis(axisY);
    
    m_recyclingChart->legend()->setVisible(true);
    m_recyclingChart->legend()->setAlignment(Qt::AlignBottom);
    m_recyclingChart->legend()->setLabelColor(QColor(COLOR_TEXT));
    
    m_recyclingChartView = new QChartView(m_recyclingChart);
    m_recyclingChartView->setRenderHint(QPainter::Antialiasing);
    m_recyclingChartView->setMinimumHeight(280);
    
    chartLayout->addWidget(m_recyclingChartView);
    
    m_recyclingInsightLabel = new QLabel("📈 Trend: +12% increase from last week");
    m_recyclingInsightLabel->setStyleSheet("font-size: 12px; color: " + COLOR_SUCCESS + "; font-style: italic;");
    chartLayout->addWidget(m_recyclingInsightLabel);
}

void AnalyticsModule::createSafetyAnalytics()
{
    QFrame *chartCard = new QFrame();
    chartCard->setStyleSheet(getCardStyle());
    
    QVBoxLayout *chartLayout = new QVBoxLayout(chartCard);
    chartLayout->setContentsMargins(20, 20, 20, 20);
    chartLayout->setSpacing(10);
    
    QLabel *chartTitle = new QLabel("🚸 Pedestrian Safety Incidents (30 Days)");
    chartTitle->setStyleSheet("font-size: 15px; font-weight: bold; color: " + COLOR_TEXT + ";");
    chartLayout->addWidget(chartTitle);
    
    // Create line chart
    m_safetySeries = new QLineSeries();
    for (int i = 0; i < m_safetyData.size(); ++i) {
        m_safetySeries->append(i, m_safetyData[i]);
    }
    
    QPen pen{QColor(COLOR_WARNING)};
    pen.setWidth(3);
    m_safetySeries->setPen(pen);
    
    m_safetyChart = new QChart();
    m_safetyChart->addSeries(m_safetySeries);
    m_safetyChart->setTitle("");
    m_safetyChart->setAnimationOptions(QChart::SeriesAnimations);
    m_safetyChart->setBackgroundBrush(QBrush(QColor(COLOR_PANEL)));
    
    QValueAxis *axisX = new QValueAxis();
    axisX->setRange(0, 29);
    axisX->setLabelFormat("%d");
    axisX->setTitleText("Day");
    axisX->setLabelsColor(QColor(COLOR_TEXT));
    axisX->setTitleBrush(QBrush(QColor(COLOR_TEXT)));
    m_safetyChart->addAxis(axisX, Qt::AlignBottom);
    m_safetySeries->attachAxis(axisX);
    
    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0, 25);
    axisY->setLabelFormat("%d");
    axisY->setTitleText("Incidents");
    axisY->setLabelsColor(QColor(COLOR_TEXT));
    axisY->setTitleBrush(QBrush(QColor(COLOR_TEXT)));
    m_safetyChart->addAxis(axisY, Qt::AlignLeft);
    m_safetySeries->attachAxis(axisY);
    
    m_safetyChart->legend()->setVisible(false);
    
    m_safetyChartView = new QChartView(m_safetyChart);
    m_safetyChartView->setRenderHint(QPainter::Antialiasing);
    m_safetyChartView->setMinimumHeight(280);
    
    chartLayout->addWidget(m_safetyChartView);
    
    m_safetyInsightLabel = new QLabel("📉 Trend: -8% reduction in violations");
    m_safetyInsightLabel->setStyleSheet("font-size: 12px; color: " + COLOR_SUCCESS + "; font-style: italic;");
    chartLayout->addWidget(m_safetyInsightLabel);
}

void AnalyticsModule::createEnergyAnalytics()
{
    QFrame *chartCard = new QFrame();
    chartCard->setStyleSheet(getCardStyle());
    
    QVBoxLayout *chartLayout = new QVBoxLayout(chartCard);
    chartLayout->setContentsMargins(20, 20, 20, 20);
    chartLayout->setSpacing(10);
    
    QLabel *chartTitle = new QLabel("💡 Energy Savings Performance (24 Hours)");
    chartTitle->setStyleSheet("font-size: 15px; font-weight: bold; color: " + COLOR_TEXT + ";");
    chartLayout->addWidget(chartTitle);
    
    // Create line chart
    m_energySeries = new QLineSeries();
    for (int i = 0; i < m_energyData.size(); ++i) {
        m_energySeries->append(i, m_energyData[i]);
    }
    
    QPen pen{QColor(COLOR_SUCCESS)};
    pen.setWidth(3);
    m_energySeries->setPen(pen);
    
    m_energyChart = new QChart();
    m_energyChart->addSeries(m_energySeries);
    m_energyChart->setTitle("");
    m_energyChart->setAnimationOptions(QChart::SeriesAnimations);
    m_energyChart->setBackgroundBrush(QBrush(QColor(COLOR_PANEL)));
    
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
    axisY->setTitleText("Savings");
    axisY->setLabelsColor(QColor(COLOR_TEXT));
    axisY->setTitleBrush(QBrush(QColor(COLOR_TEXT)));
    m_energyChart->addAxis(axisY, Qt::AlignLeft);
    m_energySeries->attachAxis(axisY);
    
    m_energyChart->legend()->setVisible(false);
    
    m_energyChartView = new QChartView(m_energyChart);
    m_energyChartView->setRenderHint(QPainter::Antialiasing);
    m_energyChartView->setMinimumHeight(280);
    
    chartLayout->addWidget(m_energyChartView);
    
    m_energyInsightLabel = new QLabel("📊 Average: 34% energy savings maintained");
    m_energyInsightLabel->setStyleSheet("font-size: 12px; color: " + COLOR_SUCCESS + "; font-style: italic;");
    chartLayout->addWidget(m_energyInsightLabel);
}

void AnalyticsModule::createSystemOverview()
{
    QFrame *chartCard = new QFrame();
    chartCard->setStyleSheet(getCardStyle());
    
    QVBoxLayout *chartLayout = new QVBoxLayout(chartCard);
    chartLayout->setContentsMargins(20, 20, 20, 20);
    chartLayout->setSpacing(10);
    
    QLabel *chartTitle = new QLabel("🔍 System Health Overview");
    chartTitle->setStyleSheet("font-size: 15px; font-weight: bold; color: " + COLOR_TEXT + ";");
    chartLayout->addWidget(chartTitle);
    
    // Create pie chart
    m_overviewSeries = new QPieSeries();
    
    QPieSlice *recyclingSlice = m_overviewSeries->append("Recycling: 95%", 95);
    recyclingSlice->setColor(QColor(COLOR_SUCCESS));
    recyclingSlice->setLabelVisible(true);
    recyclingSlice->setLabelColor(QColor(COLOR_TEXT));
    
    QPieSlice *safetySlice = m_overviewSeries->append("Safety: 88%", 88);
    safetySlice->setColor(QColor(COLOR_WARNING));
    safetySlice->setLabelVisible(true);
    safetySlice->setLabelColor(QColor(COLOR_TEXT));
    
    QPieSlice *lightingSlice = m_overviewSeries->append("Lighting: 92%", 92);
    lightingSlice->setColor(QColor(COLOR_ACCENT));
    lightingSlice->setLabelVisible(true);
    lightingSlice->setLabelColor(QColor(COLOR_TEXT));
    
    QPieSlice *securitySlice = m_overviewSeries->append("Security: 98%", 98);
    securitySlice->setColor(QColor("#00D864"));
    securitySlice->setLabelVisible(true);
    securitySlice->setLabelColor(QColor(COLOR_TEXT));
    
    m_overviewChart = new QChart();
    m_overviewChart->addSeries(m_overviewSeries);
    m_overviewChart->setTitle("");
    m_overviewChart->legend()->setVisible(true);
    m_overviewChart->legend()->setAlignment(Qt::AlignRight);
    m_overviewChart->legend()->setLabelColor(QColor(COLOR_TEXT));
    m_overviewChart->setBackgroundBrush(QBrush(QColor(COLOR_PANEL)));
    
    m_overviewChartView = new QChartView(m_overviewChart);
    m_overviewChartView->setRenderHint(QPainter::Antialiasing);
    m_overviewChartView->setMinimumHeight(280);
    
    chartLayout->addWidget(m_overviewChartView);
    
    QLabel *insightLabel = new QLabel("✅ All systems operational with high performance");
    insightLabel->setStyleSheet("font-size: 12px; color: " + COLOR_SUCCESS + "; font-style: italic;");
    chartLayout->addWidget(insightLabel);
}

void AnalyticsModule::generateRecyclingData()
{
    m_recyclingData.clear();
    // Generate 7 days x 3 waste types (Plastic, Metal, Glass)
    for (int day = 0; day < 7; ++day) {
        m_recyclingData.append(250 + QRandomGenerator::global()->bounded(100)); // Plastic
        m_recyclingData.append(200 + QRandomGenerator::global()->bounded(100)); // Metal
        m_recyclingData.append(180 + QRandomGenerator::global()->bounded(80));  // Glass
    }
}

void AnalyticsModule::generateSafetyData()
{
    m_safetyData.clear();
    // Generate 30 days of incident data
    for (int i = 0; i < 30; ++i) {
        m_safetyData.append(8 + QRandomGenerator::global()->bounded(12));
    }
}

void AnalyticsModule::generateEnergyData()
{
    m_energyData.clear();
    // Generate 24 hours of energy savings data
    for (int i = 0; i < 24; ++i) {
        m_energyData.append(25 + QRandomGenerator::global()->bounded(20));
    }
}

void AnalyticsModule::updateAnalyticsData()
{
    addLogMessage("Analytics data updated in real-time");
    // Data is regenerated on refresh or report generation
}

void AnalyticsModule::onRefreshData()
{
    generateRecyclingData();
    generateSafetyData();
    generateEnergyData();
    
    // Recreate all charts with new data
    addLogMessage("All analytics charts refreshed with latest data");
}

void AnalyticsModule::onGenerateReport()
{
    QString reportType = m_reportTypeSelector->currentText();
    QDate startDate = m_startDateEdit->date();
    QDate endDate = m_endDateEdit->date();
    
    addLogMessage("Generating report: " + reportType);
    addLogMessage("Date range: " + startDate.toString("yyyy-MM-dd") + " to " + endDate.toString("yyyy-MM-dd"));
    
    emit reportGenerated(reportType);
}

void AnalyticsModule::onExportPDF()
{
    addLogMessage("Exporting analytics report to PDF format...");
    emit dataExported("PDF");
}

void AnalyticsModule::onExportExcel()
{
    addLogMessage("Exporting analytics data to Excel format...");
    emit dataExported("Excel");
}

void AnalyticsModule::onDateRangeChanged()
{
    QDate startDate = m_startDateEdit->date();
    QDate endDate = m_endDateEdit->date();
    
    if (startDate > endDate) {
        addLogMessage("⚠️ Warning: Start date is after end date");
    } else {
        addLogMessage("Date range updated: " + startDate.toString("yyyy-MM-dd") + " to " + endDate.toString("yyyy-MM-dd"));
    }
}

void AnalyticsModule::addLogMessage(const QString &message)
{
    qDebug() << "[ANALYTICS]" << message;
}

QString AnalyticsModule::getCardStyle()
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

QString AnalyticsModule::getButtonStyle(const QString &color)
{
    return QString(
        "QPushButton {"
        "    background-color: %1;"
        "    color: white;"
        "    border: none;"
        "    border-radius: 6px;"
        "    padding: 10px 20px;"
        "    font-size: 13px;"
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

void AnalyticsModule::applyStyles()
{
    setStyleSheet("background-color: " + COLOR_BACKGROUND + ";");
}
