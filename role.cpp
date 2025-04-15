#include "role.h"
Role::Role(QGraphicsItem* parent): QGraphicsPixmapItem(parent){
    m_movie = new QMovie(":/figs/capoo.gif");
    if (!m_movie->isValid()) {
        qDebug() << "Failed to load GIF!";
    }
    m_movie->setScaledSize(QSize(160, 135));
    connect(m_movie, &QMovie::frameChanged, this, 
        [this]() { setPixmap(m_movie->currentPixmap()); 
            setZValue(1);});
    m_movie->start();

    hp = 100;
    line = new QGraphicsRectItem(20,-10, 100, 10, this);
    line->setBrush(Qt::red);

    knife_num = 4;
    for(int i = 0; i < knife_num; i++){
        QPixmap knifePixmap(":/figs/knife.png");
        if (knifePixmap.isNull()) {
            // 时常使用 qDebug 输出调试信息, 是好的开发习惯
            qDebug() << "Failed to load bush image!";
        }
        knifePixmap = knifePixmap.scaled(123, 100);
        QGraphicsPixmapItem *knifePixmapItem = new QGraphicsPixmapItem(knifePixmap, this);
        knives.append(knifePixmapItem);
    }
    angle = 0;
    rotateSpeed = 1;
    m_rotateTimer = new QTimer;
    connect(m_rotateTimer, &QTimer::timeout, this, &Role::updateKnivesRotation);
    m_rotateTimer->start(16);
}

Role::~Role(){

}
void Role::updateKnivesRotation(){
    if(knife_num<4)knife_num++;
    angle+=(rotateSpeed/knife_num);
    angle = angle>360?angle-360:angle;
    for(int i = 0; i < knife_num; i++){
        knives[i]->setTransformOriginPoint(knives[i]->boundingRect().center());
        knives[i]->setRotation(i * 360 / knife_num + angle);
        knives[i]->setPos(120*(1+(double)knife_num / 32) * cos(2 * M_PI * ((double)i / knife_num + double(angle)/360)), 
            120 * (1 + (double)knife_num / 32) * sin(2 * M_PI * ((double)i / knife_num + (double)angle/360)));
    }

}
// void Role::updateHP(){
//
// }
