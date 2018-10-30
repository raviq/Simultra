/****************************************************************************
** Meta object code from reading C++ file 'PrivateQGraphicsObject.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "guts/PrivateQGraphicsObject.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'PrivateQGraphicsObject.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_PrivateQGraphicsObject_t {
    QByteArrayData data[17];
    char stringdata0[327];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_PrivateQGraphicsObject_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_PrivateQGraphicsObject_t qt_meta_stringdata_PrivateQGraphicsObject = {
    {
QT_MOC_LITERAL(0, 0, 22), // "PrivateQGraphicsObject"
QT_MOC_LITERAL(1, 23, 22), // "handleZoomLevelChanged"
QT_MOC_LITERAL(2, 46, 0), // ""
QT_MOC_LITERAL(3, 47, 20), // "handleEnabledChanged"
QT_MOC_LITERAL(4, 68, 20), // "handleOpacityChanged"
QT_MOC_LITERAL(5, 89, 19), // "handleParentChanged"
QT_MOC_LITERAL(6, 109, 16), // "handlePosChanged"
QT_MOC_LITERAL(7, 126, 21), // "handleRotationChanged"
QT_MOC_LITERAL(8, 148, 20), // "handleVisibleChanged"
QT_MOC_LITERAL(9, 169, 19), // "handleZValueChanged"
QT_MOC_LITERAL(10, 189, 23), // "handleMGSelectedChanged"
QT_MOC_LITERAL(11, 213, 22), // "handleMGToolTipChanged"
QT_MOC_LITERAL(12, 236, 7), // "toolTip"
QT_MOC_LITERAL(13, 244, 20), // "handleMGFlagsChanged"
QT_MOC_LITERAL(14, 265, 15), // "updateAllFromMG"
QT_MOC_LITERAL(15, 281, 21), // "handleRedrawRequested"
QT_MOC_LITERAL(16, 303, 23) // "handleKeyFocusRequested"

    },
    "PrivateQGraphicsObject\0handleZoomLevelChanged\0"
    "\0handleEnabledChanged\0handleOpacityChanged\0"
    "handleParentChanged\0handlePosChanged\0"
    "handleRotationChanged\0handleVisibleChanged\0"
    "handleZValueChanged\0handleMGSelectedChanged\0"
    "handleMGToolTipChanged\0toolTip\0"
    "handleMGFlagsChanged\0updateAllFromMG\0"
    "handleRedrawRequested\0handleKeyFocusRequested"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PrivateQGraphicsObject[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   84,    2, 0x0a /* Public */,
       3,    0,   85,    2, 0x08 /* Private */,
       4,    0,   86,    2, 0x08 /* Private */,
       5,    0,   87,    2, 0x08 /* Private */,
       6,    0,   88,    2, 0x08 /* Private */,
       7,    0,   89,    2, 0x08 /* Private */,
       8,    0,   90,    2, 0x08 /* Private */,
       9,    0,   91,    2, 0x08 /* Private */,
      10,    0,   92,    2, 0x08 /* Private */,
      11,    1,   93,    2, 0x08 /* Private */,
      13,    0,   96,    2, 0x08 /* Private */,
      14,    0,   97,    2, 0x08 /* Private */,
      15,    0,   98,    2, 0x08 /* Private */,
      16,    0,   99,    2, 0x08 /* Private */,

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
    QMetaType::Void, QMetaType::QString,   12,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void PrivateQGraphicsObject::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        PrivateQGraphicsObject *_t = static_cast<PrivateQGraphicsObject *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->handleZoomLevelChanged(); break;
        case 1: _t->handleEnabledChanged(); break;
        case 2: _t->handleOpacityChanged(); break;
        case 3: _t->handleParentChanged(); break;
        case 4: _t->handlePosChanged(); break;
        case 5: _t->handleRotationChanged(); break;
        case 6: _t->handleVisibleChanged(); break;
        case 7: _t->handleZValueChanged(); break;
        case 8: _t->handleMGSelectedChanged(); break;
        case 9: _t->handleMGToolTipChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 10: _t->handleMGFlagsChanged(); break;
        case 11: _t->updateAllFromMG(); break;
        case 12: _t->handleRedrawRequested(); break;
        case 13: _t->handleKeyFocusRequested(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject PrivateQGraphicsObject::staticMetaObject = {
    { &QGraphicsObject::staticMetaObject, qt_meta_stringdata_PrivateQGraphicsObject.data,
      qt_meta_data_PrivateQGraphicsObject,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *PrivateQGraphicsObject::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PrivateQGraphicsObject::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_PrivateQGraphicsObject.stringdata0))
        return static_cast<void*>(this);
    return QGraphicsObject::qt_metacast(_clname);
}

int PrivateQGraphicsObject::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 14;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
