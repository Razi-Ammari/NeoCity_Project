/****************************************************************************
** Meta object code from reading C++ file 'securityintelligencecenter.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../securityintelligencecenter.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'securityintelligencecenter.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSSecurityIntelligenceCenterENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSSecurityIntelligenceCenterENDCLASS = QtMocHelpers::stringData(
    "SecurityIntelligenceCenter",
    "threatDetected",
    "",
    "module",
    "severity",
    "riskUpdated",
    "newScore",
    "recommendationGenerated",
    "text",
    "onSimulateSpeedViolation",
    "onSimulateWasteOverload",
    "onSimulateLightingFailure",
    "onSimulateCyberAttack",
    "onResetSystemState",
    "onClearAlerts",
    "onTriggerSimulatedThreat",
    "onApproveRecommendation",
    "onRejectRecommendation",
    "onSimulationModeToggled",
    "enabled",
    "updateRiskMetrics",
    "generatePrediction"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSSecurityIntelligenceCenterENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    2,  104,    2, 0x06,    1 /* Public */,
       5,    1,  109,    2, 0x06,    4 /* Public */,
       7,    1,  112,    2, 0x06,    6 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       9,    0,  115,    2, 0x08,    8 /* Private */,
      10,    0,  116,    2, 0x08,    9 /* Private */,
      11,    0,  117,    2, 0x08,   10 /* Private */,
      12,    0,  118,    2, 0x08,   11 /* Private */,
      13,    0,  119,    2, 0x08,   12 /* Private */,
      14,    0,  120,    2, 0x08,   13 /* Private */,
      15,    0,  121,    2, 0x08,   14 /* Private */,
      16,    0,  122,    2, 0x08,   15 /* Private */,
      17,    0,  123,    2, 0x08,   16 /* Private */,
      18,    1,  124,    2, 0x08,   17 /* Private */,
      20,    0,  127,    2, 0x08,   19 /* Private */,
      21,    0,  128,    2, 0x08,   20 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    3,    4,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::QString,    8,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   19,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject SecurityIntelligenceCenter::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_CLASSSecurityIntelligenceCenterENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSSecurityIntelligenceCenterENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSSecurityIntelligenceCenterENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<SecurityIntelligenceCenter, std::true_type>,
        // method 'threatDetected'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'riskUpdated'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'recommendationGenerated'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'onSimulateSpeedViolation'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onSimulateWasteOverload'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onSimulateLightingFailure'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onSimulateCyberAttack'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onResetSystemState'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onClearAlerts'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onTriggerSimulatedThreat'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onApproveRecommendation'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onRejectRecommendation'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onSimulationModeToggled'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'updateRiskMetrics'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'generatePrediction'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void SecurityIntelligenceCenter::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SecurityIntelligenceCenter *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->threatDetected((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 1: _t->riskUpdated((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 2: _t->recommendationGenerated((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 3: _t->onSimulateSpeedViolation(); break;
        case 4: _t->onSimulateWasteOverload(); break;
        case 5: _t->onSimulateLightingFailure(); break;
        case 6: _t->onSimulateCyberAttack(); break;
        case 7: _t->onResetSystemState(); break;
        case 8: _t->onClearAlerts(); break;
        case 9: _t->onTriggerSimulatedThreat(); break;
        case 10: _t->onApproveRecommendation(); break;
        case 11: _t->onRejectRecommendation(); break;
        case 12: _t->onSimulationModeToggled((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 13: _t->updateRiskMetrics(); break;
        case 14: _t->generatePrediction(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (SecurityIntelligenceCenter::*)(QString , int );
            if (_t _q_method = &SecurityIntelligenceCenter::threatDetected; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (SecurityIntelligenceCenter::*)(int );
            if (_t _q_method = &SecurityIntelligenceCenter::riskUpdated; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (SecurityIntelligenceCenter::*)(QString );
            if (_t _q_method = &SecurityIntelligenceCenter::recommendationGenerated; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject *SecurityIntelligenceCenter::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SecurityIntelligenceCenter::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSSecurityIntelligenceCenterENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int SecurityIntelligenceCenter::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 15)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 15;
    }
    return _id;
}

// SIGNAL 0
void SecurityIntelligenceCenter::threatDetected(QString _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SecurityIntelligenceCenter::riskUpdated(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void SecurityIntelligenceCenter::recommendationGenerated(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
