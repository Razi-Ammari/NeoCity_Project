#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGridLayout>
#include <QScrollArea>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    
    // Set window properties
    setWindowTitle("NeoCity - Smart City Control Center");
    setMinimumSize(1400, 900);
    
    // Setup the complete UI
    setupUI();
    applyStyles();
    
    // Initialize timer for date/time updates
    updateTimer = new QTimer(this);
    connect(updateTimer, &QTimer::timeout, this, &MainWindow::updateDateTime);
    updateTimer->start(1000); // Update every second
    updateDateTime(); // Initial update
    
    // Initialize system status
    updateSystemStatus();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupUI()
{
    // Create central widget with main layout
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    QVBoxLayout *mainVerticalLayout = new QVBoxLayout(centralWidget);
    mainVerticalLayout->setContentsMargins(0, 0, 0, 0);
    mainVerticalLayout->setSpacing(0);
    
    // Create top bar
    createTopBar();
    mainVerticalLayout->addWidget(topBar);
    
    // Create horizontal layout for navigation and content
    mainLayout = new QHBoxLayout();
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    
    // Create navigation menu
    createNavigationMenu();
    mainLayout->addWidget(navigationMenu);
    
    // Create content area with stacked widget
    contentArea = new QWidget();
    contentLayout = new QVBoxLayout(contentArea);
    contentLayout->setContentsMargins(0, 0, 0, 0);
    
    stackedWidget = new QStackedWidget();
    contentLayout->addWidget(stackedWidget);
    
    // Create all pages
    createDashboard();
    
    // Create placeholders for lazy loading - modules created on first access
    recyclingPage = new QWidget(this);
    safetyPage = new QWidget(this);
    lightingPage = new QWidget(this);
    analyticsPage = new QWidget(this);
    
    // Create placeholder pages for settings and about
    settingsPage = new QWidget();
    QVBoxLayout *settingsLayout = new QVBoxLayout(settingsPage);
    settingsLayout->setContentsMargins(40, 40, 40, 40);
    QLabel *settingsTitle = new QLabel("System Settings");
    settingsTitle->setObjectName("pageTitle");
    settingsLayout->addWidget(settingsTitle);
    QLabel *settingsPlaceholder = new QLabel("System configuration panel will be implemented here.\nConfigure system parameters, user preferences, and module settings.");
    settingsPlaceholder->setObjectName("placeholderText");
    settingsPlaceholder->setAlignment(Qt::AlignCenter);
    settingsLayout->addWidget(settingsPlaceholder, 1, Qt::AlignCenter);
    
    aboutPage = new QWidget();
    QVBoxLayout *aboutLayout = new QVBoxLayout(aboutPage);
    aboutLayout->setContentsMargins(40, 40, 40, 40);
    QLabel *aboutTitle = new QLabel("About NeoCity");
    aboutTitle->setObjectName("pageTitle");
    aboutLayout->addWidget(aboutTitle);
    QLabel *aboutText = new QLabel(
        "NeoCity - Smart City Control Center\n\n"
        "Version 1.0\n\n"
        "A comprehensive municipal management platform integrating:\n"
        "• Smart Recycling Management\n"
        "• Pedestrian Safety Monitoring\n"
        "• Intelligent Lighting Control\n"
        "• Security & Threat Detection\n"
        "• City Intelligence & Predictions\n"
        "• Advanced Analytics & Reporting\n\n"
        "Developed with Qt Widgets Framework"
    );
    aboutText->setObjectName("placeholderText");
    aboutText->setAlignment(Qt::AlignCenter);
    aboutLayout->addWidget(aboutText, 1, Qt::AlignCenter);
    
    // Create placeholders for lazy loading
    QWidget *intelligencePage = new QWidget(this);
    QWidget *secPage = new QWidget(this);
    securityPage = secPage;
    stationPage = new QWidget(this);
    homeSecurityPage = new QWidget(this);
    
    // Add pages to stacked widget
    stackedWidget->addWidget(dashboardPage);      // Index 0
    stackedWidget->addWidget(recyclingPage);      // Index 1
    stackedWidget->addWidget(safetyPage);         // Index 2
    stackedWidget->addWidget(lightingPage);       // Index 3
    stackedWidget->addWidget(securityPage);       // Index 4
    stackedWidget->addWidget(intelligencePage);   // Index 5
    stackedWidget->addWidget(analyticsPage);      // Index 6
    stackedWidget->addWidget(stationPage);        // Index 7
    stackedWidget->addWidget(homeSecurityPage);   // Index 8
    stackedWidget->addWidget(settingsPage);       // Index 9
    stackedWidget->addWidget(aboutPage);          // Index 10
    
    mainLayout->addWidget(contentArea, 1);
    mainVerticalLayout->addLayout(mainLayout, 1);
    
    // Set dashboard as initial page
    stackedWidget->setCurrentIndex(0);
}

