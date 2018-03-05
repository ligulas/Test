/****************************************************************************
** Meta object code from reading C++ file 'qdatastream_parsethread.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../qdatastream_parsethread.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qdatastream_parsethread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QDataStream_ParseThread_t {
    QByteArrayData data[5];
    char stringdata0[109];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QDataStream_ParseThread_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QDataStream_ParseThread_t qt_meta_stringdata_QDataStream_ParseThread = {
    {
QT_MOC_LITERAL(0, 0, 23), // "QDataStream_ParseThread"
QT_MOC_LITERAL(1, 24, 27), // "sig_container_size_notifier"
QT_MOC_LITERAL(2, 52, 0), // ""
QT_MOC_LITERAL(3, 53, 19), // "sig_Parse_data_info"
QT_MOC_LITERAL(4, 73, 35) // "on_deal_dataStream_fromCommun..."

    },
    "QDataStream_ParseThread\0"
    "sig_container_size_notifier\0\0"
    "sig_Parse_data_info\0"
    "on_deal_dataStream_fromCommunicator"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QDataStream_ParseThread[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   29,    2, 0x06 /* Public */,
       3,    5,   34,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   45,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    2,    2,
    QMetaType::Void, QMetaType::QString, QMetaType::Float, QMetaType::Float, QMetaType::Float, QMetaType::QDateTime,    2,    2,    2,    2,    2,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void QDataStream_ParseThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        QDataStream_ParseThread *_t = static_cast<QDataStream_ParseThread *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sig_container_size_notifier((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->sig_Parse_data_info((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2])),(*reinterpret_cast< float(*)>(_a[3])),(*reinterpret_cast< float(*)>(_a[4])),(*reinterpret_cast< QDateTime(*)>(_a[5]))); break;
        case 2: _t->on_deal_dataStream_fromCommunicator(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (QDataStream_ParseThread::*_t)(QString , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QDataStream_ParseThread::sig_container_size_notifier)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (QDataStream_ParseThread::*_t)(QString , float , float , float , QDateTime );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QDataStream_ParseThread::sig_Parse_data_info)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject QDataStream_ParseThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_QDataStream_ParseThread.data,
      qt_meta_data_QDataStream_ParseThread,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *QDataStream_ParseThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QDataStream_ParseThread::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_QDataStream_ParseThread.stringdata0))
        return static_cast<void*>(const_cast< QDataStream_ParseThread*>(this));
    return QThread::qt_metacast(_clname);
}

int QDataStream_ParseThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void QDataStream_ParseThread::sig_container_size_notifier(QString _t1, int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QDataStream_ParseThread::sig_Parse_data_info(QString _t1, float _t2, float _t3, float _t4, QDateTime _t5)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
