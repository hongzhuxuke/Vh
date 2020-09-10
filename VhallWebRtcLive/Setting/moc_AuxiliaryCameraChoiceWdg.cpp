/****************************************************************************
** Meta object code from reading C++ file 'AuxiliaryCameraChoiceWdg.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "AuxiliaryCameraChoiceWdg.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AuxiliaryCameraChoiceWdg.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_AuxiliaryCameraChoiceWdg_t {
    QByteArrayData data[6];
    char stringdata0[105];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AuxiliaryCameraChoiceWdg_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AuxiliaryCameraChoiceWdg_t qt_meta_stringdata_AuxiliaryCameraChoiceWdg = {
    {
QT_MOC_LITERAL(0, 0, 24), // "AuxiliaryCameraChoiceWdg"
QT_MOC_LITERAL(1, 25, 19), // "slot_OnClickedApply"
QT_MOC_LITERAL(2, 45, 0), // ""
QT_MOC_LITERAL(3, 46, 19), // "slot_OnClickedClose"
QT_MOC_LITERAL(4, 66, 32), // "on_cmbcamera_currentIndexChanged"
QT_MOC_LITERAL(5, 99, 5) // "index"

    },
    "AuxiliaryCameraChoiceWdg\0slot_OnClickedApply\0"
    "\0slot_OnClickedClose\0"
    "on_cmbcamera_currentIndexChanged\0index"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AuxiliaryCameraChoiceWdg[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   29,    2, 0x08 /* Private */,
       3,    0,   30,    2, 0x08 /* Private */,
       4,    1,   31,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    5,

       0        // eod
};

void AuxiliaryCameraChoiceWdg::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<AuxiliaryCameraChoiceWdg *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->slot_OnClickedApply(); break;
        case 1: _t->slot_OnClickedClose(); break;
        case 2: _t->on_cmbcamera_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject AuxiliaryCameraChoiceWdg::staticMetaObject = { {
    &CBaseWnd::staticMetaObject,
    qt_meta_stringdata_AuxiliaryCameraChoiceWdg.data,
    qt_meta_data_AuxiliaryCameraChoiceWdg,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *AuxiliaryCameraChoiceWdg::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AuxiliaryCameraChoiceWdg::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_AuxiliaryCameraChoiceWdg.stringdata0))
        return static_cast<void*>(this);
    return CBaseWnd::qt_metacast(_clname);
}

int AuxiliaryCameraChoiceWdg::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = CBaseWnd::qt_metacall(_c, _id, _a);
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
QT_WARNING_POP
QT_END_MOC_NAMESPACE
