#pragma once
#include <QMainWindow>
class QCloseEvent;
class QGraphicsView;
class QSpinBox;
class WatermelonScene;
class MainWindow final : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    WatermelonScene *graphicsScene() const { return scene_; }
    QGraphicsView *graphicsView() const { return view_; }
protected:
    void closeEvent(QCloseEvent *event) override;
private:
    WatermelonScene *scene_ = nullptr;
    QGraphicsView *view_ = nullptr;
    QSpinBox *quantity_ = nullptr;
};
