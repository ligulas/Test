/****************************************************************************
** Meta object code from reading C++ file 'qwritedb_thread.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../qwritedb_thread.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qwritedb_thread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QWriteDB_thread_t {
    QByteArrayData data[10];
    char stringdata0[141];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QWriteDB_thread_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QWriteDB_thread_t qt_meta_stringdata_QWriteDB_thread = {
    {
QT_MOC_LITERAL(0, 0, 15), // "QWriteDB_thread"
QT_MOC_LITERAL(1, 16, 22), // "deal_store_lx_DB_event"
QT_MOC_LITERAL(2, 39, 0), // ""
QT_MOC_LITERAL(3, 40, 14), // "device_addr_ip"
QT_MOC_LITERAL(4, 55, 15), // "data_list_count"
QT_MOC_LITERAL(5, 71, 22), // "deal_store_zx_DB_event"
QT_MOC_LITERAL(6, 94, 9), // "a_phase_f"
QT_MOC_LITERAL(7, 104, 9), // "b_phase_f"
QT_MOC_LITERAL(8, 114, 9), // "c_phase_f"
QT_MOC_LITERAL(9, 124, 16) // "new_data_rcvTime"

    },
    "QWriteDB_thread\0deal_store_lx_DB_event\0"
    "\0device_addr_ip\0data_list_count\0"
    "deal_store_zx_DB_event\0a_phase_f\0"
    "b_phase_f\0c_phase_f\0new_data_rcvTime"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QWriteDB_thread[] = {

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
       5,    5,   29,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    3,    4,
    QMetaType::Void, QMetaType::QString, QMetaType::Float, QMetaType::Float, QMetaType::Float, QMetaType::QDateTime,    3,    6,    7,    8,    9,

       0        // eod
};

void QWriteDB_thread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        QWriteDB_thread *_t = static_cast<QWriteDB_thread *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->deal_store_lx_DB_event((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->deal_store_zx_DB_event((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2])),(*reinterpret_cast< float(*)>(_a[3])),(*reinterpret_cast< float(*)>(_a[4])),(*reinterpret_cast< QDateTime(*)>(_a[5]))); break;
        default: ;
        }
    }
}

const QMetaObject QWriteDB_thread::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QWriteDB_thread.data,
      qt_meta_data_QWriteDB_thread,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *QWriteDB_thread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QWriteDB_thread::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_QWriteDB_thread.stringdata0))
        return static_cast<void*>(const_cast< QWriteDB_thread*>(this));
    return QObject::qt_metacast(_clname);
}

int QWriteDB_thread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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
