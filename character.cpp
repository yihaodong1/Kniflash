#include "character.h"
Character::Character(QGraphicsItem* parent): QObject(), QGraphicsPixmapItem(parent){
    kills = 0;
    line = new HealthBar(100, 10, this);
    line->setPos(20, -10);

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

void Character::closeAttack(Character *other, int &total){
    if(this->getKnifeNum() > other->getKnifeNum()){
        if(other->getKnifeNum() <= total){
            int used = qMin(total, this->getKnifeNum());
            other->bleed(used - other->getKnifeNum());
            other->useKnife(other->getKnifeNum());
            this->useKnife(used);
            total -= used;
        }else{
            other->useKnife(total);
            this->useKnife(total);
            total = 0;
        }
    }else{
        if(this->getKnifeNum() <= total){
            int used = qMin(total, other->getKnifeNum());
            // qDebug()<<used<<other->getKnifeNum();
            if(used > this->getKnifeNum()){
                QMediaPlayer *player = new QMediaPlayer(this);
                QAudioOutput *audioOutput = new QAudioOutput(this);
                player->setAudioOutput(audioOutput);
                audioOutput->setVolume(1);
                player->setSource(QUrl("qrc:/musics/hurt.mp3"));
                player->play();
            }
            this->bleed(used - this->getKnifeNum());
            this->useKnife(this->getKnifeNum());
            other->useKnife(used);
            total -= used;
        }else{
            other->useKnife(total);
            this->useKnife(total);
            total = 0;
        }
    }
}

void Character::rangedAttack(Character *other){
    if(this->getKnifeNum() > 0 && other->getKnifeNum() > 0){
        this->useKnife(1);
        other->useKnife(1);
    }else if(this->getKnifeNum() > 0 && other->getKnifeNum() == 0){
        this->useKnife(1);
        other->bleed(1);
    }else if(other->getKnifeNum() > 0 && this->getKnifeNum() == 0){
        other->useKnife(1);
        this->bleed(1);
    }else if(other->getKnifeNum() < 0 || this->getKnifeNum() < 0){
        assert(0);
    }
}
