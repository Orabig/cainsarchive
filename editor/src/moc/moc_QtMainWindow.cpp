/****************************************************************************
** Meta object code from reading C++ file 'QtMainWindow.h'
**
** Created: Thu Oct 4 20:05:03 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../QtMainWindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QtMainWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainWindow[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      24,   12,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
      78,   71,   11,   11, 0x0a,
     107,   71,   11,   11, 0x0a,
     145,  136,   11,   11, 0x0a,
     186,  136,   11,   11, 0x0a,
     224,   11,   11,   11, 0x0a,
     254,   11,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0pAppearance\0"
    "actionUpdateCurrentAppearance(AppearanceView*)\0"
    "nIndex\0actionTabCurrentChanged(int)\0"
    "actionTabCloseRequested(int)\0pSNOFile\0"
    "actionOpenAppearance(SNOFiles::SNOFile*)\0"
    "actionOpenTexture(SNOFiles::SNOFile*)\0"
    "actionViewAppearanceOptions()\0"
    "actionViewAssetBrowser()\0"
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MainWindow *_t = static_cast<MainWindow *>(_o);
        switch (_id) {
        case 0: _t->actionUpdateCurrentAppearance((*reinterpret_cast< AppearanceView*(*)>(_a[1]))); break;
        case 1: _t->actionTabCurrentChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->actionTabCloseRequested((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->actionOpenAppearance((*reinterpret_cast< SNOFiles::SNOFile*(*)>(_a[1]))); break;
        case 4: _t->actionOpenTexture((*reinterpret_cast< SNOFiles::SNOFile*(*)>(_a[1]))); break;
        case 5: _t->actionViewAppearanceOptions(); break;
        case 6: _t->actionViewAssetBrowser(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData MainWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow,
      qt_meta_data_MainWindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void MainWindow::actionUpdateCurrentAppearance(AppearanceView * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
