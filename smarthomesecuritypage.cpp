#include "smarthomesecuritypage.h"

SmartHomeSecurityPage::SmartHomeSecurityPage(QWidget *parent)
    : QWidget(parent)
    , nextHomeId(5001)
    , emergencyMode(false)
{
    // Initialize chart data BEFORE createUI
    for (int i = 0; i < 24; ++i) {
        temperatureHistory.append(22.0 + (QRandomGenerator::global()->generateDouble() - 0.5) * 6.0);
        humidityHistory.append(50.0 + (QRandomGenerator::global()->generateDouble() - 0.5) * 20.0);
    }
    
    createUI();
    applyDarkTheme();
    loadSampleData();
    
    // Setup timers
    sensorUpdateTimer = new QTimer(this);
    connect(sensorUpdateTimer, &QTimer::timeout, this, &SmartHomeSecurityPage::updateSensorData);
    sensorUpdateTimer->start(4000); // Update every 4 seconds
    
    chartUpdateTimer = new QTimer(this);
    connect(chartUpdateTimer, &QTimer::timeout, this, &SmartHomeSecurityPage::updateEnvironmentalChart);
    chartUpdateTimer->start(5000); // Update every 5 seconds
    
    gasAlertTimer = new QTimer(this);
    connect(gasAlertTimer, &QTimer::timeout, this, &SmartHomeSecurityPage::checkGasAlerts);
    gasAlertTimer->start(3000); // Check every 3 seconds
}

SmartHomeSecurityPage::~SmartHomeSecurityPage()
{
}

void SmartHomeSecurityPage::createUI()
{
    // Create main container widget
    QWidget *containerWidget = new QWidget();
    QVBoxLayout *containerLayout = new QVBoxLayout(containerWidget);
    containerLayout->setSpacing(15);
    containerLayout->setContentsMargins(20, 20, 20, 20);
    
    // Add all sections to container
    containerLayout->addWidget(createHeaderSection());
    containerLayout->addWidget(createKPISection());
    containerLayout->addWidget(createSearchSortSection());
    containerLayout->addWidget(createHomeMonitoringSection());
    
    // Create bottom row
    QHBoxLayout *bottomRow = new QHBoxLayout();
    bottomRow->setSpacing(15);
    
    QVBoxLayout *leftColumn = new QVBoxLayout();
    leftColumn->addWidget(createEnvironmentalChartSection());
    
    QVBoxLayout *rightColumn = new QVBoxLayout();
    rightColumn->addWidget(createEmergencySection());
    rightColumn->addWidget(createCertificateSection());
    
    bottomRow->addLayout(leftColumn, 2);
    bottomRow->addLayout(rightColumn, 1);
    
    containerLayout->addLayout(bottomRow);
    containerLayout->addStretch();
    
    // Create scroll area
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidget(containerWidget);
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setStyleSheet("QScrollArea { border: none; background-color: #121212; }");
    
    // Set scroll area as main layout
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(scrollArea);
}

QFrame* SmartHomeSecurityPage::createHeaderSection()
{
    QFrame *headerFrame = new QFrame();
    headerFrame->setObjectName("headerFrame");
    headerFrame->setMaximumHeight(80);
    
    QHBoxLayout *headerLayout = new QHBoxLayout(headerFrame);
    headerLayout->setContentsMargins(20, 15, 20, 15);
    
    // Title with icon
    QLabel *iconLabel = new QLabel("🏠🛡️");
    iconLabel->setStyleSheet("font-size: 32px;");
    
    QLabel *titleLabel = new QLabel("Smart Housing Security Control");
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: white;");
    
    // Status indicator
    QLabel *statusLabel = new QLabel("● MONITORING ACTIVE");
    statusLabel->setStyleSheet("color: #00C853; font-size: 14px; font-weight: bold;");
    
    headerLayout->addWidget(iconLabel);
    headerLayout->addWidget(titleLabel);
    headerLayout->addStretch();
    headerLayout->addWidget(statusLabel);
    
    return headerFrame;
}

QFrame* SmartHomeSecurityPage::createKPISection()
{
    QFrame *kpiFrame = new QFrame();
    kpiFrame->setObjectName("kpiFrame");
    
    QHBoxLayout *kpiLayout = new QHBoxLayout(kpiFrame);
    kpiLayout->setSpacing(15);
    kpiLayout->setContentsMargins(0, 0, 0, 0);
    
    // KPI Cards
    totalHomesLabel = new QLabel("0");
    activeAlertsLabel = new QLabel("0");
    safeHomesLabel = new QLabel("0");
    warningHomesLabel = new QLabel("0");
    
    kpiLayout->addWidget(createCard("Total Registered Homes", "0", "#1E90FF"));
    kpiLayout->addWidget(createCard("Active Alerts", "0", "#D32F2F"));
    kpiLayout->addWidget(createCard("Safe Homes", "0", "#00C853"));
    kpiLayout->addWidget(createCard("Homes in Warning", "0", "#FF9800"));
    
    return kpiFrame;
}

