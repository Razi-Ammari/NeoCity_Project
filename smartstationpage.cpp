#include "smartstationpage.h"

SmartStationPage::SmartStationPage(QWidget *parent)
    : QWidget(parent)
    , nextStationId(1001)
    , busArrivalSeconds(180)
{
    createUI();
    applyDarkTheme();
    loadSampleData();
    
    // Setup timers
    rfidUpdateTimer = new QTimer(this);
    connect(rfidUpdateTimer, &QTimer::timeout, this, &SmartStationPage::updateRFIDLog);
    rfidUpdateTimer->start(3000); // Update every 3 seconds
    
    busArrivalTimer = new QTimer(this);
    connect(busArrivalTimer, &QTimer::timeout, this, &SmartStationPage::updateBusArrival);
    busArrivalTimer->start(1000); // Update every second
    
    statsUpdateTimer = new QTimer(this);
    connect(statsUpdateTimer, &QTimer::timeout, this, &SmartStationPage::updateStatistics);
    statsUpdateTimer->start(5000); // Update every 5 seconds
}

SmartStationPage::~SmartStationPage()
{
}

void SmartStationPage::createUI()
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
    containerLayout->addWidget(createStationManagementSection());
    
    // Create bottom row with charts and features
    QHBoxLayout *bottomRow = new QHBoxLayout();
    bottomRow->setSpacing(15);
    
    QVBoxLayout *leftColumn = new QVBoxLayout();
    leftColumn->addWidget(createOccupancyChartSection());
    leftColumn->addWidget(createExportSection());
    
    QVBoxLayout *rightColumn = new QVBoxLayout();
    rightColumn->addWidget(createRFIDLogSection());
    rightColumn->addWidget(createBusArrivalSection());
    
    bottomRow->addLayout(leftColumn, 1);
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

QFrame* SmartStationPage::createHeaderSection()
{
    QFrame *headerFrame = new QFrame();
    headerFrame->setObjectName("headerFrame");
    headerFrame->setMaximumHeight(80);
    
    QHBoxLayout *headerLayout = new QHBoxLayout(headerFrame);
    headerLayout->setContentsMargins(20, 15, 20, 15);
    
    // Title with icon
    QLabel *iconLabel = new QLabel("🚉");
    iconLabel->setStyleSheet("font-size: 32px;");
    
    QLabel *titleLabel = new QLabel("Smart Station Command Center");
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: white;");
    
    // Status indicator
    QLabel *statusLabel = new QLabel("● SYSTEM ONLINE");
    statusLabel->setStyleSheet("color: #00C853; font-size: 14px; font-weight: bold;");
    
    headerLayout->addWidget(iconLabel);
    headerLayout->addWidget(titleLabel);
    headerLayout->addStretch();
    headerLayout->addWidget(statusLabel);
    
    return headerFrame;
}

QFrame* SmartStationPage::createKPISection()
{
    QFrame *kpiFrame = new QFrame();
    kpiFrame->setObjectName("kpiFrame");
    
    QHBoxLayout *kpiLayout = new QHBoxLayout(kpiFrame);
    kpiLayout->setSpacing(15);
    kpiLayout->setContentsMargins(0, 0, 0, 0);
    
    // KPI Cards
    totalStationsLabel = new QLabel("0");
    stationsFullLabel = new QLabel("0");
    totalPassengersLabel = new QLabel("0");
    avgOccupancyLabel = new QLabel("0%");
    
    kpiLayout->addWidget(createCard("Total Stations", "0", "#1E90FF"));
    kpiLayout->addWidget(createCard("Stations Full", "0", "#D32F2F"));
    kpiLayout->addWidget(createCard("Total Passengers Today", "0", "#00C853"));
    kpiLayout->addWidget(createCard("Avg Occupancy", "0%", "#FF9800"));
    
    return kpiFrame;
}