void MainWindow::createTopBar()
{
    topBar = new QWidget();
    topBar->setObjectName("topBar");
    topBar->setFixedHeight(90);
    
    QHBoxLayout *topBarLayout = new QHBoxLayout(topBar);
    topBarLayout->setContentsMargins(35, 20, 35, 20);
    topBarLayout->setSpacing(30);
    
    // Left section: Title and subtitle
    QVBoxLayout *titleLayout = new QVBoxLayout();
    titleLayout->setSpacing(5);
    titleLayout->setAlignment(Qt::AlignVCenter);
    
    titleLabel = new QLabel("NeoCity – Smart City Control Center");
    titleLabel->setObjectName("mainTitle");
    titleLayout->addWidget(titleLabel);
    
    subtitleLabel = new QLabel("Small City | Municipal Operations");
    subtitleLabel->setObjectName("subtitle");
    titleLayout->addWidget(subtitleLabel);
    
    topBarLayout->addLayout(titleLayout);
    topBarLayout->addStretch();
    
    // Right section: Date/Time and Status
    QVBoxLayout *rightLayout = new QVBoxLayout();
    rightLayout->setSpacing(8);
    rightLayout->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    
    dateTimeLabel = new QLabel();
    dateTimeLabel->setObjectName("dateTime");
    dateTimeLabel->setAlignment(Qt::AlignRight);
    rightLayout->addWidget(dateTimeLabel);
    
    statusIndicator = new QLabel("🟢 ONLINE");
    statusIndicator->setObjectName("statusIndicator");
    statusIndicator->setAlignment(Qt::AlignRight);
    rightLayout->addWidget(statusIndicator);
    
    topBarLayout->addLayout(rightLayout);
}

void MainWindow::createNavigationMenu()
{
    navigationMenu = new QWidget();
    navigationMenu->setObjectName("navigationMenu");
    navigationMenu->setFixedWidth(280);
    
    navLayout = new QVBoxLayout(navigationMenu);
    navLayout->setContentsMargins(0, 20, 0, 20);
    navLayout->setSpacing(5);
    
    // Navigation items
    QStringList navItems = {
        "🏠  Dashboard",
        "♻️  Smart Recycling",
        "🚸  Pedestrian Safety",
        "💡  Smart Sidewalk Lighting",
        "�  Security & Intelligence",        "🧠  City Intelligence",        "�📊  Analytics & Reports",
        "🚉  Station Command",
        "🏠🛡️  Housing Security",
        "⚙️  System Settings",
        "ℹ️  About NeoCity"
    };
    
    for (int i = 0; i < navItems.size(); ++i) {
        QPushButton *btn = new QPushButton(navItems[i]);
        btn->setObjectName("navButton");
        btn->setCheckable(true);
        btn->setMinimumHeight(55);
        btn->setCursor(Qt::PointingHandCursor);
        
        // Set first button as checked
        if (i == 0) {
            btn->setChecked(true);
            btn->setProperty("active", true);
            btn->style()->unpolish(btn);
            btn->style()->polish(btn);
        }
        
        connect(btn, &QPushButton::clicked, this, [this, i]() {
            navigateToPage(i);
        });
        
        navButtons.append(btn);
        navLayout->addWidget(btn);
    }
    
    navLayout->addStretch();
}

