# 🔧 Troubleshooting Guide - Security Intelligence Center

## Common Build Issues & Solutions

### ❌ Issue 1: "QtCharts: No such file or directory"

**Problem:** Qt Charts module not found

**Solution:**
```bash
# Verify Qt Charts is installed:
# In Qt Maintenance Tool, ensure "Qt Charts" is selected

# In your .pro file, verify:
QT += core gui charts

# Then rebuild:
qmake
make clean
make
```

### ❌ Issue 2: "undefined reference to SecurityIntelligenceCenter"

**Problem:** Files not added to project

**Solution:**
Check `NeoCity.pro` contains:
```qmake
SOURCES += \
    securityintelligencecenter.cpp

HEADERS += \
    securityintelligencecenter.h
```

Then run:
```bash
qmake
make
```

### ❌ Issue 3: Namespace QtCharts error

**Problem:** Missing namespace declaration

**Solution:**
In `securityintelligencecenter.cpp`, verify you have:
```cpp
using namespace QtCharts;
```
at the top after includes.

### ❌ Issue 4: Navigation button doesn't work

**Problem:** Wrong index in stackedWidget

**Solution:**
Make sure indices match in `createNavigationMenu()` and `stackedWidget->addWidget()` calls.

If Security is at Index 4:
- Navigation button 4 should navigate to index 4
- The lambda should be: `[this, i]() { navigateToPage(i); }`

### ❌ Issue 5: Window appears blank

**Problem:** Layout not properly initialized

**Solution:**
Check that parent widget is being set:
```cpp
securityPage = new SecurityIntelligenceCenter();
// NOT: new SecurityIntelligenceCenter(this);
// The stackedWidget becomes the parent when added
```

---

## Build Commands Reference

### Windows (MinGW):
```powershell
cd c:\Qt\NeoCity
qmake
mingw32-make clean
mingw32-make
.\release\NeoCity.exe
```

### Windows (MSVC):
```powershell
cd c:\Qt\NeoCity
qmake
nmake clean
nmake
.\release\NeoCity.exe
```

### Linux/Mac:
```bash
cd /path/to/NeoCity
qmake
make clean
make
./NeoCity
```

---

## Qt Creator Build

1. **Open Project:** File → Open Project → `NeoCity.pro`
2. **Configure Kit:** Select Qt 6.7.3 MinGW 64-bit
3. **Clean:** Build → Clean All
4. **Run qmake:** Build → Run qmake
5. **Build:** Build → Build Project
6. **Run:** Press Ctrl+R or click green play button

---

## Runtime Warnings

### Warning: "QWidget: Must construct a QApplication before a QWidget"

**Solution:**
Ensure `QApplication app(argc, argv);` is created before any widgets in `main.cpp`.

### Warning: "Unable to set geometry"

**Solution:**
This is usually harmless. The layout system handles geometry automatically.

---

## Debugging Tips

### Enable Debug Output:
```cpp
#include <QDebug>

// In your code:
qDebug() << "Security page created successfully";
```

### Check Object Names:
```cpp
qDebug() << "Security page object:" << securityPage;
qDebug() << "Widget count:" << stackedWidget->count();
qDebug() << "Current index:" << stackedWidget->currentIndex();
```

### Verify Signal Connections:
```cpp
bool connected = connect(secCenter, &SecurityIntelligenceCenter::threatDetected,
                        this, &MainWindow::onThreatDetected);
qDebug() << "Signal connected:" << connected;
```

---

## Performance Issues

### Slow Chart Rendering:

**Solution 1:** Reduce data points
```cpp
// In securityintelligencecenter.cpp, constructor:
for (int i = 0; i < 12; ++i) {  // Changed from 24 to 12
    m_riskHistory.append(15.0 + (QRandomGenerator::global()->bounded(10)));
}
```

**Solution 2:** Disable antialiasing
```cpp
m_chartView->setRenderHint(QPainter::Antialiasing, false);
```

### High CPU Usage:

**Solution:** Increase timer interval
```cpp
// In constructor:
m_updateTimer->start(10000); // Changed from 5000 to 10000 (10 seconds)
```

---

## Qt Version Compatibility

### Qt 6.x (Recommended):
All features work out of the box.

### Qt 5.x:
Minor modifications needed:

```cpp
// Replace:
#include <QRandomGenerator>
int value = QRandomGenerator::global()->bounded(100);

// With:
#include <QTime>
#include <QtGlobal>
qsrand(QTime::currentTime().msec());
int value = qrand() % 100;
```

---

## Module Testing

### Standalone Test Application:

Create `test_security.cpp`:
```cpp
#include <QApplication>
#include "securityintelligencecenter.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    SecurityIntelligenceCenter window;
    window.setWindowTitle("Security Test");
    window.resize(1600, 1000);
    window.show();
    
    return app.exec();
}
```

Compile:
```bash
qmake
make
```

---

## Getting Help

### Qt Documentation:
- **Qt Widgets:** https://doc.qt.io/qt-6/qtwidgets-index.html
- **Qt Charts:** https://doc.qt.io/qt-6/qtcharts-index.html
- **Signals & Slots:** https://doc.qt.io/qt-6/signalsandslots.html

### Check Project Files:
1. Verify `.pro` file is correct
2. Run `qmake -query` to check Qt installation
3. Check compiler output for specific errors
4. Review `Makefile` is generated correctly

### Common Commands:
```bash
# Show Qt configuration
qmake -query

# Force regenerate Makefile
qmake -makefile

# Verbose build output
make VERBOSE=1

# Check what's being compiled
make -n
```

---

## Still Having Issues?

1. **Clean everything:**
   - Delete `Makefile*` files
   - Delete `build/` folder
   - Delete `.qmake.stash`
   - Run `qmake` again

2. **Verify file structure:**
   ```
   NeoCity/
   ├── securityintelligencecenter.h
   ├── securityintelligencecenter.cpp
   ├── mainwindow.h
   ├── mainwindow.cpp
   ├── main.cpp
   └── NeoCity.pro
   ```

3. **Check file encoding:**
   - Files should be UTF-8
   - Line endings should be consistent (LF or CRLF)

4. **Verify Qt Kit:**
   - Must have matching compiler and Qt version
   - Qt 6.7.3 recommended
   - MinGW 64-bit or MSVC 2019 64-bit

---

**If all else fails:** Create a fresh Qt Widgets project and copy files over one by one to isolate the issue.
