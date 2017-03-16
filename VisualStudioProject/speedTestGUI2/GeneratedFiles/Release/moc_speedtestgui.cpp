/****************************************************************************
** Meta object code from reading C++ file 'speedtestgui.h'
**
** Created: Tue 25. Sep 16:04:04 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "StdAfx.h"
#include "../../speedtestgui.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'speedtestgui.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_speedTestGUI[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x0a,
      49,   13,   13,   13, 0x0a,
      76,   13,   13,   13, 0x0a,
     105,   13,   13,   13, 0x0a,
     115,   13,   13,   13, 0x0a,
     130,  128,   13,   13, 0x0a,
     170,   13,   13,   13, 0x0a,
     188,   13,   13,   13, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_speedTestGUI[] = {
    "speedTestGUI\0\0pushButtonStatistFilesDirClicked()\0"
    "pushButtonTestDirClicked()\0"
    "pushButtonTestFilesClicked()\0addItem()\0"
    "deleteItem()\0,\0itemDoubleClicked(QTreeWidgetItem*,int)\0"
    "evaluateClicked()\0checkBoxChanged(int)\0"
};

void speedTestGUI::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        speedTestGUI *_t = static_cast<speedTestGUI *>(_o);
        switch (_id) {
        case 0: _t->pushButtonStatistFilesDirClicked(); break;
        case 1: _t->pushButtonTestDirClicked(); break;
        case 2: _t->pushButtonTestFilesClicked(); break;
        case 3: _t->addItem(); break;
        case 4: _t->deleteItem(); break;
        case 5: _t->itemDoubleClicked((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 6: _t->evaluateClicked(); break;
        case 7: _t->checkBoxChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData speedTestGUI::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject speedTestGUI::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_speedTestGUI,
      qt_meta_data_speedTestGUI, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &speedTestGUI::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *speedTestGUI::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *speedTestGUI::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_speedTestGUI))
        return static_cast<void*>(const_cast< speedTestGUI*>(this));
    return QDialog::qt_metacast(_clname);
}

int speedTestGUI::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