QFrame* SmartHomeSecurityPage::createCard(const QString &title, const QString &value, const QString &color)
{
    QFrame *card = new QFrame();
    card->setObjectName("kpiCard");
    card->setMinimumHeight(100);
    
    QVBoxLayout *cardLayout = new QVBoxLayout(card);
    cardLayout->setContentsMargins(15, 15, 15, 15);
    
    QLabel *valueLabel = new QLabel(value);
    valueLabel->setStyleSheet(QString("font-size: 28px; font-weight: bold; color: %1;").arg(color));
    
    QLabel *titleLabel = new QLabel(title);
    titleLabel->setStyleSheet("font-size: 13px; color: #AAAAAA;");
    
    cardLayout->addWidget(valueLabel);
    cardLayout->addWidget(titleLabel);
    cardLayout->addStretch();
    
    // Store labels for updating
    if (title == "Total Registered Homes") totalHomesLabel = valueLabel;
    else if (title == "Active Alerts") activeAlertsLabel = valueLabel;
    else if (title == "Safe Homes") safeHomesLabel = valueLabel;
    else if (title == "Homes in Warning") warningHomesLabel = valueLabel;
    
    return card;
}

QFrame* SmartHomeSecurityPage::createSearchSortSection()
{
    QFrame *searchFrame = new QFrame();
    searchFrame->setObjectName("searchFrame");
    searchFrame->setMaximumHeight(70);
    
    QHBoxLayout *searchLayout = new QHBoxLayout(searchFrame);
    searchLayout->setContentsMargins(15, 10, 15, 10);
    
    // Search box
    QLabel *searchLabel = new QLabel("🔍 Search:");
    searchLabel->setStyleSheet("color: white; font-size: 13px;");
    
    searchBox = new QLineEdit();
    searchBox->setPlaceholderText("Search by Owner Name or Home ID...");
    searchBox->setMinimumWidth(250);
    connect(searchBox, &QLineEdit::textChanged, this, &SmartHomeSecurityPage::onSearchTextChanged);
    
    // Sort button
    sortRiskBtn = new QPushButton("⚠️ Sort by Highest Risk");
    connect(sortRiskBtn, &QPushButton::clicked, this, &SmartHomeSecurityPage::onSortByRiskClicked);
    
    // Filter dropdown
    QLabel *filterLabel = new QLabel("Filter:");
    filterLabel->setStyleSheet("color: white; font-size: 13px;");
    
    filterAlertCombo = new QComboBox();
    filterAlertCombo->addItem("All Status");
    filterAlertCombo->addItem("Safe");
    filterAlertCombo->addItem("Warning");
    filterAlertCombo->addItem("Critical");
    connect(filterAlertCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), 
            this, &SmartHomeSecurityPage::onFilterAlertChanged);
    
    searchLayout->addWidget(searchLabel);
    searchLayout->addWidget(searchBox);
    searchLayout->addSpacing(20);
    searchLayout->addWidget(sortRiskBtn);
    searchLayout->addSpacing(20);
    searchLayout->addWidget(filterLabel);
    searchLayout->addWidget(filterAlertCombo);
    searchLayout->addStretch();
    
    return searchFrame;
}

QFrame* SmartHomeSecurityPage::createHomeMonitoringSection()
{
    QFrame *monitoringFrame = new QFrame();
    monitoringFrame->setObjectName("monitoringFrame");
    
    QVBoxLayout *monitoringLayout = new QVBoxLayout(monitoringFrame);
    monitoringLayout->setContentsMargins(15, 15, 15, 15);
    
    // Header with buttons
    QHBoxLayout *headerLayout = new QHBoxLayout();
    
    QLabel *tableTitle = new QLabel("Home Security Monitoring");
    tableTitle->setStyleSheet("font-size: 16px; font-weight: bold; color: white;");
    
    addHomeBtn = new QPushButton("➕ Add Home");
    addHomeBtn->setObjectName("actionButton");
    connect(addHomeBtn, &QPushButton::clicked, this, &SmartHomeSecurityPage::onAddHomeClicked);
    
    editHomeBtn = new QPushButton("✏️ Edit Home");
    editHomeBtn->setObjectName("actionButton");
    connect(editHomeBtn, &QPushButton::clicked, this, &SmartHomeSecurityPage::onEditHomeClicked);
    
    deleteHomeBtn = new QPushButton("🗑️ Delete Home");
    deleteHomeBtn->setObjectName("dangerButton");
    connect(deleteHomeBtn, &QPushButton::clicked, this, &SmartHomeSecurityPage::onDeleteHomeClicked);
    
    headerLayout->addWidget(tableTitle);
    headerLayout->addStretch();
    headerLayout->addWidget(addHomeBtn);
    headerLayout->addWidget(editHomeBtn);
    headerLayout->addWidget(deleteHomeBtn);
    
    // Table
    setupTable();
    
    monitoringLayout->addLayout(headerLayout);
    monitoringLayout->addWidget(homeTable);
    
    return monitoringFrame;
}

