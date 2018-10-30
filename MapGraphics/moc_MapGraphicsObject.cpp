/****************************************************************************
** Meta object code from reading C++ file 'MapGraphicsObject.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "MapGraphicsObject.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MapGraphicsObject.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MapGraphicsObject_t {
    QByteArrayData data[18];
    char stringdata0[258];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MapGraphicsObject_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MapGraphicsObject_t qt_meta_stringdata_MapGraphicsObject = {
    {
QT_MOC_LITERAL(0, 0, 17), // "MapGraphicsObject"
QT_MOC_LITERAL(1, 18, 14), // "enabledChanged"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 14), // "opacityChanged"
QT_MOC_LITERAL(4, 49, 13), // "parentChanged"
QT_MOC_LITERAL(5, 63, 10), // "posChanged"
QT_MOC_LITERAL(6, 74, 15), // "rotationChanged"
QT_MOC_LITERAL(7, 90, 14), // "visibleChanged"
QT_MOC_LITERAL(8, 105, 13), // "zValueChanged"
QT_MOC_LITERAL(9, 119, 14), // "toolTipChanged"
QT_MOC_LITERAL(10, 134, 7), // "toolTip"
QT_MOC_LITERAL(11, 142, 12), // "flagsChanged"
QT_MOC_LITERAL(12, 155, 15), // "selectedChanged"
QT_MOC_LITERAL(13, 171, 18), // "newObjectGenerated"
QT_MOC_LITERAL(14, 190, 18), // "MapGraphicsObject*"
QT_MOC_LITERAL(15, 209, 15), // "redrawRequested"
QT_MOC_LITERAL(16, 225, 17), // "keyFocusRequested"
QT_MOC_LITERAL(17, 243, 14) // "setConstructed"

    },
    "MapGraphicsObject\0enabledChanged\0\0"
    "opacityChanged\0parentChanged\0posChanged\0"
    "rotationChanged\0visibleChanged\0"
    "zValueChanged\0toolTipChanged\0toolTip\0"
    "flagsChanged\0selectedChanged\0"
    "newObjectGenerated\0MapGraphicsObject*\0"
    "redrawRequested\0keyFocusRequested\0"
    "setConstructed"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MapGraphicsObject[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      13,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   84,    2, 0x06 /* Public */,
       3,    0,   85,    2, 0x06 /* Public */,
       4,    0,   86,    2, 0x06 /* Public */,
       5,    0,   87,    2, 0x06 /* Public */,
       6,    0,   88,    2, 0x06 /* Public */,
       7,    0,   89,    2, 0x06 /* Public */,
       8,    0,   90,    2, 0x06 /* Public */,
       9,    1,   91,    2, 0x06 /* Public */,
      11,    0,   94,    2, 0x06 /* Public */,
      12,    0,   95,    2, 0x06 /* Public */,
      13,    1,   96,    2, 0x06 /* Public */,
      15,    0,   99,    2, 0x06 /* Public */,
      16,    0,  100,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      17,    0,  101,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   10,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 14,    2,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void MapGraphicsObject::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MapGraphicsObject *_t = static_cast<MapGraphicsObject *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->enabledChanged(); break;
        case 1: _t->opacityChanged(); break;
        case 2: _t->parentChanged(); break;
        case 3: _t->posChanged(); break;
        case 4: _t->rotationChanged(); break;
        case 5: _t->visibleChanged(); break;
        case 6: _t->zValueChanged(); break;
        case 7: _t->toolTipChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 8: _t->flagsChanged(); break;
        case 9: _t->selectedChanged(); break;
        case 10: _t->newObjectGenerated((*reinterpret_cast< MapGraphicsObject*(*)>(_a[1]))); break;
        case 11: _t->redrawRequested(); break;
        case 12: _t->keyFocusRequested(); break;
        case 13: _t->setConstructed(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 10:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< MapGraphicsObject* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MapGraphicsObject::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MapGraphicsObject::enabledChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (MapGraphicsObject::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MapGraphicsObject::opacityChanged)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (MapGraphicsObject::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MapGraphicsObject::parentChanged)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (MapGraphicsObject::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MapGraphicsObject::posChanged)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (MapGraphicsObject::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MapGraphicsObject::rotationChanged)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (MapGraphicsObject::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MapGraphicsObject::visibleChanged)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (MapGraphicsObject::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MapGraphicsObject::zValueChanged)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (MapGraphicsObject::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MapGraphicsObject::toolTipChanged)) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (MapGraphicsObject::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MapGraphicsObject::flagsChanged)) {
                *result = 8;
                return;
            }
        }
        {
            using _t = void (MapGraphicsObject::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MapGraphicsObject::selectedChanged)) {
                *result = 9;
                return;
            }
        }
        {
            using _t = void (MapGraphicsObject::*)(MapGraphicsObject * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MapGraphicsObject::newObjectGenerated)) {
                *result = 10;
                return;
            }
        }
        {
            using _t = void (MapGraphicsObject::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MapGraphicsObject::redrawRequested)) {
                *result = 11;
                return;
            }
        }
        {
            using _t = void (MapGraphicsObject::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MapGraphicsObject::keyFocusRequested)) {
                *result = 12;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MapGraphicsObject::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_MapGraphicsObject.data,
      qt_meta_data_MapGraphicsObject,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *MapGraphicsObject::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MapGraphicsObject::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MapGraphicsObject.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int MapGraphicsObject::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    }
    return _id;
}

// SIGNAL 0
void MapGraphicsObject::enabledChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void MapGraphicsObject::opacityChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void MapGraphicsObject::parentChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void MapGraphicsObject::posChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void MapGraphicsObject::rotationChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void MapGraphicsObject::visibleChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void MapGraphicsObject::zValueChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void MapGraphicsObject::toolTipChanged(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void MapGraphicsObject::flagsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 8, nullptr);
}

// SIGNAL 9
void MapGraphicsObject::selectedChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 9, nullptr);
}

// SIGNAL 10
void MapGraphicsObject::newObjectGenerated(MapGraphicsObject * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void MapGraphicsObject::redrawRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 11, nullptr);
}

// SIGNAL 12
void MapGraphicsObject::keyFocusRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 12, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