void MainWindow::createDashboard()
{
    dashboardPage = new QWidget();
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    
    QWidget *scrollContent = new QWidget();
    QVBoxLayout *dashLayout = new QVBoxLayout(scrollContent);
    dashLayout->setContentsMargins(40, 40, 40, 40);
    dashLayout->setSpacing(30);
    
    // Page header
    QLabel *pageTitle = new QLabel("Dashboard Overview");
    pageTitle->setObjectName("pageTitle");
    dashLayout->addWidget(pageTitle);
    
    // Create KPI cards
    createKPICards();
    dashLayout->addWidget(kpiCardsContainer);
    
    // Create status cards
    createStatusCards();
    dashLayout->addWidget(statusCardsContainer);
    
    // Create city map
    createCityMap();
    dashLayout->addWidget(cityMapContainer);
    
    dashLayout->addStretch();
    
    scrollArea->setWidget(scrollContent);
    QVBoxLayout *pageLayout = new QVBoxLayout(dashboardPage);
    pageLayout->setContentsMargins(0, 0, 0, 0);
    pageLayout->addWidget(scrollArea);
}

void MainWindow::createKPICards()
{
    kpiCardsContainer = new QWidget();
    QHBoxLayout *kpiLayout = new QHBoxLayout(kpiCardsContainer);
    kpiLayout->setSpacing(25);
    kpiLayout->setContentsMargins(0, 0, 0, 0);
    
    // KPI Card 1: Total Recycled Today
    QFrame *card1 = createKPICard("♻️", "Total Recycled Today", "847 kg", "Plastic • Metal • Glass");
    kpiLayout->addWidget(card1);
    
    // KPI Card 2: Pedestrian Safety Alerts
    QFrame *card2 = createKPICard("🚸", "Pedestrian Safety Alerts", "12 alerts", "Today");
    kpiLayout->addWidget(card2);
    
    // KPI Card 3: Energy Saved
    QFrame *card3 = createKPICard("💡", "Energy Saved", "34 %", "Compared to standard lighting");
    kpiLayout->addWidget(card3);
}

QFrame* MainWindow::createKPICard(const QString &icon, const QString &title, const QString &value, const QString &subtext)
{
    QFrame *card = new QFrame();
    card->setObjectName("kpiCard");
    card->setMinimumHeight(180);
    
    QVBoxLayout *cardLayout = new QVBoxLayout(card);
    cardLayout->setContentsMargins(25, 25, 25, 25);
    cardLayout->setSpacing(10);
    
    // Icon
    QLabel *iconLabel = new QLabel(icon);
    iconLabel->setObjectName("kpiIcon");
    cardLayout->addWidget(iconLabel);
    
    // Title
    QLabel *titleLabel = new QLabel(title);
    titleLabel->setObjectName("kpiTitle");
    titleLabel->setWordWrap(true);
    cardLayout->addWidget(titleLabel);
    
    // Value
    QLabel *valueLabel = new QLabel(value);
    valueLabel->setObjectName("kpiValue");
    cardLayout->addWidget(valueLabel);
    
    // Subtext
    QLabel *subtextLabel = new QLabel(subtext);
    subtextLabel->setObjectName("kpiSubtext");
    subtextLabel->setWordWrap(true);
    cardLayout->addWidget(subtextLabel);
    
    cardLayout->addStretch();
    
    return card;
}

