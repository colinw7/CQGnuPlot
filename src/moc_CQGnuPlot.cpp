/****************************************************************************
** Meta object code from reading C++ file 'CQGnuPlot.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "CQGnuPlot.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CQGnuPlot.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CQGnuPlotCanvas_t {
    QByteArrayData data[1];
    char stringdata[17];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_CQGnuPlotCanvas_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_CQGnuPlotCanvas_t qt_meta_stringdata_CQGnuPlotCanvas = {
    {
QT_MOC_LITERAL(0, 0, 15)
    },
    "CQGnuPlotCanvas\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CQGnuPlotCanvas[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void CQGnuPlotCanvas::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject CQGnuPlotCanvas::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CQGnuPlotCanvas.data,
      qt_meta_data_CQGnuPlotCanvas,  qt_static_metacall, 0, 0}
};


const QMetaObject *CQGnuPlotCanvas::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CQGnuPlotCanvas::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CQGnuPlotCanvas.stringdata))
        return static_cast<void*>(const_cast< CQGnuPlotCanvas*>(this));
    return QWidget::qt_metacast(_clname);
}

int CQGnuPlotCanvas::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
struct qt_meta_stringdata_CQGnuPlotWindow_t {
    QByteArrayData data[5];
    char stringdata[43];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_CQGnuPlotWindow_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_CQGnuPlotWindow_t qt_meta_stringdata_CQGnuPlotWindow = {
    {
QT_MOC_LITERAL(0, 0, 15),
QT_MOC_LITERAL(1, 16, 9),
QT_MOC_LITERAL(2, 26, 0),
QT_MOC_LITERAL(3, 27, 4),
QT_MOC_LITERAL(4, 32, 9)
    },
    "CQGnuPlotWindow\0xAxisSlot\0\0show\0"
    "yAxisSlot\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CQGnuPlotWindow[] = {

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
       1,    1,   24,    2, 0x08,
       4,    1,   27,    2, 0x08,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Bool,    3,

       0        // eod
};

void CQGnuPlotWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CQGnuPlotWindow *_t = static_cast<CQGnuPlotWindow *>(_o);
        switch (_id) {
        case 0: _t->xAxisSlot((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->yAxisSlot((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject CQGnuPlotWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_CQGnuPlotWindow.data,
      qt_meta_data_CQGnuPlotWindow,  qt_static_metacall, 0, 0}
};


const QMetaObject *CQGnuPlotWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CQGnuPlotWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CQGnuPlotWindow.stringdata))
        return static_cast<void*>(const_cast< CQGnuPlotWindow*>(this));
    if (!strcmp(_clname, "CGnuPlotWindow"))
        return static_cast< CGnuPlotWindow*>(const_cast< CQGnuPlotWindow*>(this));
    if (!strcmp(_clname, "CQMouseModeIFace"))
        return static_cast< CQMouseModeIFace*>(const_cast< CQGnuPlotWindow*>(this));
    if (!strcmp(_clname, "CQPanZoomIFace"))
        return static_cast< CQPanZoomIFace*>(const_cast< CQGnuPlotWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int CQGnuPlotWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
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
QT_END_MOC_NAMESPACE
