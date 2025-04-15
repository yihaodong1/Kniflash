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

enum{
    STATIC, MOVING
};
class Role: public QObject, public QGraphicsPixmapItem{
    Q_OBJECT
private:
    QTimer *m_rotateTimer;
    QTimer *m_knifeaddTimer;
    int status;// 0 is static, 1 is moving
    int knife_num;// 周围的飞刀数
    int hp = 100;// health point血量值
    double step = 10;
    double angle;
    double rotateSpeed;
    QGraphicsRectItem *line;// 血条
    QGraphicsEllipseItem *cloud;
    QList<QGraphicsPixmapItem*> knives;
    QMovie *m_movie;
public:
    explicit Role(QGraphicsItem* parent = nullptr);
    ~Role();
    void updateKnivesRotation();
    void updateKnifenum();
    void setStatus(int s){status = s; s==MOVING?cloud->setVisible(true):cloud->setVisible(false);}
    double getStep(){return step;}
    QRectF boundingRect() const override {
        // 获取原始 pixmap 的边界
        QRectF baseRect = QGraphicsPixmapItem::boundingRect();
        return baseRect;
    }
    void knifeNumAdder();
// public slots:
//     void updateHP();
};
#endif