void SmartHomeSecurityPage::setupTable()
{
    homeTable = new QTableWidget();
    homeTable->setColumnCount(8);
    homeTable->setHorizontalHeaderLabels({
        "Home ID", "Owner Name", "Gas Level (ppm)", "Smoke (ppm)", 
        "Temp (°C)", "Humidity (%)", "Risk Score", "Alert Status"
    });
    
    // Table styling
    homeTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    homeTable->setSelectionMode(QAbstractItemView::SingleSelection);
    homeTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    homeTable->horizontalHeader()->setStretchLastSection(true);
    homeTable->setAlternatingRowColors(true);
    homeTable->verticalHeader()->setVisible(false);
    homeTable->setMinimumHeight(300);
    homeTable->setShowGrid(true);
    
    // Set column widths
    homeTable->setColumnWidth(0, 100);
    homeTable->setColumnWidth(1, 150);
    homeTable->setColumnWidth(2, 120);
    homeTable->setColumnWidth(3, 110);
    homeTable->setColumnWidth(4, 100);
    homeTable->setColumnWidth(5, 110);
    homeTable->setColumnWidth(6, 100);
}

QFrame* SmartHomeSecurityPage::createEnvironmentalChartSection()
{
    QFrame *chartFrame = new QFrame();
    chartFrame->setObjectName("chartFrame");
    chartFrame->setMinimumHeight(480);
    
    QVBoxLayout *chartLayout = new QVBoxLayout(chartFrame);
    chartLayout->setContentsMargins(15, 15, 15, 15);
    
    QLabel *chartTitle = new QLabel("📈 24-Hour Environmental Trends");
    chartTitle->setStyleSheet("font-size: 15px; font-weight: bold; color: white;");
    
    // Create line chart
    temperatureSeries = new QLineSeries();
    temperatureSeries->setName("Temperature (°C)");
    temperatureSeries->setColor(QColor("#D32F2F"));
    
    humiditySeries = new QLineSeries();
    humiditySeries->setName("Humidity (%)");
    humiditySeries->setColor(QColor("#1E90FF"));
    
    // Populate initial data
    for (int i = 0; i < 24; ++i) {
        temperatureSeries->append(i, temperatureHistory[i]);
        humiditySeries->append(i, humidityHistory[i]);
    }
    
    QChart *chart = new QChart();
    chart->addSeries(temperatureSeries);
    chart->addSeries(humiditySeries);
    chart->setTitle("");
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->setBackgroundBrush(QBrush(QColor("#252525")));
    chart->setBackgroundRoundness(0);
    
    // Axes
    QValueAxis *axisX = new QValueAxis();
    axisX->setTitleText("Hours");
    axisX->setRange(0, 23);
    axisX->setLabelFormat("%d");
    axisX->setLabelsColor(Qt::white);
    axisX->setTitleBrush(QBrush(Qt::white));
    axisX->setGridLineColor(QColor("#3A3A3A"));
    
    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Value");
    axisY->setRange(0, 100);
    axisY->setLabelsColor(Qt::white);
    axisY->setTitleBrush(QBrush(Qt::white));
    axisY->setGridLineColor(QColor("#3A3A3A"));
    
    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    
    temperatureSeries->attachAxis(axisX);
    temperatureSeries->attachAxis(axisY);
    humiditySeries->attachAxis(axisX);
    humiditySeries->attachAxis(axisY);
    
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    chart->legend()->setLabelColor(Qt::white);
    chart->legend()->setFont(QFont("Segoe UI", 10));
    
    environmentalChartView = new QChartView(chart);
    environmentalChartView->setRenderHint(QPainter::Antialiasing);
    environmentalChartView->setMinimumHeight(420);
    environmentalChartView->setStyleSheet("background-color: #252525; border-radius: 5px;");
    
    chartLayout->addWidget(chartTitle);
    chartLayout->addWidget(environmentalChartView, 1);
    
    return chartFrame;
}

