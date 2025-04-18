#include "mygraphicsview.h"

#include <QDebug>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QKeyEvent>
#include <QMovie>
#include <QPainter>
#include <QTimer>
#include <QtGlobal>
#include <QLineF>

// 槽函数: 处理信号, 展示游戏窗口
void MyGraphicsView::handleEvokeGameSignal() { this->show(); }


MyGraphicsView::MyGraphicsView(QGraphicsScene *scene, QWidget *parent)
    : QGraphicsView(scene, parent) {
    setFocusPolicy(Qt::StrongFocus);
    setWindowTitle("Moving demo");

    // 加载并处理圆形背景
    // ":/" 表示从 qrc 加载资源
    QPixmap backgroundPixmap(":/figs/background.jpg");
    if (backgroundPixmap.isNull()) {
        // 时常使用 qDebug 输出调试信息, 是好的开发习惯
        qDebug() << "Failed to load background image!";
    }

    const int bgSize = 1800;  // 圆形背景的直径
    QPixmap scaledBg =
        backgroundPixmap.scaled(bgSize, bgSize, Qt::KeepAspectRatioByExpanding,
                                Qt::SmoothTransformation);

    QPixmap circularBg(bgSize, bgSize);
    circularBg.fill(Qt::transparent);
    QPainter painter(&circularBg);
    painter.setRenderHints(QPainter::Antialiasing |
                           QPainter::SmoothPixmapTransform);
    painter.setClipRegion(QRegion(0, 0, bgSize, bgSize, QRegion::Ellipse));
    painter.drawPixmap((bgSize - scaledBg.width()) / 2,
                       (bgSize - scaledBg.height()) / 2, scaledBg);

    m_background = scene->addPixmap(circularBg);
    m_background->setPos(0, 0);
    scene->setSceneRect(0, 0, bgSize, bgSize);


    const int bushsize = 200;
    for(int i = 0; i < 9; i++){
        QPixmap bushPixmap(":/figs/bush.png");
        bushPixmap = bushPixmap.scaled(bushsize, bushsize);
        if (bushPixmap.isNull()) {
            // 时常使用 qDebug 输出调试信息, 是好的开发习惯
            qDebug() << "Failed to load bush image!";
        }
        QGraphicsPixmapItem *bush = scene->addPixmap(bushPixmap);
        bush->setZValue(10);
        bushes.push_back(bush);
    }
    const int radius = 500;
    bushes[0]->setPos(450+rand()%900, 450 + rand()%900);
    bushes[1]->setPos(900 + radius * cos(rand()%100 / (double)100 * M_PI / 2), 900 + radius * sin(rand()%100 / (double)100 * M_PI / 2));
    bushes[3]->setPos(900 + radius * cos(M_PI / 2 + rand()%100 / (double)100 * M_PI / 2), 900 + radius * sin(M_PI / 2 + rand()%100 / (double)100 * M_PI / 2));
    bushes[5]->setPos(900 + radius * cos(M_PI + rand()%100 / (double)100 * M_PI / 2), 900 + radius * sin(M_PI + rand()%100 / (double)100 * M_PI / 2));
    bushes[7]->setPos(900 + radius * cos(M_PI / 2 * 3 + rand()%100 / (double)100 * M_PI / 2), 900 + radius * sin(M_PI / 2 * 3 + rand()%100 / (double)100 * M_PI / 2));
    bushes[2]->setPos(900 + radius * cos(rand()%100 / (double)100 * M_PI / 2), 900 + radius * sin(rand()%100 / (double)100 * M_PI / 2));
    bushes[4]->setPos(900 + radius * cos(M_PI / 2 + rand()%100 / (double)100 * M_PI / 2), 900 + radius * sin(M_PI / 2 + rand()%100 / (double)100 * M_PI / 2));
    bushes[6]->setPos(900 + radius * cos(M_PI + rand()%100 / (double)100 * M_PI / 2), 900 + radius * sin(M_PI + rand()%100 / (double)100 * M_PI / 2));
    bushes[8]->setPos(900 + radius * cos(M_PI / 2 * 3 + rand()%100 / (double)100 * M_PI / 2), 900 + radius * sin(M_PI / 2 * 3 + rand()%100 / (double)100 * M_PI / 2));

    for(int i = 0; i < 10; i++){
        items.push_back(new MyItem(MyItem::KNIFE, m_background));
        items[i]->setPos(rand()%(int)scene->width(), rand()%(int)scene->height());
    }
    for(int i = 10; i < 13; i++){
        items.push_back(new MyItem(MyItem::SPEED, m_background));
        items[i]->setPos(rand()%(int)scene->width(), rand()%(int)scene->height());
    }
    for(int i = 13; i < 16; i++){
        items.push_back(new MyItem(MyItem::HP, m_background));
        items[i]->setPos(rand()%(int)scene->width(), rand()%(int)scene->height());
    }
    // 加载人物 GIF
    m_movie = new QMovie(":/figs/capoo.gif");
    if (!m_movie->isValid()) {
        qDebug() << "Failed to load GIF!";
    }
    m_movie->setScaledSize(QSize(160, 135));

    m_role = new Role();
    scene->addItem(m_role);
    m_role->setPos(scene->width() / 2,
                   scene->height() / 2);  // 初始位置在场景中心
    m_role->setZValue(1);
    connect(&m_update, &QTimer::timeout, this,
            &MyGraphicsView::updateGame);
    m_update.start(30);

    // 居中显示人物
    centerOn(m_role);

    for(int i = 0; i < 5; i++){
        npcs.push_back(new Npc(m_background));
        npcs[i]->setPos(rand()%(int)scene->width(), rand()%(int)scene->height());
    }
}