void MainWindow::createStatusCards()
{
    statusCardsContainer = new QWidget();
    QHBoxLayout *statusLayout = new QHBoxLayout(statusCardsContainer);
    statusLayout->setSpacing(25);
    statusLayout->setContentsMargins(0, 0, 0, 0);
    
    // Status Card 1: Recycling Status
    QFrame *card1 = createStatusCard("Recycling Status", "OPERATIONAL", "success");
    statusLayout->addWidget(card1);
    
    // Status Card 2: Pedestrian Risk Level
    QFrame *card2 = createStatusCard("Pedestrian Risk Level", "LOW", "success");
    statusLayout->addWidget(card2);
    
    // Status Card 3: Lighting Mode
    QFrame *card3 = createStatusCard("Lighting Mode", "ECO", "primary");
    statusLayout->addWidget(card3);
}

QFrame* MainWindow::createStatusCard(const QString &title, const QString &status, const QString &colorClass)
{
    QFrame *card = new QFrame();
    card->setObjectName("statusCard");
    card->setMinimumHeight(120);
    
    QVBoxLayout *cardLayout = new QVBoxLayout(card);
    cardLayout->setContentsMargins(25, 20, 25, 20);
    cardLayout->setSpacing(15);
    
    // Title
    QLabel *titleLabel = new QLabel(title);
    titleLabel->setObjectName("statusTitle");
    cardLayout->addWidget(titleLabel);
    
    // Status
    QLabel *statusLabel = new QLabel(status);
    statusLabel->setObjectName("statusValue_" + colorClass);
    cardLayout->addWidget(statusLabel);
    
    cardLayout->addStretch();
    
    return card;
}

void MainWindow::createCityMap()
{
    cityMapContainer = new QWidget();
    cityMapContainer->setMinimumHeight(400);
    
    QVBoxLayout *mapLayout = new QVBoxLayout(cityMapContainer);
    mapLayout->setContentsMargins(0, 0, 0, 0);
    
    QFrame *mapFrame = new QFrame();
    mapFrame->setObjectName("cityMapFrame");
    
    QVBoxLayout *frameLayout = new QVBoxLayout(mapFrame);
    frameLayout->setContentsMargins(30, 30, 30, 30);
    
    // Map title
    QLabel *mapTitle = new QLabel("City Zone Overview");
    mapTitle->setObjectName("mapTitle");
    frameLayout->addWidget(mapTitle);
    
    // Zone grid (3x3)
    QGridLayout *zoneGrid = new QGridLayout();
    zoneGrid->setSpacing(15);
    
    QString zones[3][3] = {
        {"North District", "Central Plaza", "East District"},
        {"West Industrial", "City Center", "Market Area"},
        {"South Residential", "Park Zone", "Tech Campus"}
    };
    
    QString zoneStatus[3][3] = {
        {"normal", "normal", "warning"},
        {"normal", "normal", "normal"},
        {"normal", "alert", "normal"}
    };
    
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            QFrame *zoneCard = new QFrame();
            zoneCard->setObjectName("zoneCard_" + zoneStatus[row][col]);
            zoneCard->setMinimumHeight(80);
            
            QVBoxLayout *zoneLayout = new QVBoxLayout(zoneCard);
            zoneLayout->setContentsMargins(15, 15, 15, 15);
            
            QLabel *zoneLabel = new QLabel(zones[row][col]);
            zoneLabel->setObjectName("zoneLabel");
            zoneLabel->setAlignment(Qt::AlignCenter);
            zoneLabel->setWordWrap(true);
            zoneLayout->addWidget(zoneLabel);
            
            // Status indicator
            QString indicator;
            if (zoneStatus[row][col] == "normal") indicator = "🟢";
            else if (zoneStatus[row][col] == "warning") indicator = "🟠";
            else indicator = "🔴";
            
            QLabel *statusLabel = new QLabel(indicator);
            statusLabel->setAlignment(Qt::AlignCenter);
            statusLabel->setObjectName("zoneStatus");
            zoneLayout->addWidget(statusLabel);
            
            zoneGrid->addWidget(zoneCard, row, col);
        }
    }
    
    frameLayout->addLayout(zoneGrid);
    frameLayout->addStretch();
    
    mapLayout->addWidget(mapFrame);
}