QFrame* SmartHomeSecurityPage::createEmergencySection()
{
    QFrame *emergencyFrame = new QFrame();
    emergencyFrame->setObjectName("emergencyFrame");
    
    QVBoxLayout *emergencyLayout = new QVBoxLayout(emergencyFrame);
    emergencyLayout->setContentsMargins(15, 15, 15, 15);
    
    QLabel *emergencyTitle = new QLabel("⚠️ Emergency Controls");
    emergencyTitle->setStyleSheet("font-size: 15px; font-weight: bold; color: white;");
    
    emergencyShutdownBtn = new QPushButton("🚨 EMERGENCY SHUTDOWN");
    emergencyShutdownBtn->setObjectName("emergencyButton");
    emergencyShutdownBtn->setMinimumHeight(80);
    emergencyShutdownBtn->setStyleSheet(
        "QPushButton#emergencyButton {"
        "    background-color: #D32F2F;"
        "    color: white;"
        "    font-size: 18px;"
        "    font-weight: bold;"
        "    border: 3px solid #B71C1C;"
        "    border-radius: 10px;"
        "}"
        "QPushButton#emergencyButton:hover {"
        "    background-color: #B71C1C;"
        "    border: 3px solid #D32F2F;"
        "}"
        "QPushButton#emergencyButton:pressed {"
        "    background-color: #8B0000;"
        "}"
    );
    connect(emergencyShutdownBtn, &QPushButton::clicked, this, &SmartHomeSecurityPage::onEmergencyShutdownClicked);
    
    QLabel *warningLabel = new QLabel("⚠ Activates emergency power/gas cutoff for all homes");
    warningLabel->setStyleSheet("font-size: 11px; color: #FF9800; font-style: italic;");
    warningLabel->setAlignment(Qt::AlignCenter);
    
    emergencyLayout->addWidget(emergencyTitle);
    emergencyLayout->addSpacing(10);
    emergencyLayout->addWidget(emergencyShutdownBtn);
    emergencyLayout->addWidget(warningLabel);
    emergencyLayout->addStretch();
    
    return emergencyFrame;
}

QFrame* SmartHomeSecurityPage::createCertificateSection()
{
    QFrame *certFrame = new QFrame();
    certFrame->setObjectName("certFrame");
    
    QVBoxLayout *certLayout = new QVBoxLayout(certFrame);
    certLayout->setContentsMargins(15, 15, 15, 15);
    
    QLabel *certTitle = new QLabel("📜 Safety Certification");
    certTitle->setStyleSheet("font-size: 15px; font-weight: bold; color: white;");
    
    generateCertBtn = new QPushButton("📄 Generate Safety Certificate");
    generateCertBtn->setObjectName("actionButton");
    generateCertBtn->setMinimumHeight(50);
    connect(generateCertBtn, &QPushButton::clicked, this, &SmartHomeSecurityPage::onGenerateCertificateClicked);
    
    QLabel *certInfo = new QLabel("Generate PDF certificate based on gas/smoke safety history");
    certInfo->setStyleSheet("font-size: 11px; color: #AAAAAA;");
    certInfo->setWordWrap(true);
    
    certLayout->addWidget(certTitle);
    certLayout->addSpacing(10);
    certLayout->addWidget(generateCertBtn);
    certLayout->addWidget(certInfo);
    certLayout->addStretch();
    
    return certFrame;
}

void SmartHomeSecurityPage::loadSampleData()
{
    homes.clear();
    
    // Sample homes
    homes.append({"H-5001", "John Smith", "+1-555-0101", "123 Main St", 120.5, 45.2, 22.3, 48.5, "Safe"});
    homes.append({"H-5002", "Sarah Johnson", "+1-555-0102", "456 Oak Ave", 235.8, 180.3, 24.1, 55.2, "Warning"});
    homes.append({"H-5003", "Michael Brown", "+1-555-0103", "789 Pine Rd", 85.3, 32.1, 21.8, 52.0, "Safe"});
    homes.append({"H-5004", "Emily Davis", "+1-555-0104", "321 Elm St", 520.2, 410.5, 38.5, 72.3, "Critical"});
    homes.append({"H-5005", "David Wilson", "+1-555-0105", "654 Maple Dr", 95.7, 55.8, 23.2, 50.1, "Safe"});
    homes.append({"H-5006", "Lisa Anderson", "+1-555-0106", "987 Cedar Ln", 310.4, 245.6, 26.5, 65.8, "Warning"});
    homes.append({"H-5007", "Robert Taylor", "+1-555-0107", "147 Birch Way", 78.2, 28.9, 22.0, 49.3, "Safe"});
    homes.append({"H-5008", "Jennifer Martinez", "+1-555-0108", "258 Spruce Ct", 155.6, 125.4, 25.3, 58.7, "Warning"});
    
    nextHomeId = 5009;
    
    // Populate table
    homeTable->setRowCount(homes.size());
    for (int i = 0; i < homes.size(); ++i) {
        const Home &home = homes[i];
        
        homeTable->setItem(i, 0, new QTableWidgetItem(home.id));
        homeTable->setItem(i, 1, new QTableWidgetItem(home.ownerName));
        homeTable->setItem(i, 2, new QTableWidgetItem(QString::number(home.gasLevel, 'f', 1)));
        homeTable->setItem(i, 3, new QTableWidgetItem(QString::number(home.smokeLevel, 'f', 1)));
        homeTable->setItem(i, 4, new QTableWidgetItem(QString::number(home.temperature, 'f', 1)));
        homeTable->setItem(i, 5, new QTableWidgetItem(QString::number(home.humidity, 'f', 1)));
        
        int riskScore = calculateRiskScore(home.gasLevel, home.smokeLevel, home.temperature, home.humidity);
        homeTable->setItem(i, 6, new QTableWidgetItem(QString::number(riskScore)));
        
        QTableWidgetItem *statusItem = new QTableWidgetItem(home.alertStatus);
        statusItem->setForeground(QBrush(QColor(getAlertStatusColor(home.alertStatus))));
        homeTable->setItem(i, 7, statusItem);
    }
    
    updateKPICards();
}

