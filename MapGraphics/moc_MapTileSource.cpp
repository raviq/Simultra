/****************************************************************************
** Meta object code from reading C++ file 'MapTileSource.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "MapTileSource.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MapTileSource.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MapTileSource_t {
    QByteArrayData data[10];
    char stringdata0[101];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MapTileSource_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MapTileSource_t qt_meta_stringdata_MapTileSource = {
    {
QT_MOC_LITERAL(0, 0, 13), // "MapTileSource"
QT_MOC_LITERAL(1, 14, 13), // "tileRetrieved"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 1), // "x"
QT_MOC_LITERAL(4, 31, 1), // "y"
QT_MOC_LITERAL(5, 33, 1), // "z"
QT_MOC_LITERAL(6, 35, 13), // "tileRequested"
QT_MOC_LITERAL(7, 49, 19), // "allTilesInvalidated"
QT_MOC_LITERAL(8, 69, 16), // "startTileRequest"
QT_MOC_LITERAL(9, 86, 14) // "clearTempCache"

    },
    "MapTileSource\0tileRetrieved\0\0x\0y\0z\0"
    "tileRequested\0allTilesInvalidated\0"
    "startTileRequest\0clearTempCache"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MapTileSource[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    3,   39,    2, 0x06 /* Public */,
       6,    3,   46,    2, 0x06 /* Public */,
       7,    0,   53,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    3,   54,    2, 0x08 /* Private */,
       9,    0,   61,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::UInt, QMetaType::UInt, QMetaType::UChar,    3,    4,    5,
    QMetaType::Void, QMetaType::UInt, QMetaType::UInt, QMetaType::UChar,    3,    4,    5,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::UInt, QMetaType::UInt, QMetaType::UChar,    3,    4,    5,
    QMetaType::Void,

       0        // eod
};

void MapTileSource::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MapTileSource *_t = static_cast<MapTileSource *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->tileRetrieved((*reinterpret_cast< quint32(*)>(_a[1])),(*reinterpret_cast< quint32(*)>(_a[2])),(*reinterpret_cast< quint8(*)>(_a[3]))); break;
        case 1: _t->tileRequested((*reinterpret_cast< quint32(*)>(_a[1])),(*reinterpret_cast< quint32(*)>(_a[2])),(*reinterpret_cast< quint8(*)>(_a[3]))); break;
        case 2: _t->allTilesInvalidated(); break;
        case 3: _t->startTileRequest((*reinterpret_cast< quint32(*)>(_a[1])),(*reinterpret_cast< quint32(*)>(_a[2])),(*reinterpret_cast< quint8(*)>(_a[3]))); break;
        case 4: _t->clearTempCache(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MapTileSource::*)(quint32 , quint32 , quint8 );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MapTileSource::tileRetrieved)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (MapTileSource::*)(quint32 , quint32 , quint8 );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MapTileSource::tileRequested)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (MapTileSource::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MapTileSource::allTilesInvalidated)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MapTileSource::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_MapTileSource.data,
      qt_meta_data_MapTileSource,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *MapTileSource::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MapTileSource::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MapTileSource.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int MapTileSource::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void MapTileSource::tileRetrieved(quint32 _t1, quint32 _t2, quint8 _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MapTileSource::tileRequested(quint32 _t1, quint32 _t2, quint8 _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void MapTileSource::allTilesInvalidated()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