QFrame* SmartStationPage::createCard(const QString &title, const QString &value, const QString &color)
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
    if (title == "Total Stations") totalStationsLabel = valueLabel;
    else if (title == "Stations Full") stationsFullLabel = valueLabel;
    else if (title == "Total Passengers Today") totalPassengersLabel = valueLabel;
    else if (title == "Avg Occupancy") avgOccupancyLabel = valueLabel;
    
    return card;
}

QFrame* SmartStationPage::createSearchSortSection()
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
    searchBox->setPlaceholderText("Search by Station ID or Location...");
    searchBox->setMinimumWidth(250);
    connect(searchBox, &QLineEdit::textChanged, this, &SmartStationPage::onSearchTextChanged);
    
    // Sort button
    sortVolumeBtn = new QPushButton("📊 Sort by Volume");
    connect(sortVolumeBtn, &QPushButton::clicked, this, &SmartStationPage::onSortByVolumeClicked);
    
    // Filter dropdown
    QLabel *filterLabel = new QLabel("Filter:");
    filterLabel->setStyleSheet("color: white; font-size: 13px;");
    
    filterStatusCombo = new QComboBox();
    filterStatusCombo->addItem("All Status");
    filterStatusCombo->addItem("Operational");
    filterStatusCombo->addItem("Full");
    filterStatusCombo->addItem("Maintenance");
    connect(filterStatusCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), 
            this, &SmartStationPage::onFilterStatusChanged);
    
    searchLayout->addWidget(searchLabel);
    searchLayout->addWidget(searchBox);
    searchLayout->addSpacing(20);
    searchLayout->addWidget(sortVolumeBtn);
    searchLayout->addSpacing(20);
    searchLayout->addWidget(filterLabel);
    searchLayout->addWidget(filterStatusCombo);
    searchLayout->addStretch();
    
    return searchFrame;
}

QFrame* SmartStationPage::createStationManagementSection()
{
    QFrame *managementFrame = new QFrame();
    managementFrame->setObjectName("managementFrame");
    
    QVBoxLayout *managementLayout = new QVBoxLayout(managementFrame);
    managementLayout->setContentsMargins(15, 15, 15, 15);
    
    // Header with buttons
    QHBoxLayout *headerLayout = new QHBoxLayout();
    
    QLabel *tableTitle = new QLabel("Station Management");
    tableTitle->setStyleSheet("font-size: 16px; font-weight: bold; color: white;");
    
    addStationBtn = new QPushButton("➕ Add Station");
    addStationBtn->setObjectName("actionButton");
    connect(addStationBtn, &QPushButton::clicked, this, &SmartStationPage::onAddStationClicked);
    
    editStationBtn = new QPushButton("✏️ Edit Station");
    editStationBtn->setObjectName("actionButton");
    connect(editStationBtn, &QPushButton::clicked, this, &SmartStationPage::onEditStationClicked);
    
    deleteStationBtn = new QPushButton("🗑️ Delete Station");
    deleteStationBtn->setObjectName("dangerButton");
    connect(deleteStationBtn, &QPushButton::clicked, this, &SmartStationPage::onDeleteStationClicked);
    
    headerLayout->addWidget(tableTitle);
    headerLayout->addStretch();
    headerLayout->addWidget(addStationBtn);
    headerLayout->addWidget(editStationBtn);
    headerLayout->addWidget(deleteStationBtn);
    
    // Table
    setupTable();
    
    managementLayout->addLayout(headerLayout);
    managementLayout->addWidget(stationTable);
    
    return managementFrame;
}

void SmartStationPage::setupTable()
{
    stationTable = new QTableWidget();
    stationTable->setColumnCount(6);
    stationTable->setHorizontalHeaderLabels({
        "Station ID", "Location", "Capacity", "Current Passengers", "Occupancy %", "Status"
    });
    
    // Table styling
    stationTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    stationTable->setSelectionMode(QAbstractItemView::SingleSelection);
    stationTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    stationTable->horizontalHeader()->setStretchLastSection(true);
    stationTable->setAlternatingRowColors(true);
    stationTable->verticalHeader()->setVisible(false);
    stationTable->setMinimumHeight(300);
    stationTable->setShowGrid(true);
    
    // Set column widths
    stationTable->setColumnWidth(0, 100);
    stationTable->setColumnWidth(1, 200);
    stationTable->setColumnWidth(2, 100);
    stationTable->setColumnWidth(3, 150);
    stationTable->setColumnWidth(4, 120);
}

