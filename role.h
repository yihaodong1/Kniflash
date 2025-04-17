#ifndef ROLE_H
#define ROLE_H
#include "character.h"
enum{
    STATIC, MOVING
};
class Role: public Character{
private:
    int status;// 0 is static, 1 is moving
    QGraphicsEllipseItem *cloud;// 跑动特效
    QMovie *m_movie;
public:
    explicit Role(QGraphicsItem* parent = nullptr);
    ~Role();
    void setStatus(int s){status = s; s==MOVING?cloud->setVisible(true):cloud->setVisible(false);}
// public slots:
//     void updateHP();
};
#endif
