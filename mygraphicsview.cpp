#include "mygraphicsview.h"
#include "role.h"

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

// 槽函数: 更新运动圆形的路径
// void MyGraphicsView::updateCirclePath() {
//     // 沿正方形轨迹移动
//     // 这里用的是什么坐标系?
//     const int speed = 3;
//     QPointF localPos = m_moving_circle->pos() - initPoint;
//
//     switch (currentEdge) {
//         case 0:  // 上边向右移动
//             localPos.rx() += speed;
//             if (localPos.x() > SQUARE_SIZE / 2) {
//                 localPos.rx() = SQUARE_SIZE / 2;
//                 currentEdge = 1;
//             }
//             break;
//         case 1:  // 右边向下移动
//             localPos.ry() += speed;
//             if (localPos.y() > SQUARE_SIZE / 2) {
//                 localPos.ry() = SQUARE_SIZE / 2;
//                 currentEdge = 2;
//             }
//             break;
//         case 2:  // 下边向左移动
//             localPos.rx() -= speed;
//             if (localPos.x() < -SQUARE_SIZE / 2) {
//                 localPos.rx() = -SQUARE_SIZE / 2;
//                 currentEdge = 3;
//             }
//             break;
//         case 3:  // 左边向上移动
//             localPos.ry() -= speed;
//             if (localPos.y() < -SQUARE_SIZE / 2) {
//                 localPos.ry() = -SQUARE_SIZE / 2;
//                 currentEdge = 0;
//             }
//             break;
//     }
//     m_moving_circle->setPos(localPos + initPoint);
// }

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
        // scene->addItem(items[i]);
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
void MyGraphicsView::updateGame() {
    for(MyItem* it: items){
        qreal l = QLineF(it->scenePos() + it->boundingRect().center(), 
            m_role->scenePos() + m_role->boundingRect().center()).length();
        if(l<50){
            it->setPos(rand()%(int)scene()->width(), rand()%(int)scene()->height());
            m_role->knifeNumAdder();
        }
    }

    const double step = m_role->getStep();
    const int bgsize = 1800;
    double centerx = bgsize / 2 + m_background->pos().x();
    double centery = bgsize / 2 + m_background->pos().y();
    double x = m_role->pos().x();
    double y = m_role->pos().y();
    if(x>=centerx && y>=centery){
        x += m_movie->scaledSize().width();
        y += m_movie->scaledSize().height();
    }else if(x <= centerx && y >= centery){
        y += m_movie->scaledSize().height();
    }else if(x >= centerx && y <= centery){
        x += m_movie->scaledSize().width();
    }
    if(w_pressed){
        if((x - centerx) * (x - centerx) + 
            (y - step - centery)* (y - step - centery)
            < (bgsize / 2)*(bgsize / 2)){
            m_background->moveBy(0, step);
            for(auto it: bushes){
                it->moveBy(0, step);
            }
        }
    }
    if(s_pressed){
        if((x - centerx) * (x - centerx) + 
            (y + step - centery)* (y + step - centery)
            < (bgsize / 2)*(bgsize / 2)){
            m_background->moveBy(0, -step);
            for(auto it: bushes){
                it->moveBy(0, -step);
            }
        }
    }
    if(a_pressed){
        if((x - step - centerx) * (x - step - centerx) + 
            (y - centery)* (y - centery)
            < (bgsize / 2)*(bgsize / 2)){
            m_background->moveBy(step, 0);
            for(auto it: bushes){
                it->moveBy(step, 0);
            }
        }
    }
    if(d_pressed){
        if((x + step - centerx) * (x + step - centerx) + 
            (y - centery)* (y - centery)
            < (bgsize / 2)*(bgsize / 2)){
            m_background->moveBy(-step, 0);
            for(auto it: bushes){
                it->moveBy(-step, 0);
            }
        }
    }
    centerOn(m_role);  // 强制视图始终居中显示角色
    scene()->update();
}