QFrame* SmartStationPage::createOccupancyChartSection()
{
    QFrame *chartFrame = new QFrame();
    chartFrame->setObjectName("chartFrame");
    chartFrame->setMinimumHeight(320);
    
    QVBoxLayout *chartLayout = new QVBoxLayout(chartFrame);
    chartLayout->setContentsMargins(15, 15, 15, 15);
    
    QLabel *chartTitle = new QLabel("📊 Station Occupancy Distribution");
    chartTitle->setStyleSheet("font-size: 15px; font-weight: bold; color: white;");
    
    // Create pie chart
    occupancySeries = new QPieSeries();
    
    // Add initial data
    QPieSlice *fullSlice = occupancySeries->append("Full (≥90%)", 0);
    fullSlice->setBrush(QColor("#D32F2F"));
    fullSlice->setLabelVisible(true);
    fullSlice->setLabelColor(Qt::white);
    
    QPieSlice *almostSlice = occupancySeries->append("Almost Full (70-89%)", 0);
    almostSlice->setBrush(QColor("#FF9800"));
    almostSlice->setLabelVisible(true);
    almostSlice->setLabelColor(Qt::white);
    
    QPieSlice *availableSlice = occupancySeries->append("Available (<70%)", 0);
    availableSlice->setBrush(QColor("#00C853"));
    availableSlice->setLabelVisible(true);
    availableSlice->setLabelColor(Qt::white);
    
    QChart *chart = new QChart();
    chart->addSeries(occupancySeries);
    chart->setTitle("");
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->setBackgroundBrush(QBrush(QColor("#252525")));
    chart->setTitleBrush(QBrush(Qt::white));
    chart->setBackgroundRoundness(0);
    
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    chart->legend()->setLabelColor(Qt::white);
    chart->legend()->setFont(QFont("Segoe UI", 10));
    
    occupancyChartView = new QChartView(chart);
    occupancyChartView->setRenderHint(QPainter::Antialiasing);
    occupancyChartView->setMinimumHeight(240);
    occupancyChartView->setStyleSheet("background-color: #252525; border-radius: 5px;");
    
    chartLayout->addWidget(chartTitle);
    chartLayout->addWidget(occupancyChartView, 1);
    
    return chartFrame;
}

QFrame* SmartStationPage::createRFIDLogSection()
{
    QFrame *rfidFrame = new QFrame();
    rfidFrame->setObjectName("rfidFrame");
    rfidFrame->setMinimumHeight(260);
    
    QVBoxLayout *rfidLayout = new QVBoxLayout(rfidFrame);
    rfidLayout->setContentsMargins(15, 15, 15, 15);
    
    QLabel *rfidTitle = new QLabel("📡 Real-Time RFID Tap Log");
    rfidTitle->setStyleSheet("font-size: 15px; font-weight: bold; color: white;");
    
    rfidLogTable = new QTableWidget();
    rfidLogTable->setColumnCount(3);
    rfidLogTable->setHorizontalHeaderLabels({"Timestamp", "Passenger ID", "Station ID"});
    rfidLogTable->setMinimumHeight(180);
    rfidLogTable->setSelectionMode(QAbstractItemView::NoSelection);
    rfidLogTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    rfidLogTable->setAlternatingRowColors(true);
    rfidLogTable->verticalHeader()->setVisible(false);
    rfidLogTable->horizontalHeader()->setStretchLastSection(true);
    rfidLogTable->setColumnWidth(0, 140);
    rfidLogTable->setColumnWidth(1, 130);
    rfidLogTable->setShowGrid(true);
    
    rfidLayout->addWidget(rfidTitle);
    rfidLayout->addWidget(rfidLogTable, 1);
    
    return rfidFrame;
}

