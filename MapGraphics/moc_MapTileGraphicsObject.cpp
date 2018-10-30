/****************************************************************************
** Meta object code from reading C++ file 'MapTileGraphicsObject.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "guts/MapTileGraphicsObject.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MapTileGraphicsObject.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MapTileGraphicsObject_t {
    QByteArrayData data[8];
    char stringdata0[86];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MapTileGraphicsObject_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MapTileGraphicsObject_t qt_meta_stringdata_MapTileGraphicsObject = {
    {
QT_MOC_LITERAL(0, 0, 21), // "MapTileGraphicsObject"
QT_MOC_LITERAL(1, 22, 13), // "tileRequested"
QT_MOC_LITERAL(2, 36, 0), // ""
QT_MOC_LITERAL(3, 37, 1), // "x"
QT_MOC_LITERAL(4, 39, 1), // "y"
QT_MOC_LITERAL(5, 41, 1), // "z"
QT_MOC_LITERAL(6, 43, 19), // "handleTileRetrieved"
QT_MOC_LITERAL(7, 63, 22) // "handleTileInvalidation"

    },
    "MapTileGraphicsObject\0tileRequested\0"
    "\0x\0y\0z\0handleTileRetrieved\0"
    "handleTileInvalidation"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MapTileGraphicsObject[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    3,   29,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    3,   36,    2, 0x08 /* Private */,
       7,    0,   43,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::UInt, QMetaType::UInt, QMetaType::UChar,    3,    4,    5,

 // slots: parameters
    QMetaType::Void, QMetaType::UInt, QMetaType::UInt, QMetaType::UChar,    3,    4,    5,
    QMetaType::Void,

       0        // eod
};

void MapTileGraphicsObject::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MapTileGraphicsObject *_t = static_cast<MapTileGraphicsObject *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->tileRequested((*reinterpret_cast< quint32(*)>(_a[1])),(*reinterpret_cast< quint32(*)>(_a[2])),(*reinterpret_cast< quint8(*)>(_a[3]))); break;
        case 1: _t->handleTileRetrieved((*reinterpret_cast< quint32(*)>(_a[1])),(*reinterpret_cast< quint32(*)>(_a[2])),(*reinterpret_cast< quint8(*)>(_a[3]))); break;
        case 2: _t->handleTileInvalidation(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MapTileGraphicsObject::*)(quint32 , quint32 , quint8 );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MapTileGraphicsObject::tileRequested)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MapTileGraphicsObject::staticMetaObject = {
    { &QGraphicsObject::staticMetaObject, qt_meta_stringdata_MapTileGraphicsObject.data,
      qt_meta_data_MapTileGraphicsObject,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *MapTileGraphicsObject::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MapTileGraphicsObject::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MapTileGraphicsObject.stringdata0))
        return static_cast<void*>(this);
    return QGraphicsObject::qt_metacast(_clname);
}

int MapTileGraphicsObject::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void MapTileGraphicsObject::tileRequested(quint32 _t1, quint32 _t2, quint8 _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
