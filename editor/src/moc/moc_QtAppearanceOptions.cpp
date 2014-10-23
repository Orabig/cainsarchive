/****************************************************************************
** Meta object code from reading C++ file 'QtAppearanceOptions.h'
**
** Created: Thu Oct 4 20:05:03 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../QtAppearanceOptions.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QtAppearanceOptions.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_AppearanceOptions[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      31,   19,   18,   18, 0x0a,
      82,   75,   18,   18, 0x0a,
     117,   18,   18,   18, 0x0a,
     142,   18,   18,   18, 0x0a,
     175,  169,   18,   18, 0x0a,
     232,  225,   18,   18, 0x0a,
     280,   18,   18,   18, 0x0a,
     301,   18,   18,   18, 0x0a,
     341,  332,   18,   18, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_AppearanceOptions[] = {
    "AppearanceOptions\0\0pAppearance\0"
    "actionSetCurrentAppearance(AppearanceView*)\0"
    "szText\0currentObjectIndexChanged(QString)\0"
    "actionAddButtonClicked()\0"
    "actionMinusButtonClicked()\0pItem\0"
    "actionDrawListItemDoubleClicked(QListWidgetItem*)\0"
    "nValue\0actionDrawListMaterialInputIntValueChanged(int)\0"
    "actionAlphaClicked()\0"
    "actionBackgroundColorClicked()\0newColor\0"
    "actionColorSelected(QColor)\0"
};

void AppearanceOptions::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        AppearanceOptions *_t = static_cast<AppearanceOptions *>(_o);
        switch (_id) {
        case 0: _t->actionSetCurrentAppearance((*reinterpret_cast< AppearanceView*(*)>(_a[1]))); break;
        case 1: _t->currentObjectIndexChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->actionAddButtonClicked(); break;
        case 3: _t->actionMinusButtonClicked(); break;
        case 4: _t->actionDrawListItemDoubleClicked((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 5: _t->actionDrawListMaterialInputIntValueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->actionAlphaClicked(); break;
        case 7: _t->actionBackgroundColorClicked(); break;
        case 8: _t->actionColorSelected((*reinterpret_cast< const QColor(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData AppearanceOptions::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject AppearanceOptions::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_AppearanceOptions,
      qt_meta_data_AppearanceOptions, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &AppearanceOptions::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *AppearanceOptions::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *AppearanceOptions::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AppearanceOptions))
        return static_cast<void*>(const_cast< AppearanceOptions*>(this));
    return QDialog::qt_metacast(_clname);
}

int AppearanceOptions::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
