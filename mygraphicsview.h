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
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsLineItem>
#include <QMediaPlayer>
#include <QAudioOutput>
#include "role.h"
#include "myitem.h"
#include "npc.h"

enum MUSIC_TYPE { BGM, CLASH, DEATH, WIN, PICKUP };// 音乐类型
class MyGraphicsView : public QGraphicsView {
    Q_OBJECT
private:
    void playMusic(enum MUSIC_TYPE type);
    void freshItem(Character *c);
    QList<Npc*> npcs;// 人机
    QList<MyItem*> items;// 道具
    std::vector<QGraphicsPixmapItem*> bushes;            // 草丛
    Role *m_role;// 角色
    QGraphicsPixmapItem *m_background;      // 背景图像项
    QMovie *m_movie;                        // 用于播放 GIF

    QTimer m_update;// 更新游戏总体逻辑
    QTimer m_closeAttackTimer;// 每秒近战最多消耗5把飞刀
    QTimer m_npcRangedAttackTimer;// 人机每秒向锁定的角色发射飞刀

    QGraphicsLineItem* trace;
    int totalseconds = 0;
private:
    bool w_pressed = false;
    bool a_pressed = false;
    bool s_pressed = false;
    bool d_pressed = false;

public slots:
    // 处理信号, 展示游戏窗口
    // 这里对应步骤(2)

    // 更新运动圆形的路径
    // void updateCirclePath();

public:
    explicit MyGraphicsView(QGraphicsScene *scene, QWidget *parent = nullptr);
    void updateGame();
    void npcRangedAttack();
    void checkCloseAttack();
    Npc *findNear();

protected:
    // 键盘事件, 用来移动背景
    // 注意 override
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
signals:
    void showBtnToMenu();
};

#endif  // MYGRAPHICSVIEW_H