void SmartHomeSecurityPage::updateKPICards()
{
    int totalHomes = homes.size();
    int activeAlerts = 0;
    int safeHomes = 0;
    int warningHomes = 0;
    
    for (const Home &home : homes) {
        if (home.alertStatus == "Critical") activeAlerts++;
        else if (home.alertStatus == "Warning") {
            activeAlerts++;
            warningHomes++;
        } else if (home.alertStatus == "Safe") {
            safeHomes++;
        }
    }
    
    totalHomesLabel->setText(QString::number(totalHomes));
    activeAlertsLabel->setText(QString::number(activeAlerts));
    safeHomesLabel->setText(QString::number(safeHomes));
    warningHomesLabel->setText(QString::number(warningHomes));
}

int SmartHomeSecurityPage::calculateRiskScore(double gas, double smoke, double temp, double humidity)
{
    int score = 0;
    
    // Gas risk (0-40 points)
    if (gas >= GAS_CRITICAL_THRESHOLD) score += 40;
    else if (gas >= GAS_CRITICAL_THRESHOLD * 0.6) score += 25;
    else if (gas >= GAS_CRITICAL_THRESHOLD * 0.3) score += 10;
    
    // Smoke risk (0-30 points)
    if (smoke >= SMOKE_WARNING_THRESHOLD) score += 30;
    else if (smoke >= SMOKE_WARNING_THRESHOLD * 0.6) score += 15;
    
    // Temperature risk (0-20 points)
    if (temp >= TEMP_HIGH_THRESHOLD) score += 20;
    else if (temp >= TEMP_HIGH_THRESHOLD * 0.8) score += 10;
    
    // Humidity risk (0-10 points)
    if (humidity >= HUMIDITY_HIGH_THRESHOLD) score += 10;
    
    return qMin(score, 100);
}

void SmartHomeSecurityPage::onAddHomeClicked()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Register New Home");
    dialog.setMinimumWidth(450);
    
    QVBoxLayout *mainLayout = new QVBoxLayout(&dialog);
    QFormLayout *formLayout = new QFormLayout();
    mainLayout->addLayout(formLayout);
    
    QLineEdit *ownerEdit = new QLineEdit();
    QLineEdit *contactEdit = new QLineEdit();
    QLineEdit *addressEdit = new QLineEdit();
    
    QDoubleSpinBox *gasSpin = new QDoubleSpinBox();
    gasSpin->setRange(0, 1000);
    gasSpin->setValue(100.0);
    gasSpin->setSuffix(" ppm");
    
    QDoubleSpinBox *smokeSpin = new QDoubleSpinBox();
    smokeSpin->setRange(0, 1000);
    smokeSpin->setValue(50.0);
    smokeSpin->setSuffix(" ppm");
    
    QDoubleSpinBox *tempSpin = new QDoubleSpinBox();
    tempSpin->setRange(0, 60);
    tempSpin->setValue(22.0);
    tempSpin->setSuffix(" °C");
    
    QDoubleSpinBox *humiditySpin = new QDoubleSpinBox();
    humiditySpin->setRange(0, 100);
    humiditySpin->setValue(50.0);
    humiditySpin->setSuffix(" %");
    
    formLayout->addRow("Owner Name:", ownerEdit);
    formLayout->addRow("Contact:", contactEdit);
    formLayout->addRow("Address:", addressEdit);
    formLayout->addRow("Gas Level:", gasSpin);
    formLayout->addRow("Smoke Level:", smokeSpin);
    formLayout->addRow("Temperature:", tempSpin);
    formLayout->addRow("Humidity:", humiditySpin);
    
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *saveBtn = new QPushButton("Register");
    QPushButton *cancelBtn = new QPushButton("Cancel");
    
    connect(saveBtn, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(cancelBtn, &QPushButton::clicked, &dialog, &QDialog::reject);
    
    buttonLayout->addStretch();
    buttonLayout->addWidget(saveBtn);
    buttonLayout->addWidget(cancelBtn);
    
    mainLayout->addLayout(buttonLayout);
    
    if (dialog.exec() == QDialog::Accepted) {
        Home newHome;
        newHome.id = QString("H-%1").arg(nextHomeId++);
        newHome.ownerName = ownerEdit->text();
        newHome.contact = contactEdit->text();
        newHome.address = addressEdit->text();
        newHome.gasLevel = gasSpin->value();
        newHome.smokeLevel = smokeSpin->value();
        newHome.temperature = tempSpin->value();
        newHome.humidity = humiditySpin->value();
        
        // Determine alert status
        if (newHome.gasLevel >= GAS_CRITICAL_THRESHOLD || newHome.smokeLevel >= SMOKE_WARNING_THRESHOLD) {
            newHome.alertStatus = "Critical";
        } else if (newHome.gasLevel >= GAS_CRITICAL_THRESHOLD * 0.6 || newHome.smokeLevel >= SMOKE_WARNING_THRESHOLD * 0.6) {
            newHome.alertStatus = "Warning";
        } else {
            newHome.alertStatus = "Safe";
        }
        
        homes.append(newHome);
        loadSampleData();
        
        QMessageBox::information(this, "Success", "Home registered successfully!");
    }
}

