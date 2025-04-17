#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H

#include <QDebug>
#include <QGraphicsEllipseItem>
#include <QGraphicsPixmapItem>
#include <QGraphicsView>
#include <QKeyEvent>
#include <QMovie>
#include <QTimer>
#include <QList>
#include "role.h"
#include "myitem.h"
#include "npc.h"

class MyGraphicsView : public QGraphicsView {
private:
    void freshItem(Character *c);
    QList<Npc*> npcs;
    QList<MyItem*> items;
    std::vector<QGraphicsPixmapItem*> bushes;            // 草丛
    Role *m_role;
    QGraphicsPixmapItem *m_background;      // 背景图像项
    QMovie *m_movie;                        // 用于播放 GIF

    QTimer m_update;
    const int SQUARE_SIZE = 500;  // 运动轨迹正方形边长
    int currentEdge = 0;          // 当前所在轨迹边（0-3）
    QPointF initPoint;
private:
    bool w_pressed = false;
    bool a_pressed = false;
    bool s_pressed = false;
    bool d_pressed = false;

public slots:
    // 处理信号, 展示游戏窗口
    // 这里对应步骤(2)
    void handleEvokeGameSignal();

    // 更新运动圆形的路径
    void updateCirclePath();

public:
    explicit MyGraphicsView(QGraphicsScene *scene, QWidget *parent = nullptr);
    void updateGame();
    void updateNpc();

protected:
    // 键盘事件, 用来移动背景
    // 注意 override
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
};

#endif  // MYGRAPHICSVIEW_H
