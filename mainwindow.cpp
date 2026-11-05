#include "mainwindow.h"
#include "watermelonscene.h"

#include <QCloseEvent>
#include <QFrame>
#include <QGraphicsView>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QPainter>
#include <QPushButton>
#include <QResizeEvent>
#include <QSpinBox>
#include <QStatusBar>
#include <QVBoxLayout>

namespace {
class SceneView final : public QGraphicsView
{
public:
    explicit SceneView(QGraphicsScene *scene, QWidget *parent = nullptr)
        : QGraphicsView(scene, parent)
    {
        setObjectName(QStringLiteral("sceneView"));
        setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
        setFrameShape(QFrame::NoFrame);
        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setAlignment(Qt::AlignCenter);
    }

protected:
    void resizeEvent(QResizeEvent *event) override
    {
        QGraphicsView::resizeEvent(event);
        fitInView(scene()->sceneRect().adjusted(-8, -8, 8, 8), Qt::KeepAspectRatio);
    }
};
} // namespace

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle(QStringLiteral("Практическая работа 5 · Вариант 22 · Арбузная сцена"));
    resize(1200, 900);
    setMinimumSize(880, 660);
    setStyleSheet(QStringLiteral(R"(
        QMainWindow, QWidget#central { background: #fcfcf8; color: #244a37; }
        QLabel { color: #244a37; }
        QLabel#eyebrow { color: #829081; font-size: 11px; font-weight: bold; }
        QLabel#title { color: #244a37; font-size: 28px; font-weight: bold; }
        QLabel#subtitle { color: #7b897c; font-size: 12px; }
        QLabel#help { color: #6e7c70; font-size: 12px; }
        QFrame#controls { background: #eef2e9; border-radius: 12px; }
        QPushButton { background: white; color: #31543d; border: 1px solid #d7e0d2;
                      border-radius: 8px; padding: 10px 16px; font-size: 13px; }
        QPushButton:hover { background: #f6faf2; border-color: #95b78f; }
        QPushButton:pressed { background: #dce9d6; }
        QPushButton#addWatermelon { background: #376c48; color: white; border-color: #376c48; }
        QPushButton#addWatermelon:hover { background: #437e55; }
        QPushButton#addWatermelon:pressed { background: #285437; }
        QPushButton#clearScene { color: #98716a; background: transparent; border-color: transparent; }
        QSpinBox { color: #31543d; background: white; border: 1px solid #d7e0d2;
                   border-radius: 7px; padding: 7px; min-width: 65px; font-size: 13px; }
        QStatusBar { background: #fcfcf8; color: #738071; font-size: 11px; }
    )"));

    auto *central = new QWidget(this);
    central->setObjectName(QStringLiteral("central"));
    auto *layout = new QVBoxLayout(central);
    layout->setContentsMargins(24, 20, 24, 10);
    layout->setSpacing(11);

    auto *eyebrow = new QLabel(QStringLiteral("ПРАКТИЧЕСКАЯ РАБОТА 05  /  ВАРИАНТ 22"), central);
    eyebrow->setObjectName(QStringLiteral("eyebrow"));
    layout->addWidget(eyebrow);
    auto *title = new QLabel(QStringLiteral("Арбузная сцена"), central);
    title->setObjectName(QStringLiteral("title"));
    layout->addWidget(title);
    auto *subtitle = new QLabel(QStringLiteral("Добавляйте объекты, перемещайте их и соберите ровно 256 для выхода."), central);
    subtitle->setObjectName(QStringLiteral("subtitle"));
    layout->addWidget(subtitle);

    auto *controls = new QFrame(central);
    controls->setObjectName(QStringLiteral("controls"));
    auto *row = new QHBoxLayout(controls);
    row->setContentsMargins(12, 10, 12, 10);
    row->setSpacing(9);
    auto *addWatermelon = new QPushButton(QStringLiteral("+ Добавить арбуз"), controls);
    addWatermelon->setObjectName(QStringLiteral("addWatermelon"));
    auto *addButton = new QPushButton(QStringLiteral("+ Кнопка «Выход»"), controls);
    addButton->setObjectName(QStringLiteral("addExitButton"));
    auto *quantityLabel = new QLabel(QStringLiteral("Количество:"), controls);
    quantity_ = new QSpinBox(controls);
    quantity_->setObjectName(QStringLiteral("quantity"));
    quantity_->setRange(1, 9999);
    quantity_->setValue(1);
    quantity_->setToolTip(QStringLiteral("Число объектов за одно добавление. Добавлять можно многократно."));
    quantityLabel->setBuddy(quantity_);
    auto *clear = new QPushButton(QStringLiteral("Очистить"), controls);
    clear->setObjectName(QStringLiteral("clearScene"));
    clear->setToolTip(QStringLiteral("Удалить арбузы, дольки и кнопки. Останется 1 объект — счётчик."));
    row->addWidget(addWatermelon);
    row->addWidget(addButton);
    row->addSpacing(9);
    row->addWidget(quantityLabel);
    row->addWidget(quantity_);
    row->addStretch();
    row->addWidget(clear);
    layout->addWidget(controls);

    auto *help = new QLabel(QStringLiteral("ЛКМ + движение — перетаскивание    ·    ПКМ по арбузу — 8 долек    ·    ПКМ по дольке — удалить"), central);
    help->setObjectName(QStringLiteral("help"));
    help->setWordWrap(true);
    layout->addWidget(help);

    scene_ = new WatermelonScene(this);
    view_ = new SceneView(scene_, central);
    layout->addWidget(view_, 1);
    setCentralWidget(central);

    connect(addWatermelon, &QPushButton::clicked, this, [this] {
        scene_->addWatermelons(quantity_->value());
    });
    connect(addButton, &QPushButton::clicked, this, [this] {
        scene_->addExitButtons(quantity_->value());
    });
    connect(clear, &QPushButton::clicked, scene_, &WatermelonScene::clearObjects);
    // Every Exit item uses close(), so it cannot bypass closeEvent().
    connect(scene_, &WatermelonScene::exitRequested, this, [this] { close(); });
    const auto showCounts = [this](int total, int buttons, int watermelons, int slices) {
        statusBar()->showMessage(QStringLiteral("Всего: %1  |  Арбузы: %2  |  Дольки: %3  |  Кнопки: %4  |  Счётчик: 1")
            .arg(total).arg(watermelons).arg(slices).arg(buttons));
    };
    connect(scene_, &WatermelonScene::countsChanged, this, showCounts);
    const ObjectCounts counts = scene_->counts();
    showCounts(scene_->objectCount(), counts.buttons, counts.watermelons, counts.slices);
    statusBar()->setSizeGripEnabled(false);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (scene_->objectCount() == 256) {
        event->accept();
        return;
    }
    event->ignore();
    QMessageBox::information(this, QStringLiteral("Выход запрещён"),
                             QStringLiteral("Отсюда не сбежать!"));
}