void MainWindow::applyStyles()
{
    QString stylesheet = R"(
        /* Global styles */
        QMainWindow {
            background-color: #121212;
        }
        
        /* Top Bar */
        #topBar {
            background-color: #1E1E1E;
            border-bottom: 2px solid #2A2A2A;
        }
        
        #mainTitle {
            font-size: 28px;
            font-weight: bold;
            color: #FFFFFF;
            font-family: 'Segoe UI', 'Roboto', sans-serif;
            letter-spacing: 0.5px;
        }
        
        #subtitle {
            font-size: 14px;
            color: #B0B0B0;
            font-family: 'Segoe UI', 'Roboto', sans-serif;
            letter-spacing: 0.3px;
        }
        
        #dateTime {
            font-size: 15px;
            color: #E0E0E0;
            font-weight: 600;
            font-family: 'Segoe UI', 'Roboto', sans-serif;
        }
        
        #statusIndicator {
            font-size: 14px;
            font-weight: bold;
            color: #00C853;
            font-family: 'Segoe UI', 'Roboto', sans-serif;
            padding: 3px 8px;
            letter-spacing: 0.5px;
        }
        
        /* Navigation Menu */
        #navigationMenu {
            background-color: #1A1A1A;
            border-right: 2px solid #2A2A2A;
        }
        
        #navButton {
            background-color: transparent;
            color: #B0B0B0;
            border: none;
            text-align: left;
            padding-left: 25px;
            font-size: 15px;
            font-family: 'Segoe UI', 'Roboto', sans-serif;
            font-weight: 500;
        }
        
        #navButton:hover {
            background-color: #252525;
            color: #FFFFFF;
        }
        
        #navButton:checked {
            background-color: #1E90FF;
            color: #FFFFFF;
            border-left: 4px solid #1565C0;
        }
        
        /* Content Area */
        QScrollArea {
            background-color: #121212;
            border: none;
        }
        
        #pageTitle {
            font-size: 32px;
            font-weight: bold;
            color: #FFFFFF;
            margin-bottom: 10px;
            font-family: 'Segoe UI', 'Roboto', sans-serif;
        }
        
        /* KPI Cards */
        #kpiCard {
            background-color: #1E1E1E;
            border-radius: 12px;
            border: 1px solid #2A2A2A;
        }
        
        #kpiIcon {
            font-size: 42px;
        }
        
        #kpiTitle {
            font-size: 14px;
            color: #B0B0B0;
            font-family: 'Segoe UI', 'Roboto', sans-serif;
        }
        
        #kpiValue {
            font-size: 38px;
            font-weight: bold;
            color: #1E90FF;
            font-family: 'Segoe UI', 'Roboto', sans-serif;
        }
        
        #kpiSubtext {
            font-size: 12px;
            color: #808080;
            font-family: 'Segoe UI', 'Roboto', sans-serif;
        }
        
        /* Status Cards */
        #statusCard {
            background-color: #1E1E1E;
            border-radius: 12px;
            border: 1px solid #2A2A2A;
        }
        
        #statusTitle {
            font-size: 14px;
            color: #B0B0B0;
            font-family: 'Segoe UI', 'Roboto', sans-serif;
        }
        
        #statusValue_success {
            font-size: 24px;
            font-weight: bold;
            color: #00C853;
            font-family: 'Segoe UI', 'Roboto', sans-serif;
        }
        
        #statusValue_warning {
            font-size: 24px;
            font-weight: bold;
            color: #FF9800;
            font-family: 'Segoe UI', 'Roboto', sans-serif;
        }
        
        #statusValue_danger {
            font-size: 24px;
            font-weight: bold;
            color: #D32F2F;
            font-family: 'Segoe UI', 'Roboto', sans-serif;
        }
        
        #statusValue_primary {
            font-size: 24px;
            font-weight: bold;
            color: #1E90FF;
            font-family: 'Segoe UI', 'Roboto', sans-serif;
        }
        
        /* City Map */
        #cityMapFrame {
            background-color: #1E1E1E;
            border-radius: 12px;
            border: 1px solid #2A2A2A;
        }
        
        #mapTitle {
            font-size: 20px;
            font-weight: bold;
            color: #FFFFFF;
            margin-bottom: 20px;
            font-family: 'Segoe UI', 'Roboto', sans-serif;
        }
        
        QFrame[objectName^="zoneCard_"] {
            background-color: #252525;
            border-radius: 8px;
            border: 2px solid #2A2A2A;
        }
        
        #zoneCard_normal {
            border-left: 4px solid #00C853;
        }
        
        #zoneCard_warning {
            border-left: 4px solid #FF9800;
        }
        
        #zoneCard_alert {
            border-left: 4px solid #D32F2F;
        }
        
        #zoneLabel {
            font-size: 13px;
            color: #E0E0E0;
            font-weight: 600;
            font-family: 'Segoe UI', 'Roboto', sans-serif;
        }
        
        #zoneStatus {
            font-size: 24px;
        }
        
        /* Placeholder pages */
        #placeholderText {
            font-size: 16px;
            color: #808080;
            font-family: 'Segoe UI', 'Roboto', sans-serif;
            line-height: 1.8;
        }
    )";
    
    setStyleSheet(stylesheet);
}