QFrame* SmartStationPage::createBusArrivalSection()
{
    QFrame *busFrame = new QFrame();
    busFrame->setObjectName("busFrame");
    busFrame->setMaximumHeight(150);
    
    QVBoxLayout *busLayout = new QVBoxLayout(busFrame);
    busLayout->setContentsMargins(15, 15, 15, 15);
    
    QLabel *busTitle = new QLabel("🚌 Next Bus Arrival Prediction");
    busTitle->setStyleSheet("font-size: 15px; font-weight: bold; color: white;");
    
    busArrivalLabel = new QLabel("03:00");
    busArrivalLabel->setAlignment(Qt::AlignCenter);
    busArrivalLabel->setStyleSheet("font-size: 36px; font-weight: bold; color: #1E90FF; padding: 20px;");
    
    QLabel *subLabel = new QLabel("Minutes:Seconds");
    subLabel->setAlignment(Qt::AlignCenter);
    subLabel->setStyleSheet("font-size: 12px; color: #AAAAAA;");
    
    busLayout->addWidget(busTitle);
    busLayout->addWidget(busArrivalLabel);
    busLayout->addWidget(subLabel);
    
    return busFrame;
}

QFrame* SmartStationPage::createExportSection()
{
    QFrame *exportFrame = new QFrame();
    exportFrame->setObjectName("exportFrame");
    exportFrame->setMaximumHeight(100);
    
    QHBoxLayout *exportLayout = new QHBoxLayout(exportFrame);
    exportLayout->setContentsMargins(15, 15, 15, 15);
    
    QLabel *exportTitle = new QLabel("📤 Export Reports:");
    exportTitle->setStyleSheet("font-size: 14px; font-weight: bold; color: white;");
    
    QPushButton *excelBtn = new QPushButton("📊 Export to Excel");
    excelBtn->setObjectName("actionButton");
    connect(excelBtn, &QPushButton::clicked, this, &SmartStationPage::onExportExcelClicked);
    
    QPushButton *pdfBtn = new QPushButton("📄 Export to PDF");
    pdfBtn->setObjectName("actionButton");
    connect(pdfBtn, &QPushButton::clicked, this, &SmartStationPage::onExportPDFClicked);
    
    exportLayout->addWidget(exportTitle);
    exportLayout->addStretch();
    exportLayout->addWidget(excelBtn);
    exportLayout->addWidget(pdfBtn);
    
    return exportFrame;
}

void SmartStationPage::loadSampleData()
{
    stations.clear();
    
    // Sample stations
    stations.append({"ST-1001", "Downtown Central", 500, 456, "Full"});
    stations.append({"ST-1002", "North District", 400, 234, "Operational"});
    stations.append({"ST-1003", "East Terminal", 600, 289, "Operational"});
    stations.append({"ST-1004", "West Plaza", 450, 445, "Full"});
    stations.append({"ST-1005", "South Gate", 350, 120, "Operational"});
    stations.append({"ST-1006", "Airport Link", 550, 0, "Maintenance"});
    stations.append({"ST-1007", "University Hub", 480, 370, "Operational"});
    stations.append({"ST-1008", "Business Park", 420, 315, "Operational"});
    
    nextStationId = 1009;
    
    // Populate table
    stationTable->setRowCount(stations.size());
    for (int i = 0; i < stations.size(); ++i) {
        const Station &station = stations[i];
        
        stationTable->setItem(i, 0, new QTableWidgetItem(station.id));
        stationTable->setItem(i, 1, new QTableWidgetItem(station.location));
        stationTable->setItem(i, 2, new QTableWidgetItem(QString::number(station.capacity)));
        stationTable->setItem(i, 3, new QTableWidgetItem(QString::number(station.currentPassengers)));
        
        int occupancy = (station.currentPassengers * 100) / station.capacity;
        stationTable->setItem(i, 4, new QTableWidgetItem(QString::number(occupancy) + "%"));
        
        QTableWidgetItem *statusItem = new QTableWidgetItem(station.status);
        statusItem->setForeground(QBrush(QColor(getStatusColor(station.status))));
        stationTable->setItem(i, 5, statusItem);
    }
    
    updateKPICards();
    updateOccupancyChart();
}

