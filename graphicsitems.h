#pragma once
#include <QGraphicsObject>
#include <QPainterPath>
#include <QPoint>
class QGraphicsSceneMouseEvent;
class QGraphicsSceneHoverEvent;
class QPainter;
struct ObjectCounts
{
    int buttons = 0;
    int watermelons = 0;
    int slices = 0;
    int counters = 0;
    int total() const { return buttons + watermelons + slices + counters; }
};

class SceneObject : public QGraphicsObject
{
    Q_OBJECT
public:
    enum class Kind { ExitButton, Watermelon, Slice, Counter };

    explicit SceneObject(Kind kind);
    Kind kind() const { return kind_; }

signals:
    void picked(SceneObject *item);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

    virtual void leftClick() {}
    virtual void rightClick() {}
    void paintHoverOutline(QPainter *painter) const;
    bool hovered() const { return hovered_; }
    bool pressed() const { return pressed_ && !dragged_; }
private:
    Kind kind_;
    bool hovered_ = false;
    bool pressed_ = false;
    bool dragged_ = false;
    QPoint pressScreenPosition_;
};

class ExitButtonItem final : public SceneObject
{
    Q_OBJECT
public:
    ExitButtonItem();
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *) override;

signals:
    void exitRequested();

protected:
    void leftClick() override;
};

class WatermelonItem final : public SceneObject
{
    Q_OBJECT
public:
    WatermelonItem();
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *) override;

signals:
    void cutRequested(WatermelonItem *item);

protected:
    void rightClick() override;
};

class WatermelonSliceItem final : public SceneObject
{
    Q_OBJECT
public:
    WatermelonSliceItem();
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *) override;

signals:
    void removeRequested(WatermelonSliceItem *item);
protected:
    void rightClick() override;
};
class CounterItem final : public SceneObject
{
    Q_OBJECT
public:
    CounterItem();
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *) override;
    void setCounts(const ObjectCounts &counts);
    ObjectCounts counts() const { return counts_; }

private:
    ObjectCounts counts_;
};
