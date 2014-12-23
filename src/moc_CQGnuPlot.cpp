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
    QByteArrayData data[23];
    char stringdata[210];
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
QT_MOC_LITERAL(4, 32, 9),
QT_MOC_LITERAL(5, 42, 8),
QT_MOC_LITERAL(6, 51, 7),
QT_MOC_LITERAL(7, 59, 7),
QT_MOC_LITERAL(8, 67, 7),
QT_MOC_LITERAL(9, 75, 6),
QT_MOC_LITERAL(10, 82, 6),
QT_MOC_LITERAL(11, 89, 6),
QT_MOC_LITERAL(12, 96, 6),
QT_MOC_LITERAL(13, 103, 6),
QT_MOC_LITERAL(14, 110, 5),
QT_MOC_LITERAL(15, 116, 10),
QT_MOC_LITERAL(16, 127, 11),
QT_MOC_LITERAL(17, 139, 9),
QT_MOC_LITERAL(18, 149, 12),
QT_MOC_LITERAL(19, 162, 8),
QT_MOC_LITERAL(20, 171, 9),
QT_MOC_LITERAL(21, 181, 9),
QT_MOC_LITERAL(22, 191, 17)
    },
    "CQGnuPlotWindow\0xAxisSlot\0\0show\0"
    "yAxisSlot\0enable3D\0rotateX\0rotateY\0"
    "rotateZ\0xmin3D\0xmax3D\0ymin3D\0ymax3D\0"
    "near3D\0far3D\0marginLeft\0marginRight\0"
    "marginTop\0marginBottom\0hidden3D\0"
    "surface3D\0contour3D\0trianglePattern3D\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CQGnuPlotWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
      18,   30, // properties
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

 // properties: name, type, flags
       5, QMetaType::Bool, 0x00095103,
       6, QMetaType::Double, 0x00095103,
       7, QMetaType::Double, 0x00095103,
       8, QMetaType::Double, 0x00095103,
       9, QMetaType::Double, 0x00095003,
      10, QMetaType::Double, 0x00095003,
      11, QMetaType::Double, 0x00095003,
      12, QMetaType::Double, 0x00095003,
      13, QMetaType::Double, 0x00095103,
      14, QMetaType::Double, 0x00095103,
      15, QMetaType::Double, 0x00095103,
      16, QMetaType::Double, 0x00095103,
      17, QMetaType::Double, 0x00095103,
      18, QMetaType::Double, 0x00095103,
      19, QMetaType::Bool, 0x00095103,
      20, QMetaType::Bool, 0x00095103,
      21, QMetaType::Bool, 0x00095103,
      22, QMetaType::Int, 0x00095103,

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
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< bool*>(_v) = enable3D(); break;
        case 1: *reinterpret_cast< double*>(_v) = rotateX(); break;
        case 2: *reinterpret_cast< double*>(_v) = rotateY(); break;
        case 3: *reinterpret_cast< double*>(_v) = rotateZ(); break;
        case 4: *reinterpret_cast< double*>(_v) = xmin3D(); break;
        case 5: *reinterpret_cast< double*>(_v) = xmax3D(); break;
        case 6: *reinterpret_cast< double*>(_v) = ymin3D(); break;
        case 7: *reinterpret_cast< double*>(_v) = ymax3D(); break;
        case 8: *reinterpret_cast< double*>(_v) = near3D(); break;
        case 9: *reinterpret_cast< double*>(_v) = far3D(); break;
        case 10: *reinterpret_cast< double*>(_v) = marginLeft(); break;
        case 11: *reinterpret_cast< double*>(_v) = marginRight(); break;
        case 12: *reinterpret_cast< double*>(_v) = marginTop(); break;
        case 13: *reinterpret_cast< double*>(_v) = marginBottom(); break;
        case 14: *reinterpret_cast< bool*>(_v) = hidden3D(); break;
        case 15: *reinterpret_cast< bool*>(_v) = surface3D(); break;
        case 16: *reinterpret_cast< bool*>(_v) = contour3D(); break;
        case 17: *reinterpret_cast< int*>(_v) = trianglePattern3D(); break;
        }
        _id -= 18;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setEnable3D(*reinterpret_cast< bool*>(_v)); break;
        case 1: setRotateX(*reinterpret_cast< double*>(_v)); break;
        case 2: setRotateY(*reinterpret_cast< double*>(_v)); break;
        case 3: setRotateZ(*reinterpret_cast< double*>(_v)); break;
        case 4: setXMin3D(*reinterpret_cast< double*>(_v)); break;
        case 5: setXMax3D(*reinterpret_cast< double*>(_v)); break;
        case 6: setYMin3D(*reinterpret_cast< double*>(_v)); break;
        case 7: setYMax3D(*reinterpret_cast< double*>(_v)); break;
        case 8: setNear3D(*reinterpret_cast< double*>(_v)); break;
        case 9: setFar3D(*reinterpret_cast< double*>(_v)); break;
        case 10: setMarginLeft(*reinterpret_cast< double*>(_v)); break;
        case 11: setMarginRight(*reinterpret_cast< double*>(_v)); break;
        case 12: setMarginTop(*reinterpret_cast< double*>(_v)); break;
        case 13: setMarginBottom(*reinterpret_cast< double*>(_v)); break;
        case 14: setHidden3D(*reinterpret_cast< bool*>(_v)); break;
        case 15: setSurface3D(*reinterpret_cast< bool*>(_v)); break;
        case 16: setContour3D(*reinterpret_cast< bool*>(_v)); break;
        case 17: setTrianglePattern3D(*reinterpret_cast< int*>(_v)); break;
        }
        _id -= 18;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 18;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 18;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 18;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 18;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 18;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 18;
    } else if (_c == QMetaObject::RegisterPropertyMetaType) {
        if (_id < 18)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 18;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
QT_END_MOC_NAMESPACE
