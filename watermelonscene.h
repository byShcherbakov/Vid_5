#pragma once
#include "graphicsitems.h"
#include <QGraphicsScene>
class WatermelonScene final : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit WatermelonScene(QObject *parent = nullptr);
    int objectCount() const;
    ObjectCounts counts() const;
    CounterItem *counterItem() const { return counter_; }

public slots:
    void addExitButtons(int amount = 1);
    void addWatermelons(int amount = 1);
    void cutWatermelon(WatermelonItem *item);
    void removeSlice(WatermelonSliceItem *item);
    void clearObjects();
signals:
    void exitRequested();
    void countsChanged(int total, int buttons, int watermelons, int slices);
protected:
    void drawBackground(QPainter *painter, const QRectF &rect) override;

private:
    void registerObject(SceneObject *item, const QPointF &position);
    QPointF randomPosition(const SceneObject *item) const;
    void detachForDeletion(SceneObject *item);
    void updateCounter();
    CounterItem *counter_ = nullptr;
    qreal nextZ_ = 1;
};
