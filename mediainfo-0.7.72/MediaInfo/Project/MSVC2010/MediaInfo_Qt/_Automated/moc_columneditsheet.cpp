/****************************************************************************
** Meta object code from reading C++ file 'columneditsheet.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.0.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../Source/GUI/Qt/columneditsheet.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'columneditsheet.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.0.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ColumnEditSheet_t {
    QByteArrayData data[14];
    char stringdata[158];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_ColumnEditSheet_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_ColumnEditSheet_t qt_meta_stringdata_ColumnEditSheet = {
    {
QT_MOC_LITERAL(0, 0, 15),
QT_MOC_LITERAL(1, 16, 16),
QT_MOC_LITERAL(2, 33, 0),
QT_MOC_LITERAL(3, 34, 8),
QT_MOC_LITERAL(4, 43, 10),
QT_MOC_LITERAL(5, 54, 8),
QT_MOC_LITERAL(6, 63, 14),
QT_MOC_LITERAL(7, 78, 8),
QT_MOC_LITERAL(8, 87, 10),
QT_MOC_LITERAL(9, 98, 11),
QT_MOC_LITERAL(10, 110, 11),
QT_MOC_LITERAL(11, 122, 10),
QT_MOC_LITERAL(12, 133, 10),
QT_MOC_LITERAL(13, 144, 12)
    },
    "ColumnEditSheet\0somethingChanged\0\0"
    "moveMeUp\0moveMeDown\0removeMe\0"
    "contentChanged\0upButton\0downButton\0"
    "minusButton\0posSwitched\0posRemoved\0"
    "posChanged\0fillCombobox\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ColumnEditSheet[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   74,    2, 0x05,
       3,    1,   75,    2, 0x05,
       4,    1,   78,    2, 0x05,
       5,    1,   81,    2, 0x05,

 // slots: name, argc, parameters, tag, flags
       6,    0,   84,    2, 0x0a,
       7,    0,   85,    2, 0x0a,
       8,    0,   86,    2, 0x0a,
       9,    0,   87,    2, 0x0a,
      10,    3,   88,    2, 0x0a,
      11,    2,   95,    2, 0x0a,
      12,    1,  100,    2, 0x0a,
      13,    0,  103,    2, 0x0a,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int,    2,    2,    2,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    2,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,

       0        // eod
};

void ColumnEditSheet::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ColumnEditSheet *_t = static_cast<ColumnEditSheet *>(_o);
        switch (_id) {
        case 0: _t->somethingChanged(); break;
        case 1: _t->moveMeUp((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->moveMeDown((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->removeMe((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->contentChanged(); break;
        case 5: _t->upButton(); break;
        case 6: _t->downButton(); break;
        case 7: _t->minusButton(); break;
        case 8: _t->posSwitched((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 9: _t->posRemoved((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 10: _t->posChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->fillCombobox(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (ColumnEditSheet::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ColumnEditSheet::somethingChanged)) {
                *result = 0;
            }
        }
        {
            typedef void (ColumnEditSheet::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ColumnEditSheet::moveMeUp)) {
                *result = 1;
            }
        }
        {
            typedef void (ColumnEditSheet::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ColumnEditSheet::moveMeDown)) {
                *result = 2;
            }
        }
        {
            typedef void (ColumnEditSheet::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ColumnEditSheet::removeMe)) {
                *result = 3;
            }
        }
    }
}

const QMetaObject ColumnEditSheet::staticMetaObject = {
    { &QHBoxLayout::staticMetaObject, qt_meta_stringdata_ColumnEditSheet.data,
      qt_meta_data_ColumnEditSheet,  qt_static_metacall, 0, 0}
};


const QMetaObject *ColumnEditSheet::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ColumnEditSheet::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ColumnEditSheet.stringdata))
        return static_cast<void*>(const_cast< ColumnEditSheet*>(this));
    return QHBoxLayout::qt_metacast(_clname);
}

int ColumnEditSheet::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QHBoxLayout::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void ColumnEditSheet::somethingChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void ColumnEditSheet::moveMeUp(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ColumnEditSheet::moveMeDown(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void ColumnEditSheet::removeMe(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
