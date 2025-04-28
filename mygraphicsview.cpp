#include "mygraphicsview.h"

#include <QGraphicsTextItem>
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
#include <cfloat>

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
        bush->setZValue(2);
        bushes.push_back(bush);
    }
    const int radius = 700;
    bushes[0]->setPos(scene->width()/2, scene->height()/2);
    // bushes[1]->setPos(scene->width()/2 + radius, scene->height()/2);
    // bushes[0]->setPos(450+rand()%900, 450 + rand()%900);
    double angle = rand()%100 / (double)100 * M_PI / 2;
    bushes[1]->setPos(scene->width()/2 + radius * cos(angle), 
        scene->height() /2 + radius * sin(angle));
    angle = M_PI / 2 + rand()%100 / (double)100 * M_PI / 2;
    bushes[3]->setPos(scene->width()/2 + radius * cos(angle), 
        scene->height() /2 + radius * sin(angle));
    angle = M_PI + rand()%100 / (double)100 * M_PI / 2;
    bushes[5]->setPos(scene->width()/2 + radius * cos(angle), 
        scene->height() /2 + radius * sin(angle));
    angle = M_PI / 2 * 3 + rand()%100 / (double)100 * M_PI / 2;
    bushes[7]->setPos(scene->width()/2 + radius * cos(angle), 
        scene->height() /2 + radius * sin(angle));
    angle = rand()%100 / (double)100 * M_PI / 2;
    bushes[2]->setPos(scene->width()/2 + radius * cos(angle), 
        scene->height() /2 + radius * sin(angle));
    angle = M_PI / 2 + rand()%100 / (double)100 * M_PI / 2;
    bushes[4]->setPos(scene->width()/2 + radius * cos(angle), 
        scene->height() /2 + radius * sin(angle));
    angle = M_PI + rand()%100 / (double)100 * M_PI / 2;
    bushes[6]->setPos(scene->width()/2 + radius * cos(angle), 
        scene->height() /2 + radius * sin(angle));
    angle = M_PI / 2 * 3 + rand()%100 / (double)100 * M_PI / 2;
    bushes[8]->setPos(scene->width()/2 + radius * cos(angle), 
        scene->height() /2 + radius * sin(angle));


    for(int i = 0; i < 10; i++){
        items.push_back(new MyItem(MyItem::KNIFE, m_background));
        // items[i]->setPos(rand()%(int)scene->width(), rand()%(int)scene->height());
        items[i]->setPos(scene->width() / 2 + sqrt(rand()%(radius * radius)) * cos(rand()%100 / (double)100 * M_PI * 2),
            scene->height() / 2 + sqrt(rand()%(radius * radius)) * sin(rand()%100 / (double)100 * M_PI * 2));
    }
    for(int i = 10; i < 13; i++){
        items.push_back(new MyItem(MyItem::SPEED, m_background));
        // items[i]->setPos(rand()%(int)scene->width(), rand()%(int)scene->height());
        items[i]->setPos(scene->width() / 2 + sqrt(rand()%(radius * radius)) * cos(rand()%100 / (double)100 * M_PI * 2),
            scene->height() / 2 + sqrt(rand()%(radius * radius)) * sin(rand()%100 / (double)100 * M_PI * 2));
    }
    for(int i = 13; i < 16; i++){
        items.push_back(new MyItem(MyItem::HP, m_background));
        // items[i]->setPos(rand()%(int)scene->width(), rand()%(int)scene->height());
        items[i]->setPos(scene->width() / 2 + sqrt(rand()%(radius*radius)) * cos(rand()%100 / (double)100 * M_PI * 2),
            scene->height() / 2 + sqrt(rand()%(radius*radius)) * sin(rand()%100 / (double)100 * M_PI * 2));
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
    connect(&m_closeAttackTimer, &QTimer::timeout, this,
            &MyGraphicsView::checkCloseAttack);
    m_closeAttackTimer.start(1000);
    connect(&m_npcRangedAttackTimer, &QTimer::timeout, this,
            &MyGraphicsView::npcRangedAttack);
    m_npcRangedAttackTimer.start(1000);

    // 居中显示人物
    centerOn(m_role);

    for(int i = 0; i < 5; i++){
        npcs.push_back(new Npc(m_background));
        npcs[i]->setPos(rand()%(int)scene->width(), rand()%(int)scene->height());
    }

    QPen pen(Qt::red, 6);
    pen.setStyle(Qt::DashLine);
    trace = new QGraphicsLineItem();
    trace->setPen(pen);
    scene->addItem(trace);
}

