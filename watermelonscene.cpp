#include "watermelonscene.h"

#include <QPainter>
#include <QRandomGenerator>
#include <cmath>

WatermelonScene::WatermelonScene(QObject *parent) : QGraphicsScene(parent)
{
    setSceneRect(0, 0, 1200, 760);
    setItemIndexMethod(QGraphicsScene::NoIndex);
    counter_ = new CounterItem;
    registerObject(counter_, QPointF(850, 22));
    registerObject(new WatermelonItem, QPointF(245, 255));
    registerObject(new ExitButtonItem, QPointF(520, 320));
    updateCounter();
}

int WatermelonScene::objectCount() const { return static_cast<int>(items().size()); }

ObjectCounts WatermelonScene::counts() const
{
    ObjectCounts result;
    for (QGraphicsItem *item : items()) {
        auto *object = dynamic_cast<SceneObject *>(item);
        if (!object)
            continue;
        switch (object->kind()) {
        case SceneObject::Kind::ExitButton: ++result.buttons; break;
        case SceneObject::Kind::Watermelon: ++result.watermelons; break;
        case SceneObject::Kind::Slice: ++result.slices; break;
        case SceneObject::Kind::Counter: ++result.counters; break;
        }
    }
    return result;
}

void WatermelonScene::registerObject(SceneObject *item, const QPointF &position)
{
    addItem(item);
    item->setPos(position);
    if (item->kind() != SceneObject::Kind::Counter)
        item->setZValue(nextZ_++);
    connect(item, &SceneObject::picked, this, [this](SceneObject *picked) {
        if (picked->kind() != SceneObject::Kind::Counter)
            picked->setZValue(nextZ_++);
    });
    if (auto *button = qobject_cast<ExitButtonItem *>(item))
        connect(button, &ExitButtonItem::exitRequested, this, &WatermelonScene::exitRequested);
    if (auto *watermelon = qobject_cast<WatermelonItem *>(item))
        connect(watermelon, &WatermelonItem::cutRequested, this, &WatermelonScene::cutWatermelon);
    if (auto *slice = qobject_cast<WatermelonSliceItem *>(item))
        connect(slice, &WatermelonSliceItem::removeRequested, this, &WatermelonScene::removeSlice);
}

QPointF WatermelonScene::randomPosition(const SceneObject *item) const
{
    const QRectF area = sceneRect().adjusted(16, 16, -16, -16);
    const QRectF bounds = item->boundingRect();
    QPointF fallback(area.left(), area.bottom() - bounds.height());
    for (int attempt = 0; attempt < 48; ++attempt) {
        const qreal x = area.left() + QRandomGenerator::global()->generateDouble()
            * (area.width() - bounds.width());
        const qreal y = area.top() + QRandomGenerator::global()->generateDouble()
            * (area.height() - bounds.height());
        const QPointF position(x, y);
        const QRectF candidate = bounds.translated(position);
        // Keep the counter readable; overlaps among other items are allowed
        // when the scene becomes crowded, so there is no 256-item placement cap.
        if (counter_ && candidate.intersects(counter_->sceneBoundingRect().adjusted(-8, -8, 8, 8)))
            continue;
        fallback = position;
        if (objectCount() > 64 || items(candidate, Qt::IntersectsItemBoundingRect).isEmpty())
            return position;
    }
    return fallback;
}

void WatermelonScene::addExitButtons(int amount)
{
    for (int i = 0; i < amount; ++i) {
        auto *button = new ExitButtonItem;
        registerObject(button, randomPosition(button));
    }
    updateCounter();
}

void WatermelonScene::addWatermelons(int amount)
{
    for (int i = 0; i < amount; ++i) {
        auto *watermelon = new WatermelonItem;
        registerObject(watermelon, randomPosition(watermelon));
    }
    updateCounter();
}

void WatermelonScene::detachForDeletion(SceneObject *item)
{
    removeItem(item);
    item->setEnabled(false);
    // A right-click handler is still on the stack: defer destruction safely.
    // Parent removed objects to the scene until DeferredDelete is processed.
    item->QObject::setParent(this);
    item->deleteLater();
}

void WatermelonScene::cutWatermelon(WatermelonItem *item)
{
    if (!item || item->scene() != this)
        return;
    detachForDeletion(item);
    for (int i = 0; i < 8; ++i) {
        auto *slice = new WatermelonSliceItem;
        registerObject(slice, randomPosition(slice));
    }
    updateCounter(); // Replacing one watermelon with eight slices adds seven.
}

void WatermelonScene::removeSlice(WatermelonSliceItem *item)
{
    if (!item || item->scene() != this)
        return;
    detachForDeletion(item);
    updateCounter();
}

void WatermelonScene::clearObjects()
{
    // Do not use QGraphicsScene::clear(): the permanent counter must survive.
    const auto snapshot = items();
    for (QGraphicsItem *item : snapshot) {
        if (item != counter_) {
            removeItem(item);
            delete item;
        }
    }
    nextZ_ = 1;
    counter_->setPos(850, 22);
    updateCounter();
}

void WatermelonScene::updateCounter()
{
    const ObjectCounts value = counts();
    counter_->setCounts(value);
    emit countsChanged(objectCount(), value.buttons, value.watermelons, value.slices);
}

void WatermelonScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    painter->fillRect(rect, QColor("#f5f6ef"));
    painter->save();
    painter->setPen(QPen(QColor("#d9e1d5"), 1));
    constexpr int grid = 28;
    const int left = static_cast<int>(std::floor(rect.left() / grid)) * grid;
    const int top = static_cast<int>(std::floor(rect.top() / grid)) * grid;
    for (int x = left; x <= rect.right(); x += grid)
        for (int y = top; y <= rect.bottom(); y += grid)
            painter->drawPoint(QPointF(x, y));
    painter->restore();
    // The background is painted, not added as items, so it does not affect count.
}