void SmartHomeSecurityPage::onEditHomeClicked()
{
    int currentRow = homeTable->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "Warning", "Please select a home to edit.");
        return;
    }
    
    if (currentRow >= homes.size()) {
        QMessageBox::warning(this, "Warning", "Invalid selection.");
        return;
    }
    
    Home &home = homes[currentRow];
    
    QDialog dialog(this);
    dialog.setWindowTitle("Edit Home");
    dialog.setMinimumWidth(450);
    
    QVBoxLayout *mainLayout = new QVBoxLayout(&dialog);
    QFormLayout *formLayout = new QFormLayout();
    mainLayout->addLayout(formLayout);
    
    QLineEdit *ownerEdit = new QLineEdit(home.ownerName);
    QLineEdit *contactEdit = new QLineEdit(home.contact);
    QLineEdit *addressEdit = new QLineEdit(home.address);
    
    QDoubleSpinBox *gasSpin = new QDoubleSpinBox();
    gasSpin->setRange(0, 1000);
    gasSpin->setValue(home.gasLevel);
    gasSpin->setSuffix(" ppm");
    
    QDoubleSpinBox *smokeSpin = new QDoubleSpinBox();
    smokeSpin->setRange(0, 1000);
    smokeSpin->setValue(home.smokeLevel);
    smokeSpin->setSuffix(" ppm");
    
    QDoubleSpinBox *tempSpin = new QDoubleSpinBox();
    tempSpin->setRange(0, 60);
    tempSpin->setValue(home.temperature);
    tempSpin->setSuffix(" °C");
    
    QDoubleSpinBox *humiditySpin = new QDoubleSpinBox();
    humiditySpin->setRange(0, 100);
    humiditySpin->setValue(home.humidity);
    humiditySpin->setSuffix(" %");
    
    formLayout->addRow("Owner Name:", ownerEdit);
    formLayout->addRow("Contact:", contactEdit);
    formLayout->addRow("Address:", addressEdit);
    formLayout->addRow("Gas Level:", gasSpin);
    formLayout->addRow("Smoke Level:", smokeSpin);
    formLayout->addRow("Temperature:", tempSpin);
    formLayout->addRow("Humidity:", humiditySpin);
    
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *saveBtn = new QPushButton("Save");
    QPushButton *cancelBtn = new QPushButton("Cancel");
    
    connect(saveBtn, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(cancelBtn, &QPushButton::clicked, &dialog, &QDialog::reject);
    
    buttonLayout->addStretch();
    buttonLayout->addWidget(saveBtn);
    buttonLayout->addWidget(cancelBtn);
    
    mainLayout->addLayout(buttonLayout);
    
    if (dialog.exec() == QDialog::Accepted) {
        home.ownerName = ownerEdit->text();
        home.contact = contactEdit->text();
        home.address = addressEdit->text();
        home.gasLevel = gasSpin->value();
        home.smokeLevel = smokeSpin->value();
        home.temperature = tempSpin->value();
        home.humidity = humiditySpin->value();
        
        // Update alert status
        if (home.gasLevel >= GAS_CRITICAL_THRESHOLD || home.smokeLevel >= SMOKE_WARNING_THRESHOLD) {
            home.alertStatus = "Critical";
        } else if (home.gasLevel >= GAS_CRITICAL_THRESHOLD * 0.6 || home.smokeLevel >= SMOKE_WARNING_THRESHOLD * 0.6) {
            home.alertStatus = "Warning";
        } else {
            home.alertStatus = "Safe";
        }
        
        loadSampleData();
        QMessageBox::information(this, "Success", "Home updated successfully!");
    }
}

void SmartHomeSecurityPage::onDeleteHomeClicked()
{
    int currentRow = homeTable->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "Warning", "Please select a home to delete.");
        return;
    }
    
    if (currentRow >= homes.size()) {
        QMessageBox::warning(this, "Warning", "Invalid selection.");
        return;
    }
    
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Confirm Delete", 
        "Are you sure you want to delete this home registration?",
        QMessageBox::Yes | QMessageBox::No
    );
    
    if (reply == QMessageBox::Yes) {
        // Remove from alert tracking
        alertedHomes.remove(homes[currentRow].id);
        
        homes.removeAt(currentRow);
        loadSampleData();
        QMessageBox::information(this, "Success", "Home deleted successfully!");
    }
}

void SmartHomeSecurityPage::onSearchTextChanged(const QString &text)
{
    for (int i = 0; i < homeTable->rowCount(); ++i) {
        bool match = false;
        
        QString id = homeTable->item(i, 0)->text();
        QString owner = homeTable->item(i, 1)->text();
        
        if (id.contains(text, Qt::CaseInsensitive) || 
            owner.contains(text, Qt::CaseInsensitive)) {
            match = true;
        }
        
        homeTable->setRowHidden(i, !match && !text.isEmpty());
    }
}