void SmartStationPage::updateKPICards()
{
    int totalStations = stations.size();
    int fullStations = 0;
    int totalPassengers = 0;
    int totalOccupancy = 0;
    
    for (const Station &station : stations) {
        if (station.status == "Full") fullStations++;
        totalPassengers += station.currentPassengers;
        if (station.capacity > 0) {
            totalOccupancy += (station.currentPassengers * 100) / station.capacity;
        }
    }
    
    int avgOccupancy = totalStations > 0 ? totalOccupancy / totalStations : 0;
    
    totalStationsLabel->setText(QString::number(totalStations));
    stationsFullLabel->setText(QString::number(fullStations));
    totalPassengersLabel->setText(QString::number(totalPassengers));
    avgOccupancyLabel->setText(QString::number(avgOccupancy) + "%");
}

void SmartStationPage::updateOccupancyChart()
{
    int fullCount = 0;
    int almostFullCount = 0;
    int availableCount = 0;
    
    for (const Station &station : stations) {
        int occupancy = (station.currentPassengers * 100) / station.capacity;
        
        if (occupancy >= 90) fullCount++;
        else if (occupancy >= 70) almostFullCount++;
        else availableCount++;
    }
    
    // Update existing slices instead of clearing
    if (occupancySeries->slices().size() >= 3) {
        occupancySeries->slices()[0]->setValue(fullCount);
        occupancySeries->slices()[1]->setValue(almostFullCount);
        occupancySeries->slices()[2]->setValue(availableCount);
    } else {
        // Fallback if slices don't exist
        occupancySeries->clear();
        
        QPieSlice *fullSlice = occupancySeries->append("Full (≥90%)", fullCount);
        fullSlice->setBrush(QColor("#D32F2F"));
        fullSlice->setLabelVisible(true);
        fullSlice->setLabelColor(Qt::white);
        
        QPieSlice *almostSlice = occupancySeries->append("Almost Full (70-89%)", almostFullCount);
        almostSlice->setBrush(QColor("#FF9800"));
        almostSlice->setLabelVisible(true);
        almostSlice->setLabelColor(Qt::white);
        
        QPieSlice *availableSlice = occupancySeries->append("Available (<70%)", availableCount);
        availableSlice->setBrush(QColor("#00C853"));
        availableSlice->setLabelVisible(true);
        availableSlice->setLabelColor(Qt::white);
    }
}

void SmartStationPage::onAddStationClicked()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Add New Station");
    dialog.setMinimumWidth(400);
    
    QVBoxLayout *mainLayout = new QVBoxLayout(&dialog);
    QFormLayout *formLayout = new QFormLayout();
    mainLayout->addLayout(formLayout);
    
    QLineEdit *locationEdit = new QLineEdit();
    QSpinBox *capacitySpin = new QSpinBox();
    capacitySpin->setRange(100, 1000);
    capacitySpin->setValue(400);
    
    QSpinBox *passengersSpin = new QSpinBox();
    passengersSpin->setRange(0, 1000);
    passengersSpin->setValue(0);
    
    QComboBox *statusCombo = new QComboBox();
    statusCombo->addItems({"Operational", "Full", "Maintenance"});
    
    formLayout->addRow("Location:", locationEdit);
    formLayout->addRow("Capacity:", capacitySpin);
    formLayout->addRow("Current Passengers:", passengersSpin);
    formLayout->addRow("Status:", statusCombo);
    
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
        Station newStation;
        newStation.id = QString("ST-%1").arg(nextStationId++);
        newStation.location = locationEdit->text();
        newStation.capacity = capacitySpin->value();
        newStation.currentPassengers = passengersSpin->value();
        newStation.status = statusCombo->currentText();
        
        stations.append(newStation);
        loadSampleData();
        
        QMessageBox::information(this, "Success", "Station added successfully!");
    }
}

