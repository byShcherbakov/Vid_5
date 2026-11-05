/****************************************************************************
** Meta object code from reading C++ file 'watermelonscene.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.12.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../watermelonscene.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'watermelonscene.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.12.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN15WatermelonSceneE_t {};
} // unnamed namespace

template <> constexpr inline auto WatermelonScene::qt_create_metaobjectdata<qt_meta_tag_ZN15WatermelonSceneE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "WatermelonScene",
        "exitRequested",
        "",
        "countsChanged",
        "total",
        "buttons",
        "watermelons",
        "slices",
        "addExitButtons",
        "amount",
        "addWatermelons",
        "cutWatermelon",
        "WatermelonItem*",
        "item",
        "removeSlice",
        "WatermelonSliceItem*",
        "clearObjects"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'exitRequested'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'countsChanged'
        QtMocHelpers::SignalData<void(int, int, int, int)>(3, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 4 }, { QMetaType::Int, 5 }, { QMetaType::Int, 6 }, { QMetaType::Int, 7 },
        }}),
        // Slot 'addExitButtons'
        QtMocHelpers::SlotData<void(int)>(8, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 9 },
        }}),
        // Slot 'addExitButtons'
        QtMocHelpers::SlotData<void()>(8, 2, QMC::AccessPublic | QMC::MethodCloned, QMetaType::Void),
        // Slot 'addWatermelons'
        QtMocHelpers::SlotData<void(int)>(10, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 9 },
        }}),
        // Slot 'addWatermelons'
        QtMocHelpers::SlotData<void()>(10, 2, QMC::AccessPublic | QMC::MethodCloned, QMetaType::Void),
        // Slot 'cutWatermelon'
        QtMocHelpers::SlotData<void(WatermelonItem *)>(11, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 12, 13 },
        }}),
        // Slot 'removeSlice'
        QtMocHelpers::SlotData<void(WatermelonSliceItem *)>(14, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 15, 13 },
        }}),
        // Slot 'clearObjects'
        QtMocHelpers::SlotData<void()>(16, 2, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<WatermelonScene, qt_meta_tag_ZN15WatermelonSceneE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject WatermelonScene::staticMetaObject = { {
    QMetaObject::SuperData::link<QGraphicsScene::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN15WatermelonSceneE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN15WatermelonSceneE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN15WatermelonSceneE_t>.metaTypes,
    nullptr
} };

void WatermelonScene::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<WatermelonScene *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->exitRequested(); break;
        case 1: _t->countsChanged((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[4]))); break;
        case 2: _t->addExitButtons((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 3: _t->addExitButtons(); break;
        case 4: _t->addWatermelons((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 5: _t->addWatermelons(); break;
        case 6: _t->cutWatermelon((*reinterpret_cast<std::add_pointer_t<WatermelonItem*>>(_a[1]))); break;
        case 7: _t->removeSlice((*reinterpret_cast<std::add_pointer_t<WatermelonSliceItem*>>(_a[1]))); break;
        case 8: _t->clearObjects(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 6:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< WatermelonItem* >(); break;
            }
            break;
        case 7:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< WatermelonSliceItem* >(); break;
            }
            break;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (WatermelonScene::*)()>(_a, &WatermelonScene::exitRequested, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (WatermelonScene::*)(int , int , int , int )>(_a, &WatermelonScene::countsChanged, 1))
            return;
    }
}

const QMetaObject *WatermelonScene::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *WatermelonScene::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN15WatermelonSceneE_t>.strings))
        return static_cast<void*>(this);
    return QGraphicsScene::qt_metacast(_clname);
}

int WatermelonScene::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsScene::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void WatermelonScene::exitRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void WatermelonScene::countsChanged(int _t1, int _t2, int _t3, int _t4)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1, _t2, _t3, _t4);
}
QT_WARNING_POP
