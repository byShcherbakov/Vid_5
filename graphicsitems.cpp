#include "graphicsitems.h"
#include <QApplication>
#include <QFont>
#include <QGraphicsScene>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneMouseEvent>
#include <QLinearGradient>
#include <QPainter>
#include <QRadialGradient>
#include <QVariant>
#include <algorithm>
namespace {
QFont textFont(int pixels, bool bold = false)
{
    QFont font = QApplication::font();
    font.setPixelSize(pixels);
    font.setBold(bold);
    return font;
}
QPainterPath slicePath(qreal left, qreal right, qreal edgeY, qreal bottomY)
{
    QPainterPath path;
    path.moveTo(66, 7);
    path.lineTo(right, edgeY);
    path.quadTo(66, bottomY, left, edgeY);
    path.closeSubpath();
    return path;
}
}
SceneObject::SceneObject(Kind kind) : kind_(kind)
{
    setFlags(ItemIsMovable | ItemSendsGeometryChanges);
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);
    setCursor(Qt::OpenHandCursor);
}
void SceneObject::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::RightButton) {
        event->accept();
        rightClick();
        return;
    }
    if (event->button() != Qt::LeftButton) {
        event->ignore();
        return;
    }
    pressed_ = true;
    dragged_ = false;
    pressScreenPosition_ = event->screenPos();
    setCursor(Qt::ClosedHandCursor);
    emit picked(this);
    update();
    QGraphicsObject::mousePressEvent(event);
}
void SceneObject::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (!(event->buttons() & Qt::LeftButton) || !pressed_)
        return;
    if ((event->screenPos() - pressScreenPosition_).manhattanLength()
        >= QApplication::startDragDistance())
        dragged_ = true;
    if (dragged_) {
        QGraphicsObject::mouseMoveEvent(event);
        update();
    }
}
void SceneObject::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() != Qt::LeftButton) {
        event->accept();
        return;
    }
    const bool click = pressed_ && !dragged_
        && (event->screenPos() - pressScreenPosition_).manhattanLength()
            < QApplication::startDragDistance()
        && shape().contains(event->pos());
    pressed_ = false;
    dragged_ = false;
    setCursor(Qt::OpenHandCursor);
    QGraphicsObject::mouseReleaseEvent(event);
    update();
    if (click)
        leftClick();
}
void SceneObject::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    hovered_ = true;
    update();
    QGraphicsObject::hoverEnterEvent(event);
}
void SceneObject::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    hovered_ = false;
    update();
    QGraphicsObject::hoverLeaveEvent(event);
}
QVariant SceneObject::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionChange && scene()) {
        const QRectF area = scene()->sceneRect().adjusted(12, 12, -12, -12);
        const QRectF bounds = boundingRect();
        QPointF position = value.toPointF();
        const qreal maxX = std::max(area.left() - bounds.left(), area.right() - bounds.right());
        const qreal maxY = std::max(area.top() - bounds.top(), area.bottom() - bounds.bottom());
        position.setX(qBound(area.left() - bounds.left(), position.x(), maxX));
        position.setY(qBound(area.top() - bounds.top(), position.y(), maxY));
        return position;
    }
    return QGraphicsObject::itemChange(change, value);
}
void SceneObject::paintHoverOutline(QPainter *painter) const
{
    if (!hovered_)
        return;
    painter->save();
    painter->setBrush(Qt::NoBrush);
    painter->setPen(QPen(QColor(35, 67, 53, 180), 1.5, Qt::DashLine));
    painter->drawPath(shape());
    painter->restore();
}
ExitButtonItem::ExitButtonItem() : SceneObject(Kind::ExitButton)
{
    setToolTip(QStringLiteral("ЛКМ — выход; перетаскивание ЛКМ — перемещение"));
}
QRectF ExitButtonItem::boundingRect() const { return QRectF(0, 0, 156, 66); }
QPainterPath ExitButtonItem::shape() const
{
    QPainterPath path;
    path.addRoundedRect(QRectF(4, 4, 148, 52), 14, 14);
    return path;
}
void ExitButtonItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(86, 31, 39, 26));
    painter->drawRoundedRect(QRectF(4, 9, 148, 52), 14, 14);
    QLinearGradient fill(0, 4, 0, 56);
    fill.setColorAt(0, QColor(hovered() ? "#f66b79" : "#ef596d"));
    fill.setColorAt(1, QColor(pressed() ? "#b32d46" : "#d73c55"));
    painter->setBrush(fill);
    painter->drawPath(shape());
    painter->setFont(textFont(21, true));
    painter->setPen(Qt::white);
    painter->drawText(QRectF(4, 4, 148, 52), Qt::AlignCenter, QStringLiteral("Выход"));
}
void ExitButtonItem::leftClick() { emit exitRequested(); }
WatermelonItem::WatermelonItem() : SceneObject(Kind::Watermelon)
{
    setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);
    setToolTip(QStringLiteral("ЛКМ — перемещение; ПКМ — разрезать на 8 долек"));
}
QRectF WatermelonItem::boundingRect() const { return QRectF(0, 0, 168, 162); }
QPainterPath WatermelonItem::shape() const
{
    QPainterPath path;
    path.addEllipse(QRectF(7, 16, 154, 130));
    return path;
}
void WatermelonItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(34, 69, 41, 24));
    painter->drawEllipse(QRectF(19, 140, 132, 15));
    const QRectF body(7, 16, 154, 130);
    QRadialGradient green(QPointF(53, 45), 145);
    green.setColorAt(0, QColor("#a4ce62"));
    green.setColorAt(0.52, QColor("#62a448"));
    green.setColorAt(1, QColor("#245f38"));
    painter->setBrush(green);
    painter->drawEllipse(body);
    painter->save();
    painter->setClipPath(shape());
    painter->setBrush(QColor("#2b713b"));
    for (int i = -2; i <= 2; ++i) {
        const qreal shift = i * 33.0;
        QPainterPath stripe;
        stripe.moveTo(84 + shift * 0.58 - 3, 9);
        stripe.cubicTo(84 + shift * 1.22 - 15, 47,
                      84 + shift * 1.26 - 13, 110, 84 + shift * 0.6 - 4, 153);
        stripe.lineTo(84 + shift * 0.6 + 5, 153);
        stripe.cubicTo(84 + shift * 1.26 + 4, 110,
                      84 + shift * 1.22 + 4, 47, 84 + shift * 0.58 + 4, 9);
        stripe.closeSubpath();
        painter->drawPath(stripe);
    }
    painter->setBrush(QColor(255, 255, 220, 32));
    painter->drawEllipse(QRectF(30, 29, 51, 28));
    painter->restore();
    painter->setPen(QPen(QColor("#2e6334"), 2));
    painter->setBrush(Qt::NoBrush);
    painter->drawEllipse(body);
    QPainterPath stem;
    stem.moveTo(84, 18);
    stem.cubicTo(81, 3, 98, 1, 100, 9);
    painter->setPen(QPen(QColor("#64753b"), 4, Qt::SolidLine, Qt::RoundCap));
    painter->drawPath(stem);
    paintHoverOutline(painter);
}
void WatermelonItem::rightClick() { emit cutRequested(this); }
WatermelonSliceItem::WatermelonSliceItem() : SceneObject(Kind::Slice)
{
    setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);
    setToolTip(QStringLiteral("ЛКМ — перемещение; ПКМ — удалить дольку"));
}
QRectF WatermelonSliceItem::boundingRect() const { return QRectF(0, 0, 132, 120); }
QPainterPath WatermelonSliceItem::shape() const { return slicePath(7, 125, 82, 137); }
void WatermelonSliceItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(34, 69, 41, 22));
    painter->drawEllipse(QRectF(24, 104, 86, 11));
    painter->setBrush(QColor("#286d43"));
    painter->drawPath(shape());
    painter->setBrush(QColor("#d9edaa"));
    painter->drawPath(slicePath(16, 116, 77, 122));
    QLinearGradient flesh(66, 8, 66, 104);
    flesh.setColorAt(0, QColor("#ff7c7b"));
    flesh.setColorAt(1, QColor("#e94660"));
    painter->setBrush(flesh);
    painter->drawPath(slicePath(23, 109, 72, 109));
    const QPointF seeds[] = {{60, 37}, {76, 49}, {46, 56}, {61, 69},
                             {89, 70}, {39, 76}, {76, 85}};
    for (const QPointF &position : seeds) {
        painter->save();
        painter->translate(position);
        painter->rotate((position.x() - 66) * -0.65);
        QPainterPath seed;
        seed.moveTo(0, -5);
        seed.cubicTo(-5, 0, -4, 5, 0, 5);
        seed.cubicTo(4, 5, 5, 0, 0, -5);
        painter->setBrush(QColor("#4a3039"));
        painter->drawPath(seed);
        painter->setBrush(QColor(255, 255, 255, 100));
        painter->drawEllipse(QRectF(-1.8, 0, 1.3, 2.8));
        painter->restore();
    }
    paintHoverOutline(painter);
}
void WatermelonSliceItem::rightClick() { emit removeRequested(this); }
CounterItem::CounterItem() : SceneObject(Kind::Counter)
{
    setZValue(1000000000.0);
    setToolTip(QStringLiteral("Счётчик — отдельный объект; его тоже можно перемещать ЛКМ"));
}
QRectF CounterItem::boundingRect() const { return QRectF(0, 0, 330, 218); }
QPainterPath CounterItem::shape() const
{
    QPainterPath path;
    path.addRoundedRect(QRectF(2, 2, 324, 208), 18, 18);
    return path;
}
void CounterItem::setCounts(const ObjectCounts &counts)
{
    counts_ = counts;
    update();
}
void CounterItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setRenderHint(QPainter::Antialiasing);
    const bool ready = counts_.total() == 256;
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(30, 60, 45, 17));
    painter->drawRoundedRect(QRectF(2, 8, 324, 208), 18, 18);
    painter->setPen(QPen(QColor(ready ? "#7fb496" : "#dbe3d8"), 1));
    painter->setBrush(QColor("#ffffff"));
    painter->drawPath(shape());
    painter->setPen(QColor("#657669"));
    painter->setFont(textFont(11, true));
    painter->drawText(QRectF(20, 18, 290, 18), Qt::AlignLeft,
                      QStringLiteral("ОБЪЕКТОВ НА СЦЕНЕ"));
    painter->setFont(textFont(counts_.total() > 9999 ? 35 : 48, true));
    painter->setPen(QColor("#244a37"));
    painter->drawText(QRectF(18, 38, 209, 60), Qt::AlignLeft | Qt::AlignVCenter,
                      QString::number(counts_.total()));
    painter->setFont(textFont(22));
    painter->setPen(QColor("#93a194"));
    painter->drawText(QRectF(224, 53, 82, 34), Qt::AlignRight | Qt::AlignVCenter,
                      QStringLiteral("/ 256"));
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(ready ? "#e1f3e6" : "#f3f0e7"));
    painter->drawRoundedRect(QRectF(18, 105, 290, 25), 8, 8);
    painter->setPen(QColor(ready ? "#287345" : "#796b4e"));
    painter->setFont(textFont(12, true));
    painter->drawText(QRectF(18, 105, 290, 25), Qt::AlignCenter,
                      ready ? QStringLiteral("Можно выйти из программы")
                            : QStringLiteral("Для выхода нужно ровно 256"));

    const QString labels[] = {QStringLiteral("Арбузы"), QStringLiteral("Дольки"),
                              QStringLiteral("Кнопки")};
    const int values[] = {counts_.watermelons, counts_.slices, counts_.buttons};
    for (int i = 0; i < 3; ++i) {
        painter->setPen(QColor("#718075"));
        painter->setFont(textFont(11));
        painter->drawText(QRectF(20 + 98 * i, 140, 94, 18), Qt::AlignLeft, labels[i]);
        painter->setPen(QColor("#294c3a"));
        painter->setFont(textFont(17, true));
        painter->drawText(QRectF(20 + 98 * i, 159, 94, 22), Qt::AlignLeft,
                          QString::number(values[i]));
    }
    painter->setPen(QColor("#879487"));
    painter->setFont(textFont(10));
    painter->drawText(QRectF(20, 187, 290, 17), Qt::AlignLeft,
                      QStringLiteral("+ 1 счётчик, включённый в общее число"));
}
