// ============================================
// MAINWINDOW.H - ADDITIONS NEEDED
// ============================================
// Add this line at the top with other includes:

#include "securityintelligencecenter.h"

// In the private section, find where pages are declared:
// (around line 65)

    QWidget *safetyPage;
    QWidget *lightingPage;
    QWidget *securityPage;        // ← ADD THIS LINE
    QWidget *analyticsPage;
    QWidget *settingsPage;
    QWidget *aboutPage;


// ============================================
// MAINWINDOW.CPP - MODIFICATIONS NEEDED
// ============================================

// ──────────────────────────────────────────
// MODIFICATION 1: In setupUI() method
// Find the section where pages are added to stackedWidget
// (around line 68-77)
// ──────────────────────────────────────────

void MainWindow::setupUI()
{
    // ... existing code ...
    
    // Create all pages
    createDashboard();
    createPlaceholderPages();
    
    // ✨ ADD THIS LINE BEFORE stackedWidget->addWidget calls:
    securityPage = new SecurityIntelligenceCenter();
    
    // Add pages to stacked widget
    stackedWidget->addWidget(dashboardPage);      // Index 0
    stackedWidget->addWidget(recyclingPage);      // Index 1
    stackedWidget->addWidget(safetyPage);         // Index 2
    stackedWidget->addWidget(lightingPage);       // Index 3
    stackedWidget->addWidget(securityPage);       // Index 4  ← ADD THIS
    stackedWidget->addWidget(analyticsPage);      // Index 5  ← WAS 4
    stackedWidget->addWidget(settingsPage);       // Index 6  ← WAS 5
    stackedWidget->addWidget(aboutPage);          // Index 7  ← WAS 6
    
    mainLayout->addWidget(contentArea, 1);
    mainVerticalLayout->addLayout(mainLayout, 1);
    
    // ✨ ADD SIGNAL CONNECTIONS (Optional but recommended):
    SecurityIntelligenceCenter *secCenter = qobject_cast<SecurityIntelligenceCenter*>(securityPage);
    if (secCenter) {
        connect(secCenter, &SecurityIntelligenceCenter::threatDetected, 
                this, [](QString module, int severity) {
            qDebug() << "⚠️ Threat detected in" << module << "- Severity:" << severity;
        });
        
        connect(secCenter, &SecurityIntelligenceCenter::riskUpdated, 
                this, [](int newScore) {
            qDebug() << "📊 City stability updated:" << newScore;
        });
        
        connect(secCenter, &SecurityIntelligenceCenter::recommendationGenerated, 
                this, [](QString text) {
            qDebug() << "🧠 New recommendation:" << text;
        });
    }
    
    // Set dashboard as initial page
    stackedWidget->setCurrentIndex(0);
}


// ──────────────────────────────────────────
// MODIFICATION 2: In createNavigationMenu() method
// Find the QStringList navItems section
// (around line 139)
// ──────────────────────────────────────────

void MainWindow::createNavigationMenu()
{
    navigationMenu = new QWidget();
    navigationMenu->setObjectName("navigationMenu");
    navigationMenu->setFixedWidth(280);
    
    navLayout = new QVBoxLayout(navigationMenu);
    navLayout->setContentsMargins(0, 20, 0, 20);
    navLayout->setSpacing(5);
    
    // ✨ REPLACE the existing navItems list with this:
    QStringList navItems = {
        "🏠  Dashboard",
        "♻️  Smart Recycling",
        "🚸  Pedestrian Safety",
        "💡  Smart Sidewalk Lighting",
        "🔐  Security & Intelligence",    // ← ADD THIS LINE
        "📊  Analytics & Reports",
        "⚙️  System Settings",
        "ℹ️  About NeoCity"
    };
    
    // ... rest of the method stays the same ...
}


// ============================================
// COMPLETE EXAMPLE - Alternative Minimal Integration
// If you want to test it standalone first:
// ============================================

/*
#include "securityintelligencecenter.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    SecurityIntelligenceCenter window;
    window.setWindowTitle("NeoCity Security & Intelligence Command Center");
    window.resize(1600, 1000);
    window.show();
    
    return app.exec();
}
*/
