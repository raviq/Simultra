/****************************************************************************
** Meta object code from reading C++ file 'PolygonObject.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "PolygonObject.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'PolygonObject.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_PolygonObject_t {
    QByteArrayData data[7];
    char stringdata0[118];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_PolygonObject_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_PolygonObject_t qt_meta_stringdata_PolygonObject = {
    {
QT_MOC_LITERAL(0, 0, 13), // "PolygonObject"
QT_MOC_LITERAL(1, 14, 14), // "polygonChanged"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 4), // "poly"
QT_MOC_LITERAL(4, 35, 26), // "handleEditCirclePosChanged"
QT_MOC_LITERAL(5, 62, 29), // "handleAddVertexCircleSelected"
QT_MOC_LITERAL(6, 92, 25) // "handleEditCircleDestroyed"

    },
    "PolygonObject\0polygonChanged\0\0poly\0"
    "handleEditCirclePosChanged\0"
    "handleAddVertexCircleSelected\0"
    "handleEditCircleDestroyed"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PolygonObject[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   37,    2, 0x08 /* Private */,
       5,    0,   38,    2, 0x08 /* Private */,
       6,    0,   39,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QPolygonF,    3,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void PolygonObject::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        PolygonObject *_t = static_cast<PolygonObject *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->polygonChanged((*reinterpret_cast< const QPolygonF(*)>(_a[1]))); break;
        case 1: _t->handleEditCirclePosChanged(); break;
        case 2: _t->handleAddVertexCircleSelected(); break;
        case 3: _t->handleEditCircleDestroyed(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (PolygonObject::*)(const QPolygonF & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&PolygonObject::polygonChanged)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject PolygonObject::staticMetaObject = {
    { &MapGraphicsObject::staticMetaObject, qt_meta_stringdata_PolygonObject.data,
      qt_meta_data_PolygonObject,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *PolygonObject::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PolygonObject::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_PolygonObject.stringdata0))
        return static_cast<void*>(this);
    return MapGraphicsObject::qt_metacast(_clname);
}

int PolygonObject::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = MapGraphicsObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void PolygonObject::polygonChanged(const QPolygonF & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
