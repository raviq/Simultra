/****************************************************************************
** Meta object code from reading C++ file 'weathimage.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../gui/weathimage.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'weathimage.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_WeatherImageObject_t {
    QByteArrayData data[6];
    char stringdata0[52];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_WeatherImageObject_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_WeatherImageObject_t qt_meta_stringdata_WeatherImageObject = {
    {
QT_MOC_LITERAL(0, 0, 18), // "WeatherImageObject"
QT_MOC_LITERAL(1, 19, 10), // "setWeather"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 3), // "img"
QT_MOC_LITERAL(4, 35, 12), // "sizeInMeters"
QT_MOC_LITERAL(5, 48, 3) // "pos"

    },
    "WeatherImageObject\0setWeather\0\0img\0"
    "sizeInMeters\0pos"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_WeatherImageObject[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    2,   24,    2, 0x0a /* Public */,
       1,    3,   29,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::QImage, QMetaType::QRectF,    3,    4,
    QMetaType::Void, QMetaType::QImage, QMetaType::QRectF, QMetaType::QPointF,    3,    4,    5,

       0        // eod
};

void WeatherImageObject::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        WeatherImageObject *_t = static_cast<WeatherImageObject *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->setWeather((*reinterpret_cast< const QImage(*)>(_a[1])),(*reinterpret_cast< const QRectF(*)>(_a[2]))); break;
        case 1: _t->setWeather((*reinterpret_cast< const QImage(*)>(_a[1])),(*reinterpret_cast< const QRectF(*)>(_a[2])),(*reinterpret_cast< const QPointF(*)>(_a[3]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject WeatherImageObject::staticMetaObject = {
    { &MapGraphicsObject::staticMetaObject, qt_meta_stringdata_WeatherImageObject.data,
      qt_meta_data_WeatherImageObject,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *WeatherImageObject::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *WeatherImageObject::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_WeatherImageObject.stringdata0))
        return static_cast<void*>(this);
    return MapGraphicsObject::qt_metacast(_clname);
}

int WeatherImageObject::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = MapGraphicsObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
