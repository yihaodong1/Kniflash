#ifndef ROLE_H
#define ROLE_H
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

enum{
    STATIC, MOVING
};
class Role: public QObject, public QGraphicsPixmapItem{
    Q_OBJECT
private:
    QTimer *m_rotateTimer;
    QTimer *m_knifeaddTimer;
    QTimer *m_speedUpTimer;
    int speedUpCount = 0;
    int status;// 0 is static, 1 is moving
    int knife_num;// 周围的飞刀数
    int hp = 100;// health point血量值
    double step = 10;// 运动速度
    double angle;// 旋转角度
    double rotateSpeed;// 旋转速度
    QGraphicsRectItem *line;// 血条
    QGraphicsEllipseItem *cloud;// 跑动特效
    QList<QGraphicsPixmapItem*> knives;
    QMovie *m_movie;
    QGraphicsPathItem *path;
public:
    explicit Role(QGraphicsItem* parent = nullptr);
    ~Role();
    void updateKnivesRotation();
    void updateKnifenum();
    void updateSpeedUpCount();
    void setStatus(int s){status = s; s==MOVING?cloud->setVisible(true):cloud->setVisible(false);}
    double getStep(){return step;}
    QRectF boundingRect() const override {
        // 获取原始 pixmap 的边界
        QRectF baseRect = QGraphicsPixmapItem::boundingRect();
        return baseRect;
    }
    void knifeNumAdder();
    void speedUp(){
        step = 15;
        m_speedUpTimer->start(1000);
        speedUpCount = 5;
        path->setVisible(true);
    }
    void hpAdder(){hp = (hp + 5) >= 100? 100: hp + 5;}
// public slots:
//     void updateHP();
};
#endif
