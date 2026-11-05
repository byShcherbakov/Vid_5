#include "mainwindow.h"
#include "watermelonscene.h"

#include <QApplication>
#include <QGraphicsView>
#include <QImage>
#include <QLineF>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <QPointer>
#include <QPushButton>
#include <QSignalSpy>
#include <QSpinBox>
#include <QTest>
#include <QTimer>
#include <QVector>

namespace {
template<class T>
T *findItem(WatermelonScene *scene)
{
    for (QGraphicsItem *item : scene->items())
        if (auto *result = dynamic_cast<T *>(item))
            return result;
    return nullptr;
}

void showWindow(MainWindow &window)
{
    window.show();
    QApplication::processEvents();
}

void sendMouse(QWidget *widget, QEvent::Type type, const QPoint &position,
               Qt::MouseButton button, Qt::MouseButtons buttons)
{
    const QPoint global = widget->mapToGlobal(position);
    QMouseEvent event(type, QPointF(position), QPointF(global), button, buttons, Qt::NoModifier);
    QApplication::sendEvent(widget, &event);
    QApplication::processEvents();
}

void dragItem(MainWindow &window, SceneObject *item, const QPointF &localPoint,
              const QPoint &delta)
{
    auto *view = window.graphicsView();
    auto *viewport = view->viewport();
    const QPoint start = view->mapFromScene(item->mapToScene(localPoint));
    sendMouse(viewport, QEvent::MouseButtonPress, start, Qt::LeftButton, Qt::LeftButton);
    sendMouse(viewport, QEvent::MouseMove, start + delta / 2, Qt::NoButton, Qt::LeftButton);
    sendMouse(viewport, QEvent::MouseMove, start + delta, Qt::NoButton, Qt::LeftButton);
    sendMouse(viewport, QEvent::MouseButtonRelease, start + delta, Qt::LeftButton, Qt::NoButton);
}

void dismissMessage(MainWindow &window, QString *text)
{
    QTimer::singleShot(0, &window, [text] {
        for (QWidget *widget : QApplication::topLevelWidgets()) {
            if (auto *message = qobject_cast<QMessageBox *>(widget)) {
                *text = message->text();
                message->accept();
            }
        }
    });
}
} // namespace