void MyGraphicsView::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
        case Qt::Key_W:
            w_pressed = true;
            break;
        case Qt::Key_A:
            a_pressed = true;
            break;
        case Qt::Key_S:
            s_pressed = true;
            break;
        case Qt::Key_D:
            d_pressed = true;
            break;
    }
    this->m_role->setStatus(w_pressed | a_pressed | s_pressed | d_pressed);
}

void MyGraphicsView::keyReleaseEvent(QKeyEvent *event) {
    switch (event->key()) {
        case Qt::Key_W:
            w_pressed = false;
            break;
        case Qt::Key_A:
            a_pressed = false;
            break;
        case Qt::Key_S:
            s_pressed = false;
            break;
        case Qt::Key_D:
            d_pressed = false;
            break;
    }
    this->m_role->setStatus(w_pressed | a_pressed | s_pressed | d_pressed);
}

void MyGraphicsView::mousePressEvent(QMouseEvent *event){
    if(m_role->getKnifeNum() > 0){
        m_role->useKnife();
        qreal min = QLineF(m_role->scenePos()+m_role->boundingRect().center(), 
                npcs[0]->scenePos() + npcs[0]->boundingRect().center()).length();
        Npc *near_npc = npcs[0];
        for(auto npc: npcs){
            qreal l = QLineF(m_role->scenePos() + m_role->boundingRect().center(), 
                npc->scenePos() + npc->boundingRect().center()).length();
            if(l < min){
                min = l;
                near_npc = npc;
            }
        }
        if(min < 300){
            near_npc->beAttack();
            if(near_npc->getKnifeNum() > 0)
                near_npc->useKnife();
            else
                near_npc->bleed();
        }
    }
    QGraphicsView::mousePressEvent(event);
}

void MyGraphicsView::freshItem(Character *c){
    for(MyItem* it: items){
        qreal l = QLineF(it->scenePos() + it->boundingRect().center(), 
            c->scenePos() + c->boundingRect().center()).length();
        if(l<50){
            switch(it->getKind()){
                case MyItem::KNIFE:
                    it->setPos(rand()%(int)scene()->width(), rand()%(int)scene()->height());
                    c->knifeNumAdder();
                    break;
                case MyItem::SPEED:
                    items.removeAll(it);
                    delete it;
                    c->speedUp();
                    break;
                case MyItem::HP:
                    items.removeAll(it);
                    delete it;
                    c->hpAdder();
                    break;
            }
        }
    }
}
void MyGraphicsView::updateGame() {
    freshItem(m_role);
    for(auto npc: npcs){
        freshItem(npc);
    }

    const double step = m_role->getStep();
    const int bgsize = 1800;
    QPointF ds = QPointF(1, 0);
    if(w_pressed){
        ds += QPointF(0, step);
    }
    if(s_pressed){
        ds += QPointF(0, -step);
    }
    if(a_pressed){
        ds += QPointF(step, 0);
    }
    if(d_pressed){
        ds += QPointF(-step, 0);
    }
    qreal distance = QLineF(m_background->scenePos()+m_background->boundingRect().center() + ds,
        m_role->scenePos() + m_role->boundingRect().center()).length();
    if(distance < bgsize / 2){
        m_background->moveBy(ds.x(), ds.y());
        for(auto it: bushes){
            it->moveBy(ds.x(), ds.y());
        }
    }
    centerOn(m_role);  // 强制视图始终居中显示角色
    scene()->update();
}
