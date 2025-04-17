#ifndef CHARACTER_H
#define CHARACTER_H
#include <QDebug>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsPixmapItem>
#include <QGraphicsView>
#include <QKeyEvent>
#include <QMovie>
#include <QTimer>
#include <QColor>
#include <QList>
#include <QPainter>
#include <QGraphicsPathItem>
#include <QPainterPath>
#include "myitem.h"
class Character: public QObject, public QGraphicsPixmapItem{
    Q_OBJECT
protected:
    QTimer m_speedUpTimer;
    int speedUpCount = 0;
    QGraphicsPathItem *path;

    QTimer m_rotateTimer;
    QTimer m_knifeaddTimer;
    int knife_num;// 周围的飞刀数
    int hp = 100;// health point血量值
    double step = 10;// 运动速度
    double angle;// 旋转角度
    double rotateSpeed;// 旋转速度
    QGraphicsRectItem *line;// 血条
    QList<QGraphicsPixmapItem*> knives;
public:
    explicit Character(QGraphicsItem* parent = nullptr);
    ~Character();
    void updateKnivesRotation();
    void updateKnifenum();
    double getStep(){return step;}
    QRectF boundingRect() const override {
        // 获取原始 pixmap 的边界
        QRectF baseRect = QGraphicsPixmapItem::boundingRect();
        return baseRect;
    }
    void knifeNumAdder();
    void updateSpeedUpCount();
    void speedUp(){
        step = step / 2 * 3;
        m_speedUpTimer.start(1000);
        speedUpCount = 5;
    }
    void hpAdder(){hp = (hp + 5) >= 100? 100: hp + 5;}
// public slots:
//     void updateHP();
};
#endif