void MyGraphicsView::checkCloseAttack(){
    int total = 5;
    for(auto npc: npcs){
        qreal l = QLineF(m_role->scenePos() + m_role->boundingRect().center(), 
            npc->scenePos() + npc->boundingRect().center()).length();
        if(l < 200){
            m_role->closeAttack(npc, total);
            if(npc->getHP()<=0){
                m_role->killed();
            }
            // qDebug()<<total;
            if(total == 0){
                break;
            }
        }
    }
}

void MyGraphicsView::npcRangedAttack(){
    totalseconds++;
    for(auto i: npcs){
        qreal min = QLineF((i)->scenePos() + (i)->boundingRect().center(), 
                    (m_role)->scenePos() + (m_role)->boundingRect().center()).length();
        Character *near = m_role;
        for(auto j: npcs){
            if(i!=j){
                qreal l = QLineF((i)->scenePos() + (i)->boundingRect().center(), 
                    (j)->scenePos() + (j)->boundingRect().center()).length();
                if(l < min){
                    min = l;
                    near = j;
                }
            }
        }
        if(min < 300){
            (i)->rangedAttack(near);
            if(near->getHP()<=0){
                i->killed();
            }
        }
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
    if(m_role->getKnifeNum() > 0 && npcs.size() > 0){
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
            if(near_npc->getHP() <= 0){
                m_role->killed();
            }
        }
    }
    QGraphicsView::mousePressEvent(event);
}

void MyGraphicsView::freshItem(Character *c){
    const int radius = 800;
    for(MyItem* it: items){
        qreal l = QLineF(it->scenePos() + it->boundingRect().center(), 
            c->scenePos() + c->boundingRect().center()).length();
        if(l<50){
            switch(it->getKind()){
                case MyItem::KNIFE:
                    it->setPos(scene()->width() / 2 + sqrt(rand()%(radius*radius)) * cos(rand()%100 / (double)100 * M_PI * 2),
                        scene()->height() / 2 + sqrt(rand()%(radius*radius)) * sin(rand()%100 / (double)100 * M_PI * 2));
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

Npc* MyGraphicsView::findNear(){

    if(npcs.size() > 0){
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
        return near_npc;
    }
    return nullptr;
}

void MyGraphicsView::updateGame() {
    for(auto npc: npcs){
        if(npc->getHP() <= 0){
            // delete npc;
            npc->deleteLater();
            // npc->setVisible(false);
            npcs.removeAll(npc);
        }
    }
    Npc* near_npc = findNear();
    if(near_npc != nullptr)
        trace->setLine(QLineF(m_role->scenePos()+m_role->boundingRect().center(),
              near_npc->scenePos() + near_npc->boundingRect().center()));
    else{
        trace->setVisible(false);
        QGraphicsTextItem* textItem = scene()->addText(QString(
              "Rank: #1/6\nKills：%1\nSurvive Time: %2s").
            arg(m_role->getKills()).arg(totalseconds));
        textItem->setPos(scene()->width() / 2, scene()->height() / 2);
        textItem->setDefaultTextColor(Qt::green);
        textItem->setZValue(3);
        QFont f;
        f.setPixelSize(50);
        textItem->setFont(f);
        m_closeAttackTimer.stop();
        m_npcRangedAttackTimer.stop();
        m_update.stop();
        emit showBtnToMenu();

    }
    if(m_role->getHP() <= 0){
        QGraphicsTextItem* textItem = scene()->addText(QString(
              "Rank: #%1/6\nKills：%2\nSurvive Time: %3s").
            arg(npcs.size()+1).arg(m_role->getKills()).arg(totalseconds));
        textItem->setPos(scene()->width() / 2, scene()->height() / 2);
        textItem->setDefaultTextColor(Qt::green);
        textItem->setZValue(3);
        QFont f;
        f.setPixelSize(50);
        textItem->setFont(f);
        m_closeAttackTimer.stop();
        m_npcRangedAttackTimer.stop();
        m_update.stop();
        emit showBtnToMenu();
    }


    freshItem(m_role);
    for(auto npc: npcs){
        freshItem(npc);
    }

    const double step = m_role->getStep();
    const int bgsize = 1800;
    QPointF ds = QPointF(0, 0);
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
