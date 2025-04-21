#include "npc.h"
Npc::Npc(QGraphicsItem* parent): Character(parent){
    step = 5;
    QPixmap fig(":/figs/slime.png");
    if (fig.isNull()) {
        // 时常使用 qDebug 输出调试信息, 是好的开发习惯
        qDebug() << "Failed to load slime image!";
    }
    fig = fig.scaled(120, 100);
    setPixmap(fig);
    connect(&m_posTimer, &QTimer::timeout, this, &Npc::updatePos);
    m_posTimer.start(16);

    dir = rand()%4;
    connect(&m_dirTimer, &QTimer::timeout, this, &Npc::updateDir);
    m_dirTimer.start(1000);

    beattack = new QGraphicsEllipseItem(0, 80, 120, 40, this);
    beattack->setPen(QPen(Qt::red, 6));
    beattack->setFlag(QGraphicsItem::ItemStacksBehindParent, true);
    beattack->setVisible(false);
}
Npc::~Npc(){}
enum{
    UP, DOWN, LEFT, RIGHT
};
void Npc::updatePos(){
    QPointF ds(0, 0);
    switch(dir){
      case UP:
          ds = QPoint(0, -step);
          break;
      case DOWN:
          ds = QPoint(0, step);
          break;
      case LEFT:
          ds = QPoint(-step, 0);
          break;
      case RIGHT:
          ds = QPoint(step, 0);
          break;
    }
    qreal distance = QLineF(parentItem()->scenePos()+parentItem()->boundingRect().center(),
        this->scenePos() + this->boundingRect().center() + ds).length();
    if(distance < 1800 / 2){
        this->setPos(this->pos() + ds);
    }
}
