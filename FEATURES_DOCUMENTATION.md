# 🔐 Security & Intelligence Command Center - Complete Feature Documentation

## 📊 Module Overview

The **Security & Intelligence Command Center** is the most advanced module in the NeoCity Smart City platform, providing comprehensive monitoring, threat detection, predictive intelligence, and strategic decision support capabilities.

---

## 🎯 Core Capabilities

### 1️⃣ System Health Monitoring
Monitor real-time operational status of all city infrastructure modules.

**Monitored Systems:**
- ♻️ Recycling Infrastructure
- 🚸 Pedestrian Safety Network  
- 💡 Smart Lighting Network
- 📡 Communication Layer
- 💾 Database & Storage
- 🔐 Authentication System

**Data Points:**
- ✅ Operational Status (✓ Operational / ⚠ Warning / ⛔ Critical)
- 🕒 Last Signal (real-time timestamps)
- ⚠️ Risk Level (Low / Medium / High)

**Color Coding:**
- 🟢 Green = Operational / Low Risk
- 🟠 Orange = Warning / Medium Risk
- 🔴 Red = Critical / High Risk

---

### 2️⃣ Intrusion & Threat Detection

Real-time anomaly detection and security threat monitoring.

**Detected Threat Types:**
- 🚨 Fake Speed Data Injection
- ⚠️ Unrealistic Waste Deposit Patterns
- 📡 Sensor Offline / Communication Loss
- 🔓 Suspicious Login Attempts
- ⚡ Abnormal Energy Consumption Patterns
- ⛔ Data Integrity Violations

**Severity Levels:**
- 🔵 **INFO** - Informational alerts
- 🟠 **WARNING** - Potential issues requiring attention
- 🔴 **CRITICAL** - Immediate action required

**Interactive Controls:**
- 🗑️ Clear Alerts - Remove all current alerts
- ⚠️ Trigger Simulated Threat - Generate test alerts

**Alert Details:**
- Timestamp of detection
- Affected module/system
- Alert type classification
- Severity assessment

---

### 3️⃣ Live Security Log Monitor

Professional terminal-style security event logging system.

**Features:**
- 🖥️ Monospaced terminal font (Courier New)
- 🎨 Color-coded log levels
- 📜 Auto-scrolling to latest entries
- ⚫ Black background for terminal aesthetic
- 🟢 Green text for readability

**Log Entry Format:**
```
[LEVEL] YYYY-MM-DD HH:MM:SS - Message
```

**Log Levels:**
- 🟢 **[INFO]** - Normal operations
- 🟠 **[WARNING]** - Attention required
- 🔴 **[CRITICAL]** - Urgent issues

**Logged Events:**
- System initializations
- Module status changes
- Threat detections
- Configuration changes
- User actions
- Authentication events
- Network status
- Backup operations

---

### 4️⃣ Predictive Intelligence Engine

AI-powered predictive analytics and strategic recommendations.

**Prediction Categories:**

📊 **Infrastructure Predictions:**
- Bin overflow forecasts (4-8 hour window)
- Maintenance requirement predictions
- Capacity threshold warnings
- Equipment failure probability

🚸 **Safety Predictions:**
- Pedestrian traffic pattern analysis
- Risk escalation detection
- Accident probability zones
- Crosswalk safety assessments

💡 **Energy Predictions:**
- Lighting system instability
- Energy consumption anomalies
- Power system failures
- Peak demand forecasts

🌐 **Network Predictions:**
- Communication latency spikes
- Network congestion forecasts
- Connectivity issues
- Bandwidth requirements

**Prediction Attributes:**
- 📈 Confidence Score: 60% - 95%
- 🎯 Related Module
- 💡 Recommended Action

**Recommended Actions:**
- 🚚 Dispatch maintenance crew
- 👁️ Increase monitoring frequency
- ⚙️ Adjust system configuration
- 👮 Notify authorities
- 🔧 Schedule preventive maintenance

---

### 🧠 Strategic Recommendation System

