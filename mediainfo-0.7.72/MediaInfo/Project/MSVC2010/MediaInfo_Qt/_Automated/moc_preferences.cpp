/****************************************************************************
** Meta object code from reading C++ file 'preferences.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.0.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../Source/GUI/Qt/preferences.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'preferences.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.0.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Preferences_t {
    QByteArrayData data[15];
    char stringdata[436];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_Preferences_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_Preferences_t qt_meta_stringdata_Preferences = {
    {
QT_MOC_LITERAL(0, 0, 11),
QT_MOC_LITERAL(1, 12, 37),
QT_MOC_LITERAL(2, 50, 0),
QT_MOC_LITERAL(3, 51, 5),
QT_MOC_LITERAL(4, 57, 34),
QT_MOC_LITERAL(5, 92, 32),
QT_MOC_LITERAL(6, 125, 31),
QT_MOC_LITERAL(7, 157, 39),
QT_MOC_LITERAL(8, 197, 36),
QT_MOC_LITERAL(9, 234, 33),
QT_MOC_LITERAL(10, 268, 34),
QT_MOC_LITERAL(11, 303, 33),
QT_MOC_LITERAL(12, 337, 30),
QT_MOC_LITERAL(13, 368, 31),
QT_MOC_LITERAL(14, 400, 34)
    },
    "Preferences\0on_customComboBox_currentIndexChanged\0"
    "\0index\0on_pushButton_deleteCustom_clicked\0"
    "on_pushButton_editCustom_clicked\0"
    "on_pushButton_newCustom_clicked\0"
    "on_treeTextComboBox_currentIndexChanged\0"
    "on_pushButton_deleteTreeText_clicked\0"
    "on_pushButton_newTreeText_clicked\0"
    "on_pushButton_editTreeText_clicked\0"
    "on_pushButton_deleteSheet_clicked\0"
    "on_pushButton_newSheet_clicked\0"
    "on_pushButton_editSheet_clicked\0"
    "on_treeWidget_itemSelectionChanged\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Preferences[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   74,    2, 0x08,
       4,    0,   77,    2, 0x08,
       5,    0,   78,    2, 0x08,
       6,    0,   79,    2, 0x08,
       7,    1,   80,    2, 0x08,
       8,    0,   83,    2, 0x08,
       9,    0,   84,    2, 0x08,
      10,    0,   85,    2, 0x08,
      11,    0,   86,    2, 0x08,
      12,    0,   87,    2, 0x08,
      13,    0,   88,    2, 0x08,
      14,    0,   89,    2, 0x08,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Preferences::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Preferences *_t = static_cast<Preferences *>(_o);
        switch (_id) {
        case 0: _t->on_customComboBox_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->on_pushButton_deleteCustom_clicked(); break;
        case 2: _t->on_pushButton_editCustom_clicked(); break;
        case 3: _t->on_pushButton_newCustom_clicked(); break;
        case 4: _t->on_treeTextComboBox_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->on_pushButton_deleteTreeText_clicked(); break;
        case 6: _t->on_pushButton_newTreeText_clicked(); break;
        case 7: _t->on_pushButton_editTreeText_clicked(); break;
        case 8: _t->on_pushButton_deleteSheet_clicked(); break;
        case 9: _t->on_pushButton_newSheet_clicked(); break;
        case 10: _t->on_pushButton_editSheet_clicked(); break;
        case 11: _t->on_treeWidget_itemSelectionChanged(); break;
        default: ;
        }
    }
}

const QMetaObject Preferences::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_Preferences.data,
      qt_meta_data_Preferences,  qt_static_metacall, 0, 0}
};


const QMetaObject *Preferences::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Preferences::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Preferences.stringdata))
        return static_cast<void*>(const_cast< Preferences*>(this));
    return QDialog::qt_metacast(_clname);
}

int Preferences::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
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
QT_END_MOC_NAMESPACE