class SceneTests final : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase()
    {
        QApplication::setQuitOnLastWindowClosed(false);
    }

    void initialCounts()
    {
        WatermelonScene scene;
        QCOMPARE(scene.objectCount(), 3);
        QCOMPARE(scene.counts().buttons, 1);
        QCOMPARE(scene.counts().watermelons, 1);
        QCOMPARE(scene.counts().slices, 0);
        QCOMPARE(scene.counts().counters, 1);
        QCOMPARE(scene.counterItem()->counts().total(), scene.objectCount());
    }

    void unlimitedAdditionAndClear()
    {
        WatermelonScene scene;
        scene.clearObjects();
        QCOMPARE(scene.objectCount(), 1);
        scene.addWatermelons(10);
        scene.addExitButtons(270);
        QCOMPARE(scene.objectCount(), 281);
        QCOMPARE(scene.counts().watermelons, 10);
        QCOMPARE(scene.counts().buttons, 270);
        QCOMPARE(scene.counterItem()->counts().total(), 281);
        scene.addWatermelons(0);
        scene.addExitButtons(-1);
        QCOMPARE(scene.objectCount(), 281);
        scene.clearObjects();
        QCOMPARE(scene.items().size(), 1);
        QVERIFY(scene.items().contains(scene.counterItem()));
        QCOMPARE(scene.counterItem()->counts().total(), 1);
    }

    void toolbarUsesQuantity()
    {
        MainWindow window;
        showWindow(window);
        auto *quantity = window.findChild<QSpinBox *>(QStringLiteral("quantity"));
        auto *addWatermelon = window.findChild<QPushButton *>(QStringLiteral("addWatermelon"));
        auto *addButton = window.findChild<QPushButton *>(QStringLiteral("addExitButton"));
        QVERIFY(quantity && addWatermelon && addButton);
        quantity->setValue(5);
        QTest::mouseClick(addWatermelon, Qt::LeftButton);
        QCOMPARE(window.graphicsScene()->counts().watermelons, 6);
        QTest::mouseClick(addButton, Qt::LeftButton);
        QCOMPARE(window.graphicsScene()->counts().buttons, 6);
        QCOMPARE(window.graphicsScene()->objectCount(), 13);
    }

    void rightClickSplitsIntoEight()
    {
        MainWindow window;
        showWindow(window);
        auto *scene = window.graphicsScene();
        QPointer<WatermelonItem> watermelon = findItem<WatermelonItem>(scene);
        QVERIFY(watermelon);
        auto *view = window.graphicsView();
        const QPoint point = view->mapFromScene(watermelon->mapToScene(QPointF(84, 80)));
        QTest::mouseClick(view->viewport(), Qt::RightButton, Qt::NoModifier, point);
        QCOMPARE(scene->counts().watermelons, 0);
        QCOMPARE(scene->counts().slices, 8);
        QCOMPARE(scene->objectCount(), 10);
        QCOMPARE(scene->counterItem()->counts().total(), 10);

        QVector<QPointF> positions;
        for (QGraphicsItem *item : scene->items()) {
            if (auto *slice = dynamic_cast<WatermelonSliceItem *>(item)) {
                QVERIFY(scene->sceneRect().contains(slice->sceneBoundingRect()));
                QVERIFY(!positions.contains(slice->pos()));
                positions.append(slice->pos());
            }
        }
        QCOMPARE(positions.size(), 8);
        QCoreApplication::sendPostedEvents(nullptr, QEvent::DeferredDelete);
        QVERIFY(watermelon.isNull());
    }

    void rightClickDeletesOnlyTheSlice()
    {
        MainWindow window;
        showWindow(window);
        auto *scene = window.graphicsScene();
        scene->cutWatermelon(findItem<WatermelonItem>(scene));
        QPointer<WatermelonSliceItem> slice = findItem<WatermelonSliceItem>(scene);
        QVERIFY(slice);
        slice->setPos(150, 440);
        slice->setZValue(100000);
        auto *view = window.graphicsView();
        const QPoint point = view->mapFromScene(slice->mapToScene(QPointF(66, 60)));
        QTest::mouseClick(view->viewport(), Qt::RightButton, Qt::NoModifier, point);
        QCOMPARE(scene->objectCount(), 9);
        QCOMPARE(scene->counts().slices, 7);
        QCOMPARE(scene->counts().buttons, 1);
        QCoreApplication::sendPostedEvents(nullptr, QEvent::DeferredDelete);
        QVERIFY(slice.isNull());
    }

    void dragging_data()
    {
        QTest::addColumn<int>("kind");
        QTest::newRow("exit-button") << 0;
        QTest::newRow("whole-watermelon") << 1;
        QTest::newRow("slice") << 2;
        QTest::newRow("counter") << 3;
    }

    void dragging()
    {
        QFETCH(int, kind);
        MainWindow window;
        showWindow(window);
        auto *scene = window.graphicsScene();
        SceneObject *item = nullptr;
        QPointF localPoint;
        switch (kind) {
        case 0: item = findItem<ExitButtonItem>(scene); localPoint = QPointF(78, 30); break;
        case 1: item = findItem<WatermelonItem>(scene); localPoint = QPointF(84, 80); break;
        case 2:
            scene->cutWatermelon(findItem<WatermelonItem>(scene));
            item = findItem<WatermelonSliceItem>(scene);
            localPoint = QPointF(66, 60);
            break;
        case 3: item = scene->counterItem(); localPoint = QPointF(150, 60); break;
        }
        QVERIFY(item);
        item->setPos(160, 280);
        item->setZValue(2000000000.0);
        const QPointF before = item->pos();
        const int count = scene->objectCount();
        QSignalSpy exitSpy(scene, &WatermelonScene::exitRequested);
        dragItem(window, item, localPoint, QPoint(100, 65));
        QVERIFY(QLineF(before, item->pos()).length() > 30);
        QCOMPARE(scene->objectCount(), count);
        QCOMPARE(exitSpy.count(), 0);
        QVERIFY(window.isVisible());

        // Programmatic and mouse movement both stay inside the scene.
        item->setPos(-10000, -10000);
        QVERIFY(scene->sceneRect().contains(item->sceneBoundingRect()));
        item->setPos(10000, 10000);
        QVERIFY(scene->sceneRect().contains(item->sceneBoundingRect()));
    }

    void closeBoundary_data()
    {
        QTest::addColumn<int>("count");
        QTest::addColumn<bool>("allowed");
        QTest::newRow("255-refused") << 255 << false;
        QTest::newRow("256-accepted") << 256 << true;
        QTest::newRow("257-refused") << 257 << false;
    }

    void closeBoundary()
    {
        QFETCH(int, count);
        QFETCH(bool, allowed);
        MainWindow window;
        auto *scene = window.graphicsScene();
        scene->clearObjects();
        scene->addExitButtons(count - 1);
        QCOMPARE(scene->objectCount(), count);
        showWindow(window);
        QString message;
        if (!allowed)
            dismissMessage(window, &message);
        QCOMPARE(window.close(), allowed);
        QCOMPARE(window.isVisible(), !allowed);
        if (!allowed)
            QCOMPARE(message, QStringLiteral("Отсюда не сбежать!"));
    }

    void exitButtonUsesTheSameCloseCheck_data()
    {
        QTest::addColumn<int>("count");
        QTest::newRow("button-at-3") << 3;
        QTest::newRow("button-at-256") << 256;
    }

    void exitButtonUsesTheSameCloseCheck()
    {
        QFETCH(int, count);
        MainWindow window;
        auto *scene = window.graphicsScene();
        if (count > 3)
            scene->addExitButtons(count - 3);
        auto *button = findItem<ExitButtonItem>(scene);
        QVERIFY(button);
        button->setPos(450, 420);
        button->setZValue(100000);
        showWindow(window);
        QString message;
        if (count != 256)
            dismissMessage(window, &message);
        const QPoint point = window.graphicsView()->mapFromScene(button->mapToScene(QPointF(78, 30)));
        QTest::mouseClick(window.graphicsView()->viewport(), Qt::LeftButton, Qt::NoModifier, point);
        QCOMPARE(window.isVisible(), count != 256);
        if (count != 256)
            QCOMPARE(message, QStringLiteral("Отсюда не сбежать!"));
    }

    void repeatedCutsAndRemovals()
    {
        WatermelonScene scene;
        scene.clearObjects();
        scene.addWatermelons(40);
        while (auto *item = findItem<WatermelonItem>(&scene))
            scene.cutWatermelon(item);
        QCOMPARE(scene.counts().slices, 320);
        QCOMPARE(scene.objectCount(), 321);
        while (auto *item = findItem<WatermelonSliceItem>(&scene))
            scene.removeSlice(item);
        QCoreApplication::sendPostedEvents(nullptr, QEvent::DeferredDelete);
        QCOMPARE(scene.objectCount(), 1);
        QCOMPARE(scene.counterItem()->counts().total(), 1);
    }

    void renderPreview()
    {
        const QString output = qEnvironmentVariable("LAB5_SCREENSHOT_PATH");
        if (output.isEmpty())
            QSKIP("Set LAB5_SCREENSHOT_PATH to save a preview.");
        MainWindow window;
        auto *scene = window.graphicsScene();
        scene->addWatermelons(1);
        scene->cutWatermelon(findItem<WatermelonItem>(scene));
        const QPointF positions[] = {{110, 65}, {405, 145}, {660, 225}, {1000, 300},
                                     {190, 550}, {460, 560}, {750, 510}, {1045, 555}};
        int index = 0;
        for (QGraphicsItem *item : scene->items()) {
            if (auto *slice = dynamic_cast<WatermelonSliceItem *>(item))
                slice->setPos(positions[index++]);
        }
        findItem<WatermelonItem>(scene)->setPos(180, 300);
        findItem<ExitButtonItem>(scene)->setPos(485, 350);
        showWindow(window);
        QImage image(window.size(), QImage::Format_ARGB32_Premultiplied);
        image.fill(Qt::transparent);
        QPainter painter(&image);
        window.render(&painter);
        painter.end();
        QVERIFY(image.save(output));
        QCOMPARE(index, 8);
    }
};

QTEST_MAIN(SceneTests)
#include "tst_scene.moc"