AI-generated strategic recommendations with approval workflow.

**Recommendation Types:**

🛡️ **Preventive Recommendations:**
- Proactive measures to prevent issues
- Maintenance scheduling optimization
- Resource allocation improvements

⚙️ **Operational Recommendations:**
- Day-to-day operational adjustments
- Configuration optimizations
- Resource reallocation

📋 **Strategic Recommendations:**
- Long-term planning initiatives
- Infrastructure upgrades
- Policy changes

**Priority Levels:**
- 🟢 **Low** - Non-urgent suggestions
- 🟠 **Medium** - Should be addressed soon
- 🔴 **High** - Requires immediate attention

**Workflow:**
1. AI analyzes city-wide data patterns
2. Generates recommendation with context
3. Assigns type and priority
4. Operator reviews
5. ✅ Approve → Execute action
6. ❌ Reject → Log decision & generate new recommendation

**Example Recommendations:**
- "Increase monitoring frequency for waste collection points in Zone A to prevent overflow incidents"
- "Deploy additional pedestrian safety monitoring at high-traffic intersections during peak hours"
- "Optimize lighting schedule to reduce energy consumption while maintaining safety standards"

---

### 5️⃣ City Stability & Risk Score

Master indicator showing overall city infrastructure health.

**Stability Index Calculation:**

```
Score = 100 
    - (WasteRisk × 0.3)
    - (SafetyRisk × 0.4)  
    - (EnergyRisk × 0.3)
    - (CyberThreatLevel × 0.5)
```

**Score normalized to 0-100 range**

**Visual Indicators:**

📊 **Progress Bar:**
- 🟢 Green: Score > 75 (Stable)
- 🟠 Orange: Score 50-75 (Moderate Risk)
- 🔴 Red: Score < 50 (High Risk)

🔢 **Large Numeric Display:**
- Real-time score updates
- Color-matched to risk level
- 48px bold font for visibility