void SmartStationPage::onEditStationClicked()
{
    int currentRow = stationTable->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "Warning", "Please select a station to edit.");
        return;
    }
    
    if (currentRow >= stations.size()) {
        QMessageBox::warning(this, "Warning", "Invalid selection.");
        return;
    }
    
    Station &station = stations[currentRow];
    
    QDialog dialog(this);
    dialog.setWindowTitle("Edit Station");
    dialog.setMinimumWidth(400);
    
    QVBoxLayout *mainLayout = new QVBoxLayout(&dialog);
    QFormLayout *formLayout = new QFormLayout();
    mainLayout->addLayout(formLayout);
    
    QLineEdit *locationEdit = new QLineEdit(station.location);
    QSpinBox *capacitySpin = new QSpinBox();
    capacitySpin->setRange(100, 1000);
    capacitySpin->setValue(station.capacity);
    
    QSpinBox *passengersSpin = new QSpinBox();
    passengersSpin->setRange(0, 1000);
    passengersSpin->setValue(station.currentPassengers);
    
    QComboBox *statusCombo = new QComboBox();
    statusCombo->addItems({"Operational", "Full", "Maintenance"});
    statusCombo->setCurrentText(station.status);
    
    formLayout->addRow("Location:", locationEdit);
    formLayout->addRow("Capacity:", capacitySpin);
    formLayout->addRow("Current Passengers:", passengersSpin);
    formLayout->addRow("Status:", statusCombo);
    
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
        station.location = locationEdit->text();
        station.capacity = capacitySpin->value();
        station.currentPassengers = passengersSpin->value();
        station.status = statusCombo->currentText();
        
        loadSampleData();
        QMessageBox::information(this, "Success", "Station updated successfully!");
    }
}

void SmartStationPage::onDeleteStationClicked()
{
    int currentRow = stationTable->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "Warning", "Please select a station to delete.");
        return;
    }
    
    if (currentRow >= stations.size()) {
        QMessageBox::warning(this, "Warning", "Invalid selection.");
        return;
    }
    
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Confirm Delete", 
        "Are you sure you want to delete this station?",
        QMessageBox::Yes | QMessageBox::No
    );
    
    if (reply == QMessageBox::Yes) {
        stations.removeAt(currentRow);
        loadSampleData();
        QMessageBox::information(this, "Success", "Station deleted successfully!");
    }
}

void SmartStationPage::onSearchTextChanged(const QString &text)
{
    for (int i = 0; i < stationTable->rowCount(); ++i) {
        bool match = false;
        
        QString id = stationTable->item(i, 0)->text();
        QString location = stationTable->item(i, 1)->text();
        
        if (id.contains(text, Qt::CaseInsensitive) || 
            location.contains(text, Qt::CaseInsensitive)) {
            match = true;
        }
        
        stationTable->setRowHidden(i, !match && !text.isEmpty());
    }
}

void SmartStationPage::onSortByVolumeClicked()
{
    stationTable->sortItems(3, Qt::DescendingOrder);
}

void SmartStationPage::onFilterStatusChanged(int index)
{
    QString filter = filterStatusCombo->itemText(index);
    
    for (int i = 0; i < stationTable->rowCount(); ++i) {
        QString status = stationTable->item(i, 5)->text();
        
        if (filter == "All Status" || status == filter) {
            stationTable->setRowHidden(i, false);
        } else {
            stationTable->setRowHidden(i, true);
        }
    }
}

void SmartStationPage::onExportExcelClicked()
{
    QMessageBox::information(this, "Export", 
        "Exporting passenger traffic data to Excel...\n\nFile: station_report.xlsx");
}