void MainWindow::updateDateTime()
{
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString dateTimeStr = currentDateTime.toString("dddd, MMMM dd, yyyy | hh:mm:ss AP");
    dateTimeLabel->setText(dateTimeStr);
}

void MainWindow::navigateToPage(int index)
{
    // Lazy-create the page if it hasn't been created yet
    lazyCreatePage(index);
    
    // Update navigation buttons
    for (int i = 0; i < navButtons.size(); ++i) {
        navButtons[i]->setChecked(i == index);
    }
    
    // Switch page
    stackedWidget->setCurrentIndex(index);
}

void MainWindow::lazyCreatePage(int index)
{
    QWidget *currentPage = stackedWidget->widget(index);
    if (!currentPage) return;
    
    // Check if page is just a placeholder (has no children or is empty QWidget)
    bool isPlaceholder = (currentPage->children().size() <= 1);  // Only has QObject parent
    
    if (!isPlaceholder) return;  // Already created
    
    QWidget *newPage = nullptr;
    
    switch(index) {
        case 1:  // Smart Recycling
            newPage = new SmartRecyclingModule(this);
            recyclingPage = newPage;
            break;
        case 2:  // Pedestrian Safety
            newPage = new PedestrianSafetyModule(this);
            safetyPage = newPage;
            break;
        case 3:  // Smart Lighting
            newPage = new SmartLightingModule(this);
            lightingPage = newPage;
            break;
        case 4:  // Security Intelligence
            newPage = new SecurityIntelligenceCenter(this);
            securityPage = newPage;
            break;
        case 5:  // City Intelligence
            newPage = new CityIntelligenceModule(this);
            break;
        case 6:  // Analytics
            newPage = new AnalyticsModule(this);
            analyticsPage = newPage;
            break;
        case 7:  // Station Command
            newPage = new SmartStationPage(this);
            stationPage = newPage;
            break;
        case 8:  // Housing Security
            newPage = new SmartHomeSecurityPage(this);
            homeSecurityPage = newPage;
            break;
        default:
            return;  // Dashboard, Settings, About don't need lazy loading
    }
    
    if (newPage) {
        // Replace placeholder with real module
        stackedWidget->removeWidget(currentPage);
        currentPage->deleteLater();
        stackedWidget->insertWidget(index, newPage);
    }
}

void MainWindow::updateSystemStatus()
{
    // In a real application, this would check actual system status
    // For now, we'll set it to ONLINE
    statusIndicator->setText("🟢 ONLINE");
    statusIndicator->setStyleSheet("color: #00C853;");
}
