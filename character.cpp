#include "character.h"
Character::Character(QGraphicsItem* parent): QGraphicsPixmapItem(parent){
    hp = 100;
    line = new QGraphicsRectItem(20,-10, 100, 10, this);
    line->setBrush(Qt::red);

    knife_num = 0;
    for(int i = 0; i < 4; i++){
        knifeNumAdder();
    }
    angle = rand()%360;
    rotateSpeed = 1;
    connect(&m_rotateTimer, &QTimer::timeout, this, &Character::updateKnivesRotation);
    m_rotateTimer.start(16);
    connect(&m_knifeaddTimer, &QTimer::timeout, this, &Character::updateKnifenum);
    m_knifeaddTimer.start(1000);

    connect(&m_speedUpTimer, &QTimer::timeout, this, &Character::updateSpeedUpCount);

    const int speed_radius = 50;
    path = new QGraphicsPathItem(this);
    path->setPen(QPen(Qt::green, 6));
    QPainterPath painter;
    painter.moveTo(speed_radius, 0);
    painter.arcTo(0, 0, 2 * speed_radius, 2 * speed_radius, 90, 360);
    path->setPath(painter);
    path->setVisible(false);
    new MyItem(MyItem::SPEED, path);
}

Character::~Character(){

}

void Character::updateSpeedUpCount(){
    speedUpCount--;
    const int speed_radius = 50;
    QPainterPath painter;
    painter.moveTo(speed_radius, 0);
    painter.arcTo(0, 0, 2 * speed_radius, 2 * speed_radius, 90, 72 * speedUpCount);
    path->setPath(painter);
    path->setPos(this->boundingRect().width() / 2 - speed_radius, this->boundingRect().height());
    path->setVisible(true);

    if(speedUpCount == 0){
        m_speedUpTimer.stop();
        step = 10;
        path->setVisible(false);
    }else if(speedUpCount < 0){
        qDebug()<<"error";
    }
}

void Character::updateKnivesRotation(){
    angle = knife_num != 0? angle+(rotateSpeed/knife_num):angle;// NOTE: avoid to divide zero!!!!
    angle = angle>360?angle-360:angle;
    for(int i = 0; i < knife_num; i++){
        knives[i]->setTransformOriginPoint(knives[i]->boundingRect().center());
        knives[i]->setRotation(i * 360 / knife_num + angle);
        knives[i]->setPos(60*(1+(double)knife_num / 8) * cos(2 * M_PI * ((double)i / knife_num + double(angle)/360)), 
            60 * (1 + (double)knife_num / 8) * sin(2 * M_PI * ((double)i / knife_num + (double)angle/360)));
    }

}

void Character::knifeNumAdder(){
    QPixmap knifePixmap(":/figs/knife.png");
    if (knifePixmap.isNull()) {
        // 时常使用 qDebug 输出调试信息, 是好的开发习惯
        qDebug() << "Failed to load knife image!";
    }
    knifePixmap = knifePixmap.scaled(123, 100);
    QGraphicsPixmapItem *knifePixmapItem = new QGraphicsPixmapItem(knifePixmap, this);
    knifePixmapItem->setPos(rand()%100, rand()%100);
    knives.push_back(knifePixmapItem);
    knife_num++;
}

void Character::updateKnifenum(){
    if(knife_num<4){
        knifeNumAdder();
    }
}
