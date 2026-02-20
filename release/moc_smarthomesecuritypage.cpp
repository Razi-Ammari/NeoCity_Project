/****************************************************************************
** Meta object code from reading C++ file 'smarthomesecuritypage.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../smarthomesecuritypage.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'smarthomesecuritypage.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_CLASSSmartHomeSecurityPageENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSSmartHomeSecurityPageENDCLASS = QtMocHelpers::stringData(
    "SmartHomeSecurityPage",
    "onAddHomeClicked",
    "",
    "onEditHomeClicked",
    "onDeleteHomeClicked",
    "onSearchTextChanged",
    "text",
    "onSortByRiskClicked",
    "onFilterAlertChanged",
    "index",
    "onEmergencyShutdownClicked",
    "onGenerateCertificateClicked",
    "updateSensorData",
    "updateEnvironmentalChart",
    "checkGasAlerts"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSSmartHomeSecurityPageENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   80,    2, 0x08,    1 /* Private */,
       3,    0,   81,    2, 0x08,    2 /* Private */,
       4,    0,   82,    2, 0x08,    3 /* Private */,
       5,    1,   83,    2, 0x08,    4 /* Private */,
       7,    0,   86,    2, 0x08,    6 /* Private */,
       8,    1,   87,    2, 0x08,    7 /* Private */,
      10,    0,   90,    2, 0x08,    9 /* Private */,
      11,    0,   91,    2, 0x08,   10 /* Private */,
      12,    0,   92,    2, 0x08,   11 /* Private */,
      13,    0,   93,    2, 0x08,   12 /* Private */,
      14,    0,   94,    2, 0x08,   13 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject SmartHomeSecurityPage::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_CLASSSmartHomeSecurityPageENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSSmartHomeSecurityPageENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSSmartHomeSecurityPageENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<SmartHomeSecurityPage, std::true_type>,
        // method 'onAddHomeClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onEditHomeClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onDeleteHomeClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onSearchTextChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'onSortByRiskClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onFilterAlertChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onEmergencyShutdownClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onGenerateCertificateClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'updateSensorData'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'updateEnvironmentalChart'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'checkGasAlerts'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void SmartHomeSecurityPage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SmartHomeSecurityPage *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->onAddHomeClicked(); break;
        case 1: _t->onEditHomeClicked(); break;
        case 2: _t->onDeleteHomeClicked(); break;
        case 3: _t->onSearchTextChanged((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 4: _t->onSortByRiskClicked(); break;
        case 5: _t->onFilterAlertChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 6: _t->onEmergencyShutdownClicked(); break;
        case 7: _t->onGenerateCertificateClicked(); break;
        case 8: _t->updateSensorData(); break;
        case 9: _t->updateEnvironmentalChart(); break;
        case 10: _t->checkGasAlerts(); break;
        default: ;
        }
    }
}

const QMetaObject *SmartHomeSecurityPage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SmartHomeSecurityPage::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSSmartHomeSecurityPageENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int SmartHomeSecurityPage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 11;
    }
    return _id;
}
QT_WARNING_POP
