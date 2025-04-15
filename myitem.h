#ifndef MYITEM_H
#define MYITEM_H
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

class MyItem: public QObject, public QGraphicsPixmapItem{
    Q_OBJECT
public:
    enum KIND{
        KNIFE, SPEED, HP
    };
private:
    enum KIND kind;
public:
    MyItem(enum KIND k, QGraphicsItem* parent = nullptr);
    ~MyItem();
    enum KIND getKind(){return kind;}
    QRectF boundingRect() const override {
        // 获取原始 pixmap 的边界
        QRectF baseRect = QGraphicsPixmapItem::boundingRect();
        return baseRect;
    }
};
#endif
