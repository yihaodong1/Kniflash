#include "myitem.h"
MyItem::~MyItem(){}
MyItem::MyItem(enum KIND k, QGraphicsItem* parent ): QGraphicsPixmapItem(parent){
    kind = k;
    switch(k){
        case KNIFE:{
            QPixmap casePixmap(":/figs/case.png");
            if (casePixmap.isNull()) {
                qDebug() << "Failed to load case image!";
            }
            casePixmap = casePixmap.scaled(125, 100);
            setPixmap(casePixmap);

            QPixmap knifePixmap(":/figs/knife.png");
            if (knifePixmap.isNull()) {
                qDebug() << "Failed to load knife image!";
            }
            knifePixmap = knifePixmap.scaled(123, 100);
            QGraphicsPixmapItem *knifePixmapItem = new QGraphicsPixmapItem(knifePixmap, this);
            knifePixmapItem->setTransformOriginPoint(knifePixmapItem->boundingRect().center());
            knifePixmapItem->setRotation(270);
            // knifePixmapItem->setPos(this->boundingRect().center());
            break;
        }
        case SPEED:
        break;
        case HP:
        break;

    }
    setZValue(100);
}
