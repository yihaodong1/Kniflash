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
#include <QGraphicsItem>
#include <QPainter>

class HealthBar : public QGraphicsItem {
public:
    HealthBar(qreal width = 100, qreal height = 10, QGraphicsItem* parent = nullptr)
        : QGraphicsItem(parent), m_width(width), m_height(height),
          m_currentHealth(100), m_maxHealth(100) {}

    QRectF boundingRect() const override {
        return QRectF(0, 0, m_width, m_height);
    }

    void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) override {
        // 绘制背景（灰色）
        painter->setBrush(Qt::gray);
        painter->drawRect(0, 0, m_width, m_height);

        // 计算当前血量比例
        qreal healthRatio = qBound(0.0, m_currentHealth / m_maxHealth, 1.0);

        // 绘制当前血量（绿色）
        painter->setBrush(Qt::green);
        painter->drawRect(0, 0, m_width * healthRatio, m_height);
    }

    qreal getHealth(){return m_currentHealth;}
    void setHealth(qreal health) {
        m_currentHealth = qBound(0.0, health, m_maxHealth);
        update(); // 触发重绘
    }

    void setMaxHealth(qreal maxHealth) {
        m_maxHealth = maxHealth;
        if (m_currentHealth > m_maxHealth) {
            m_currentHealth = m_maxHealth;
        }
        update();
    }

private:
    qreal m_width;
    qreal m_height;
    qreal m_currentHealth;
    qreal m_maxHealth;
};

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
    // QGraphicsRectItem *line;// 血条
    HealthBar *line;
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
    void hpAdder(){line->setHealth(line->getHealth()+5);}
    int getKnifeNum(){return knife_num;}
    void useKnife(){knife_num--;delete knives[0]; knives.pop_front();}
    void bleed(){line->setHealth(line->getHealth()-25);}
// public slots:
//     void updateHP();
};
#endif

