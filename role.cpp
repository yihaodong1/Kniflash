#include "role.h"
#include "myitem.h"
Role::Role(QGraphicsItem* parent): Character(parent){
    m_movie = new QMovie(":/figs/capoo.gif");
    if (!m_movie->isValid()) {
        qDebug() << "Failed to load GIF!";
    }
    m_movie->setScaledSize(QSize(160, 135));
    connect(m_movie, &QMovie::frameChanged, this, 
        [this]() { setPixmap(m_movie->currentPixmap()); 
            setZValue(1);});
    m_movie->start();

    status = 0;
    cloud = new QGraphicsEllipseItem(-10, 100, 30, 30, this);
    QColor color(150, 147, 147);
    cloud->setBrush(color);
    cloud->setPen(QPen(color));
    cloud->setVisible(false);

}

Role::~Role(){

}
// void Role::updateHP(){
//
// }