void SmartHomeSecurityPage::onSortByRiskClicked()
{
    homeTable->sortItems(6, Qt::DescendingOrder);
}

void SmartHomeSecurityPage::onFilterAlertChanged(int index)
{
    QString filter = filterAlertCombo->itemText(index);
    
    for (int i = 0; i < homeTable->rowCount(); ++i) {
        QString status = homeTable->item(i, 7)->text();
        
        if (filter == "All Status" || status == filter) {
            homeTable->setRowHidden(i, false);
        } else {
            homeTable->setRowHidden(i, true);
        }
    }
}

void SmartHomeSecurityPage::onEmergencyShutdownClicked()
{
    QMessageBox::StandardButton reply = QMessageBox::critical(
        this, 
        "⚠️ EMERGENCY SHUTDOWN CONFIRMATION",
        "This will initiate emergency power and gas cutoff for ALL registered homes.\n\n"
        "This action should only be used in extreme emergencies.\n\n"
        "Do you want to proceed?",
        QMessageBox::Yes | QMessageBox::No
    );
    
    if (reply == QMessageBox::Yes) {
        emergencyMode = true;
        
        // Clear all alerts
        alertedHomes.clear();
        
        // Simulate emergency shutdown
        for (Home &home : homes) {
            home.alertStatus = "Emergency";
            home.gasLevel = 0;
            home.smokeLevel = 0;
        }
        
        loadSampleData();
        
        // Update button appearance
        emergencyShutdownBtn->setText("✅ EMERGENCY MODE ACTIVE");
        emergencyShutdownBtn->setEnabled(false);
        
        QMessageBox::information(this, "Emergency Shutdown", 
            "Emergency shutdown activated!\n\n"
            "All gas and power systems have been isolated.\n"
            "Event logged at: " + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    }
}

void SmartHomeSecurityPage::onGenerateCertificateClicked()
{
    int selectedRow = homeTable->currentRow();
    
    QString homeInfo;
    if (selectedRow >= 0) {
        const Home &home = homes[selectedRow];
        homeInfo = QString("Home: %1\nOwner: %2\nStatus: %3")
            .arg(home.id)
            .arg(home.ownerName)
            .arg(home.alertStatus);
    } else {
        homeInfo = "All Homes Safety Overview";
    }
    
    QMessageBox::information(this, "Safety Certificate Generated", 
        QString("📄 PDF Safety Certificate Generated\n\n%1\n\n"
                "Generated: %2\n"
                "File: safety_certificate_%3.pdf\n\n"
                "Certificate includes:\n"
                "• Gas/Smoke safety history\n"
                "• Environmental monitoring data\n"
                "• Compliance status\n"
                "• Recommendations")
            .arg(homeInfo)
            .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"))
            .arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss")));
}

void SmartHomeSecurityPage::updateSensorData()
{
    if (emergencyMode) return;
    
    // Simulate sensor value changes
    for (Home &home : homes) {
        // Random fluctuations
        home.gasLevel += (QRandomGenerator::global()->generateDouble() - 0.5) * 35.0;
        home.gasLevel = qMax(0.0, qMin(home.gasLevel, 800.0));
        
        home.smokeLevel += (QRandomGenerator::global()->generateDouble() - 0.5) * 25.0;
        home.smokeLevel = qMax(0.0, qMin(home.smokeLevel, 600.0));
        
        home.temperature += (QRandomGenerator::global()->generateDouble() - 0.5) * 2.5;
        home.temperature = qMax(15.0, qMin(home.temperature, 45.0));
        
        home.humidity += (QRandomGenerator::global()->generateDouble() - 0.5) * 4.0;
        home.humidity = qMax(30.0, qMin(home.humidity, 90.0));
        
        // Update alert status
        if (home.gasLevel >= GAS_CRITICAL_THRESHOLD || home.smokeLevel >= SMOKE_WARNING_THRESHOLD) {
            home.alertStatus = "Critical";
        } else if (home.gasLevel >= GAS_CRITICAL_THRESHOLD * 0.6 || home.smokeLevel >= SMOKE_WARNING_THRESHOLD * 0.6) {
            home.alertStatus = "Warning";
        } else {
            home.alertStatus = "Safe";
        }
    }
    
    loadSampleData();
}

void SmartHomeSecurityPage::updateEnvironmentalChart()
{
    // Shift history and add new data
    temperatureHistory.removeFirst();
    humidityHistory.removeFirst();
    
    // Calculate average from all homes
    double avgTemp = 0;
    double avgHumidity = 0;
    
    for (const Home &home : homes) {
        avgTemp += home.temperature;
        avgHumidity += home.humidity;
    }
    
    if (!homes.isEmpty()) {
        avgTemp /= homes.size();
        avgHumidity /= homes.size();
    }
    
    temperatureHistory.append(avgTemp);
    humidityHistory.append(avgHumidity);
    
    // Update series
    temperatureSeries->clear();
    humiditySeries->clear();
    
    for (int i = 0; i < 24; ++i) {
        temperatureSeries->append(i, temperatureHistory[i]);
        humiditySeries->append(i, humidityHistory[i]);
    }
}

void SmartHomeSecurityPage::checkGasAlerts()
{
    if (emergencyMode) return;
    
    for (const Home &home : homes) {
        if (home.gasLevel >= GAS_CRITICAL_THRESHOLD) {
            // Check if we already alerted for this home
            if (alertedHomes.contains(home.id)) {
                continue; // Skip - already alerted
            }
            
            // Show alert
            QMessageBox criticalAlert;
            criticalAlert.setIcon(QMessageBox::Critical);
            criticalAlert.setWindowTitle("⚠️ CRITICAL GAS ALERT");
            criticalAlert.setText(QString(
                "🚨 GAS LEVEL EXCEEDED SAFE THRESHOLD\n\n"
                "Home ID: %1\n"
                "Owner: %2\n"
                "Gas Level: %3 ppm\n"
                "Threshold: %4 ppm\n\n"
                "IMMEDIATE ACTION REQUIRED!"
            ).arg(home.id)
             .arg(home.ownerName)
             .arg(home.gasLevel, 0, 'f', 1)
             .arg(GAS_CRITICAL_THRESHOLD, 0, 'f', 1));
            
            criticalAlert.setStyleSheet(
                "QMessageBox {"
                "    background-color: #1E1E1E;"
                "}"
                "QLabel {"
                "    color: #D32F2F;"
                "    font-weight: bold;"
                "    font-size: 13px;"
                "}"
            );
            
            criticalAlert.exec();
            
            // Mark this home as alerted
            alertedHomes.insert(home.id);
            
            // Only show one alert at a time
            break;
        } else {
            // Gas level is now safe, remove from alerted set
            alertedHomes.remove(home.id);
        }
    }
}

QString SmartHomeSecurityPage::getAlertStatusColor(const QString &status)
{
    if (status == "Safe") return "#00C853";
    if (status == "Warning") return "#FF9800";
    if (status == "Critical") return "#D32F2F";
    if (status == "Emergency") return "#8B0000";
    return "#FFFFFF";
}

void SmartHomeSecurityPage::applyDarkTheme()
{
    setStyleSheet(R"(
        QWidget {
            background-color: #121212;
            color: white;
            font-family: 'Segoe UI', Arial;
        }
        
        QFrame#headerFrame, QFrame#kpiFrame, QFrame#searchFrame, 
        QFrame#monitoringFrame, QFrame#chartFrame, QFrame#emergencyFrame, 
        QFrame#certFrame {
            background-color: #1E1E1E;
            border-radius: 8px;
            border: 1px solid #2A2A2A;
        }
        
        QFrame#kpiCard {
            background-color: #1E1E1E;
            border-radius: 8px;
            border: 1px solid #2A2A2A;
        }
        
        QTableWidget {
            background-color: #1E1E1E;
            alternate-background-color: #252525;
            gridline-color: #2A2A2A;
            color: white;
            border: 1px solid #2A2A2A;
            border-radius: 5px;
        }
        
        QTableWidget::item {
            padding: 8px;
        }
        
        QHeaderView::section {
            background-color: #2A2A2A;
            color: white;
            padding: 8px;
            border: none;
            font-weight: bold;
        }
        
        QLineEdit, QComboBox, QSpinBox, QDoubleSpinBox {
            background-color: #2A2A2A;
            border: 1px solid #3A3A3A;
            border-radius: 5px;
            padding: 8px;
            color: white;
        }
        
        QLineEdit:focus, QComboBox:focus, QSpinBox:focus, QDoubleSpinBox:focus {
            border: 1px solid #1E90FF;
        }
        
        QPushButton {
            background-color: #2A2A2A;
            border: 1px solid #3A3A3A;
            border-radius: 5px;
            padding: 8px 15px;
            color: white;
            font-weight: bold;
        }
        
        QPushButton:hover {
            background-color: #3A3A3A;
            border: 1px solid #1E90FF;
        }
        
        QPushButton#actionButton {
            background-color: #1E90FF;
            border: none;
        }
        
        QPushButton#actionButton:hover {
            background-color: #1C7ED6;
        }
        
        QPushButton#dangerButton {
            background-color: #D32F2F;
            border: none;
        }
        
        QPushButton#dangerButton:hover {
            background-color: #B71C1C;
        }
        
        QComboBox::drop-down {
            border: none;
        }
        
        QComboBox::down-arrow {
            image: none;
            border-left: 5px solid transparent;
            border-right: 5px solid transparent;
            border-top: 5px solid white;
            margin-right: 10px;
        }
        
        QDialog {
            background-color: #1E1E1E;
        }
    )");
}
