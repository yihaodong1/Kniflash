#ifndef ROLE_H
#define ROLE_H
#include <QDebug>
#include <QGraphicsRectItem>
#include <QGraphicsPixmapItem>
#include <QGraphicsView>
#include <QKeyEvent>
#include <QMovie>
#include <QTimer>
#include <QColor>
#include <QList>
#include <QPainter>

class Role: public QObject, public QGraphicsPixmapItem{
  Q_OBJECT
public:
    explicit Role(QGraphicsItem* parent = nullptr);
    ~Role();
    void updateKnivesRotation();
// public slots:
//     void updateHP();
private:
    QTimer *m_rotateTimer;
    int knife_num;// 周围的飞刀数
    int hp = 100;// health point血量值
    double angle;
    double rotateSpeed;
    QGraphicsRectItem *line;// 血条
    QList<QGraphicsPixmapItem*> knives;
    QMovie *m_movie;
};
#endif