void SmartStationPage::onExportPDFClicked()
{
    QMessageBox::information(this, "Export", 
        "Generating PDF report...\n\nFile: station_report.pdf");
}

void SmartStationPage::updateRFIDLog()
{
    // Simulate new RFID tap
    QDateTime now = QDateTime::currentDateTime();
    QString timestamp = now.toString("hh:mm:ss");
    QString passengerId = QString("PASS-%1").arg(QRandomGenerator::global()->bounded(10000, 99999));
    QString stationId = stations.isEmpty() ? "ST-1001" : 
                        stations[QRandomGenerator::global()->bounded(stations.size())].id;
    
    // Add to top of table
    rfidLogTable->insertRow(0);
    rfidLogTable->setItem(0, 0, new QTableWidgetItem(timestamp));
    rfidLogTable->setItem(0, 1, new QTableWidgetItem(passengerId));
    rfidLogTable->setItem(0, 2, new QTableWidgetItem(stationId));
    
    // Keep only last 10 entries
    while (rfidLogTable->rowCount() > 10) {
        rfidLogTable->removeRow(rfidLogTable->rowCount() - 1);
    }
}

void SmartStationPage::updateBusArrival()
{
    busArrivalSeconds--;
    
    if (busArrivalSeconds <= 0) {
        busArrivalSeconds = QRandomGenerator::global()->bounded(120, 300); // 2-5 minutes
    }
    
    int minutes = busArrivalSeconds / 60;
    int seconds = busArrivalSeconds % 60;
    
    QString timeText = QString("%1:%2")
        .arg(minutes, 2, 10, QChar('0'))
        .arg(seconds, 2, 10, QChar('0'));
    
    busArrivalLabel->setText(timeText);
    
    // Change color based on time
    if (busArrivalSeconds < 30) {
        busArrivalLabel->setStyleSheet("font-size: 36px; font-weight: bold; color: #00C853; padding: 20px;");
    } else if (busArrivalSeconds < 60) {
        busArrivalLabel->setStyleSheet("font-size: 36px; font-weight: bold; color: #FF9800; padding: 20px;");
    } else {
        busArrivalLabel->setStyleSheet("font-size: 36px; font-weight: bold; color: #1E90FF; padding: 20px;");
    }
}

void SmartStationPage::updateStatistics()
{
    // Simulate passenger count changes
    for (Station &station : stations) {
        if (station.status != "Maintenance") {
            int change = QRandomGenerator::global()->bounded(-10, 15);
            station.currentPassengers = qBound(0, station.currentPassengers + change, station.capacity);
            
            // Update status based on occupancy
            int occupancy = (station.currentPassengers * 100) / station.capacity;
            if (occupancy >= 95) {
                station.status = "Full";
            } else if (station.status == "Full" && occupancy < 90) {
                station.status = "Operational";
            }
        }
    }
    
    loadSampleData();
}

QString SmartStationPage::getStatusColor(const QString &status)
{
    if (status == "Operational") return "#00C853";
    if (status == "Full") return "#D32F2F";
    if (status == "Maintenance") return "#FF9800";
    return "#FFFFFF";
}

void SmartStationPage::applyDarkTheme()
{
    setStyleSheet(R"(
        QWidget {
            background-color: #121212;
            color: white;
            font-family: 'Segoe UI', Arial;
        }
        
        QFrame#headerFrame, QFrame#kpiFrame, QFrame#searchFrame, 
        QFrame#managementFrame, QFrame#chartFrame, QFrame#rfidFrame, 
        QFrame#busFrame, QFrame#exportFrame {
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
        
        QLineEdit, QComboBox, QSpinBox {
            background-color: #2A2A2A;
            border: 1px solid #3A3A3A;
            border-radius: 5px;
            padding: 8px;
            color: white;
        }
        
        QLineEdit:focus, QComboBox:focus, QSpinBox:focus {
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
    )");
}