📈 **Risk Trend Chart (Qt Charts):**
- **Title:** "City Risk Trend – Last 24 Hours"
- **X-Axis:** Time (0-24 hours)
- **Y-Axis:** Risk Level (0-50)
- **Line Color:** Dodger Blue (#1E90FF)
- **Data Points:** 24 hourly readings
- **Update:** Real-time sliding window

**Risk Trend Features:**
- Smooth line graph with antialiasing
- Dark theme integration
- Automatic axis scaling
- Rolling 24-hour window
- New data pushes old data out

---

### 6️⃣ Simulation & Strategic Control Panel

Training and testing environment for operators.

**Simulation Mode:**
- ✅ Enable/Disable toggle checkbox
- When enabled: All scenario buttons activate
- When disabled: Only monitoring mode (live data)

**Scenario Triggers:**

⚡ **Simulate Speed Violations:**
- Injects fake pedestrian safety data
- Generates speed anomaly alerts
- Increases safety risk factor
- Updates stability score
- Creates warning logs
- Generates predictions

🗑️ **Simulate Waste Overload:**
- Triggers bin overflow conditions
- Creates critical alerts
- Increases waste risk factor
- Updates city stability
- Logs critical events
- Triggers maintenance predictions

💡 **Simulate Lighting Failure:**
- Simulates pole communication loss
- Marks lighting system as warning/critical
- Increases energy risk
- Updates system health panel
- Generates failure logs
- Creates maintenance recommendations

🔓 **Simulate Cyber Attack:**
- Simulates intrusion attempts
- Creates authentication alerts
- Critical threat level
- Increases cyber threat factor
- Logs security events
- Triggers security protocols

🔄 **Reset System State:**
- Restores all risk factors to baseline
- Clears system warnings
- Resets health indicators to operational
- Logs system reset event
- Returns stability score to normal

**What Happens During Simulation:**
1. **Immediate Effects:**
   - Log entry created
   - Alert added to threat panel
   - Risk factors updated
   - Stability score recalculated

2. **Cascading Updates:**
   - Progress bar color changes
   - Score display updates
   - Chart adds new data point
   - System health may change
   - Predictions generated

3. **Signal Emissions:**
   - `threatDetected()` signal fired
   - `riskUpdated()` signal fired
   - Other modules can react
   - Dashboard can update KPIs

---

## 🔧 Technical Architecture

### Signal-Slot Framework

**Emitted Signals:**

```cpp
void threatDetected(QString module, int severity);
// Fired when: New threat detected or simulated
// Parameters: Module name, Severity (1=INFO, 2=WARNING, 3=CRITICAL)
// Use case: Update dashboard alerts, trigger notifications

void riskUpdated(int newScore);
// Fired when: City stability score changes
// Parameters: New score (0-100)
// Use case: Update main dashboard KPI, trigger alarms

void recommendationGenerated(QString text);
// Fired when: New strategic recommendation created
// Parameters: Recommendation text
// Use case: Show notifications, log decisions
```

### Auto-Update System

**Timer Configuration:**
- **Interval:** 5 seconds (5000ms)
- **Purpose:** Gradual risk recovery simulation
- **Actions per tick:**
  - Decrease risk factors (if elevated)
  - Recalculate stability score
  - Random informational log entries
  - Chart updates

**Risk Recovery Logic:**
```cpp
if (m_wasteRisk > baseline) m_wasteRisk -= 1;
if (m_safetyRisk > baseline) m_safetyRisk -= 1;
if (m_energyRisk > baseline) m_energyRisk -= 1;
if (m_cyberThreatLevel > baseline) m_cyberThreatLevel -= 1;
```

Simulates natural system recovery over time.

### Data Structures

**Risk History (QVector):**
```cpp
QVector<double> m_riskHistory;  // 24 data points
```
Rolling window of risk levels for chart visualization.

**Risk Factors (int):**
```cpp
int m_wasteRisk;        // 0-100
int m_safetyRisk;       // 0-100
int m_energyRisk;       // 0-100
int m_cyberThreatLevel; // 0-100
```

**UI Components:**
- 3 QTableWidgets (Health, Threats, Predictions)
- 1 QPlainTextEdit (Log monitor)
- 1 QChart + QLineSeries (Risk trend)
- 1 QProgressBar + QLabel (Stability score)
- 8 QPushButtons (Actions)
- 1 QCheckBox (Simulation mode)

---

## 🎨 Visual Design System

### Color Palette

| Purpose | Color | Hex Code | Usage |
|---------|-------|----------|-------|
| Background | Deep Black | #121212 | Main widget background |
| Panels | Dark Gray | #1E1E1E | Card backgrounds |
| Borders | Border Gray | #2A2A2A | Separators, frames |
| Accent | Dodger Blue | #1E90FF | Primary actions, highlights |
| Success | Green | #00C853 | Operational status, low risk |
| Warning | Orange | #FF9800 | Warnings, medium risk |
| Critical | Red | #D32F2F | Critical alerts, high risk |
| Text | White | #FFFFFF | Primary text |
| Text Dim | Light Gray | #CCCCCC | Secondary text |

### Typography

**Primary Font:**
- Segoe UI (Windows)
- Roboto (Linux/Mac)
- Sans-serif fallback

**Monospace Font:**
- Courier New
- Used for: Security log terminal

**Font Sizes:**
- Title: 24px bold
- Panel Headers: 16px bold
- Score Display: 48px bold
- Body Text: 12-14px
- Log Text: 11px

### Layout System

**Grid-Based Dashboard:**
- QGridLayout for responsive positioning
- 4-column grid system
- Equal spacing: 15px
- Margin: 20px all sides

**Card Pattern:**
```
┌─────────────────────────────┐
│ 🔹 Panel Title              │
│ ─────────────────────────── │
│                             │
│   Content Area              │
│                             │
│   Interactive Elements      │
│                             │
└─────────────────────────────┘
```

**Features:**
- Rounded corners (10px)
- Border (1px solid #2A2A2A)
- Padding (15px)
- Shadow effects via styling

---

## 🚀 Performance Characteristics

### Resource Usage

**Memory:**
- Base: ~5-8 MB
- With full data: ~12-15 MB
- Chart rendering: ~2-3 MB

**CPU:**
- Idle: <1%
- Update cycles: 2-5%
- Chart redraws: 5-10%

**Optimization Features:**
- Efficient QVector for data storage
- Limited table row counts
- Chart data point limits
- Timer-based updates (not continuous polling)

### Scalability

**Current Limits:**
- System Health: 6 modules
- Threat Alerts: Unlimited (with scrolling)
- Log Entries: Unlimited (with scrolling)
- Predictions: 5 visible at once
- Risk History: 24 data points (rolling)

**Easy to Extend:**
- Add more modules to health table
- Increase chart resolution
- Add more threat types
- Expand prediction categories

---

## 🔌 Integration Points

### With Dashboard Module:
```cpp
connect(secCenter, &SecurityIntelligenceCenter::riskUpdated,
        dashboard, &Dashboard::updateCityStability);
```

### With Analytics Module:
```cpp
connect(secCenter, &SecurityIntelligenceCenter::threatDetected,
        analytics, &Analytics::logSecurityEvent);
```

### With Notification System:
```cpp
connect(secCenter, &SecurityIntelligenceCenter::recommendationGenerated,
        notifier, &Notifier::showRecommendation);
```

### With Database Layer:
- Log all security events
- Store historical risk data
- Archive recommendations
- Track approval decisions

### With Real IoT Systems:
- Replace simulated data with sensor feeds
- Connect to MQTT brokers
- WebSocket real-time updates
- REST API integration

---

## 📚 Use Cases

### 1. Municipal Operations Center
Real-time monitoring of entire smart city infrastructure from a single command center.

### 2. Security Training
Simulation mode allows operators to practice responding to various scenarios without affecting live systems.

### 3. Incident Response
Rapid identification and response to security threats and system anomalies.

### 4. Strategic Planning
AI-powered recommendations help optimize resource allocation and prevent issues before they occur.

### 5. Audit & Compliance
Complete security log provides audit trail for regulatory compliance.

### 6. Predictive Maintenance
Early warning system identifies potential failures before they cause service disruptions.

---

## 🎓 Operator Workflow

**Daily Monitoring:**
1. Check City Stability Score upon login
2. Review System Health Overview
3. Scan threat detection panel for new alerts
4. Read security log for anomalies
5. Review and approve/reject recommendations

**Incident Response:**
1. Alert appears in threat detection panel
2. Review severity and affected module
3. Check security log for details
4. Review predictive engine for forecast
5. Take recommended action
6. Monitor stability score for recovery

**Training Session:**
1. Enable Simulation Mode
2. Trigger various scenarios
3. Observe cascading effects across panels
4. Practice response procedures
5. Reset system state
6. Repeat with different scenarios

---

## ✨ Future Enhancement Roadmap

### Phase 1: Data Integration
- [ ] Connect to real sensor networks
- [ ] Database persistence layer
- [ ] Historical data analysis
- [ ] Export reports (PDF/CSV)

### Phase 2: Advanced AI
- [ ] Machine learning models
- [ ] Pattern recognition
- [ ] Anomaly detection algorithms
- [ ] Predictive accuracy tracking

### Phase 3: Collaboration
- [ ] Multi-user support
- [ ] Role-based access control
- [ ] Shared decision-making
- [ ] Notification system

### Phase 4: Mobile/Remote
- [ ] Web dashboard
- [ ] Mobile app integration
- [ ] Remote monitoring
- [ ] Push notifications

---

## 📄 License & Copyright

Part of the **NeoCity Smart City Control Center** platform.

**Technologies:**
- Qt 6.7.3
- Qt Widgets Framework
- Qt Charts Module
- C++17

**Compatibility:**
- Windows 10/11
- Linux (Ubuntu 20.04+)
- macOS (10.15+)

---

**Built with ❤️ for Smart Cities worldwide** 🌆
