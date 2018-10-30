/****************************************************************************
** Meta object code from reading C++ file 'CompositeTileSource.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "tileSources/CompositeTileSource.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CompositeTileSource.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CompositeTileSource_t {
    QByteArrayData data[12];
    char stringdata0[129];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CompositeTileSource_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CompositeTileSource_t qt_meta_stringdata_CompositeTileSource = {
    {
QT_MOC_LITERAL(0, 0, 19), // "CompositeTileSource"
QT_MOC_LITERAL(1, 20, 14), // "sourcesChanged"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 11), // "sourceAdded"
QT_MOC_LITERAL(4, 48, 5), // "index"
QT_MOC_LITERAL(5, 54, 13), // "sourceRemoved"
QT_MOC_LITERAL(6, 68, 16), // "sourcesReordered"
QT_MOC_LITERAL(7, 85, 19), // "handleTileRetrieved"
QT_MOC_LITERAL(8, 105, 1), // "x"
QT_MOC_LITERAL(9, 107, 1), // "y"
QT_MOC_LITERAL(10, 109, 1), // "z"
QT_MOC_LITERAL(11, 111, 17) // "clearPendingTiles"

    },
    "CompositeTileSource\0sourcesChanged\0\0"
    "sourceAdded\0index\0sourceRemoved\0"
    "sourcesReordered\0handleTileRetrieved\0"
    "x\0y\0z\0clearPendingTiles"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CompositeTileSource[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   44,    2, 0x06 /* Public */,
       3,    1,   45,    2, 0x06 /* Public */,
       5,    1,   48,    2, 0x06 /* Public */,
       6,    0,   51,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    3,   52,    2, 0x08 /* Private */,
      11,    0,   59,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::UInt, QMetaType::UInt, QMetaType::UChar,    8,    9,   10,
    QMetaType::Void,

       0        // eod
};

void CompositeTileSource::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CompositeTileSource *_t = static_cast<CompositeTileSource *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sourcesChanged(); break;
        case 1: _t->sourceAdded((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->sourceRemoved((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->sourcesReordered(); break;
        case 4: _t->handleTileRetrieved((*reinterpret_cast< quint32(*)>(_a[1])),(*reinterpret_cast< quint32(*)>(_a[2])),(*reinterpret_cast< quint8(*)>(_a[3]))); break;
        case 5: _t->clearPendingTiles(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (CompositeTileSource::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CompositeTileSource::sourcesChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (CompositeTileSource::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CompositeTileSource::sourceAdded)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (CompositeTileSource::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CompositeTileSource::sourceRemoved)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (CompositeTileSource::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CompositeTileSource::sourcesReordered)) {
                *result = 3;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject CompositeTileSource::staticMetaObject = {
    { &MapTileSource::staticMetaObject, qt_meta_stringdata_CompositeTileSource.data,
      qt_meta_data_CompositeTileSource,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *CompositeTileSource::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CompositeTileSource::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CompositeTileSource.stringdata0))
        return static_cast<void*>(this);
    return MapTileSource::qt_metacast(_clname);
}

int CompositeTileSource::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = MapTileSource::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void CompositeTileSource::sourcesChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void CompositeTileSource::sourceAdded(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CompositeTileSource::sourceRemoved(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CompositeTileSource::sourcesReordered()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
