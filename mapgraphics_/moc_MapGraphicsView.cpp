/****************************************************************************
** Meta object code from reading C++ file 'MapGraphicsView.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "MapGraphicsView.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MapGraphicsView.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MapGraphicsView_t {
    QByteArrayData data[15];
    char stringdata0[252];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MapGraphicsView_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MapGraphicsView_t qt_meta_stringdata_MapGraphicsView = {
    {
QT_MOC_LITERAL(0, 0, 15), // "MapGraphicsView"
QT_MOC_LITERAL(1, 16, 16), // "zoomLevelChanged"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 5), // "nZoom"
QT_MOC_LITERAL(4, 40, 27), // "handleChildMouseDoubleClick"
QT_MOC_LITERAL(5, 68, 12), // "QMouseEvent*"
QT_MOC_LITERAL(6, 81, 5), // "event"
QT_MOC_LITERAL(7, 87, 20), // "handleChildMouseMove"
QT_MOC_LITERAL(8, 108, 21), // "handleChildMousePress"
QT_MOC_LITERAL(9, 130, 23), // "handleChildMouseRelease"
QT_MOC_LITERAL(10, 154, 26), // "handleChildViewContextMenu"
QT_MOC_LITERAL(11, 181, 18), // "QContextMenuEvent*"
QT_MOC_LITERAL(12, 200, 26), // "handleChildViewScrollWheel"
QT_MOC_LITERAL(13, 227, 12), // "QWheelEvent*"
QT_MOC_LITERAL(14, 240, 11) // "renderTiles"

    },
    "MapGraphicsView\0zoomLevelChanged\0\0"
    "nZoom\0handleChildMouseDoubleClick\0"
    "QMouseEvent*\0event\0handleChildMouseMove\0"
    "handleChildMousePress\0handleChildMouseRelease\0"
    "handleChildViewContextMenu\0"
    "QContextMenuEvent*\0handleChildViewScrollWheel\0"
    "QWheelEvent*\0renderTiles"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MapGraphicsView[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   54,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    1,   57,    2, 0x09 /* Protected */,
       7,    1,   60,    2, 0x09 /* Protected */,
       8,    1,   63,    2, 0x09 /* Protected */,
       9,    1,   66,    2, 0x09 /* Protected */,
      10,    1,   69,    2, 0x09 /* Protected */,
      12,    1,   72,    2, 0x09 /* Protected */,
      14,    0,   75,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::UChar,    3,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void, 0x80000000 | 11,    6,
    QMetaType::Void, 0x80000000 | 13,    6,
    QMetaType::Void,

       0        // eod
};

void MapGraphicsView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MapGraphicsView *_t = static_cast<MapGraphicsView *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->zoomLevelChanged((*reinterpret_cast< quint8(*)>(_a[1]))); break;
        case 1: _t->handleChildMouseDoubleClick((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 2: _t->handleChildMouseMove((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 3: _t->handleChildMousePress((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 4: _t->handleChildMouseRelease((*reinterpret_cast< QMouseEvent*(*)>(_a[1]))); break;
        case 5: _t->handleChildViewContextMenu((*reinterpret_cast< QContextMenuEvent*(*)>(_a[1]))); break;
        case 6: _t->handleChildViewScrollWheel((*reinterpret_cast< QWheelEvent*(*)>(_a[1]))); break;
        case 7: _t->renderTiles(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MapGraphicsView::*)(quint8 );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MapGraphicsView::zoomLevelChanged)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MapGraphicsView::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_MapGraphicsView.data,
      qt_meta_data_MapGraphicsView,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *MapGraphicsView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MapGraphicsView::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MapGraphicsView.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "PrivateQGraphicsInfoSource"))
        return static_cast< PrivateQGraphicsInfoSource*>(this);
    return QWidget::qt_metacast(_clname);
}

int MapGraphicsView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void MapGraphicsView::zoomLevelChanged(quint8 _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
