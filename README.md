# 文件树
```
.
├── character.cpp         //人物基类
├── character.h
├── example.pro           //工程文件
├── figs                  //图形资源，如道具图片、人物图片等
│   ├── background.jpg
│   ├── boots.png
│   ├── bush.png
│   ├── capoo.gif
│   ├── case.png
│   ├── heart.png
│   ├── knife.png
│   └── slime.png
├── main.cpp
├── mainwindow.cpp
├── mainwindow.h
├── mainwindow.ui
├── musics                //音乐资源，背景音乐、拾取道具音乐等
│   ├── background.mp3
│   ├── clash.mp3
│   ├── death.mp3
│   ├── hurt.mp3
│   └── pickup.mp3
├── mygraphicsview.cpp
├── mygraphicsview.h
├── myitem.cpp            //自定义的道具类，包括刀、加血、加速
├── myitem.h
├── npc.cpp               //电脑人继承自Character类
├── npc.h
├── README.md             //本文件
├── README.pdf            //pa2代码帮助文档
├── role.cpp              //控制的游戏人物，继承自Character类
├── role.h
└── src.qrc               //项目使用的资源

3 directories, 31 files
```
# 类和数据结构
- Character类
```
character.h
class HealthBar{
    qreal m_width;                        // 绘制的血条宽度
    qreal m_height;                       // 高度
    qreal m_currentHealth;                // 现在血量
    qreal m_maxHealth;                    // 满血血量
};
class Character{
    QTimer m_speedUpTimer;                // 加速后的倒计时计时器
    int speedUpCount = 0;                 // 加速后的倒计时
    QGraphicsPathItem *path;              // 加速倒计时动画

    QTimer m_rotateTimer;                 // 飞刀旋转计时器
    QTimer m_knifeaddTimer;               // 当飞刀数小于4时，飞刀自动增加的计时器
    int kills;                            // 杀死的对手数目
    int knife_num;                        // 周围的飞刀数
    double step = 10;                     // 运动速度
    double angle;                         // 飞刀的旋转角度
    double rotateSpeed;                   // 飞刀的旋转速度
    HealthBar *line;                      // 自定义类HealthBar，显示血条
    QList<QGraphicsPixmapItem*> knives;   // 人物周围的飞刀数
}
```
```
character.cpp
- void updateKnivesRotation();            // 定时更新飞刀旋转角度
- void updateKnifenum();                  // 当飞刀数小于4时，定时增加飞刀数
- double getStep(){return step;}          // 返回移速
- QRectF boundingRect() const override {
      // 获取原始 pixmap 的边界
      QRectF baseRect = QGraphicsPixmapItem::boundingRect();
      return baseRect;
  }
- void knifeNumAdder();                                   // 增加飞刀数
- void updateSpeedUpCount();                              // 定时更新加速倒计时
- void speedUp(){                                         // 设置加速状态
      step = step / 2 * 3;
      m_speedUpTimer.start(1000);
      speedUpCount = 5;
  }
- int getHP(){return line->getHealth();}                  //返回血量
- void hpAdder(){line->setHealth(line->getHealth()+5);}   //拾取到加血道具后加血
- int getKnifeNum(){return knife_num;}                    //返回飞刀数
- void useKnife(int n = 1){                               // 消耗飞刀
      knife_num-=n;
      for(int i = 0; i < n; i++){
          delete knives[0];
          knives.pop_front();
      }
  }
- void bleed(int n = 1){line->setHealth(line->getHealth()- n * 25);}// 掉血
- void closeAttack(Character *other, int &total);         // 近距离攻击
- void rangedAttack(Character *other);                    // 远距离攻击
- void killed(){kills++;}                                 // 击杀数加一
- int getKills(){return kills;}                           // 获得击杀数
```
- Role类
```
role.h
class Role: public Character{
    int status;                     // 记录运动状态，0是静止, 1是运动
    QGraphicsEllipseItem *cloud;    // 跑动特效
    QMovie *m_movie;                // 加载gif动图
}
```
```
role.cpp

- void setStatus(int s){status = s; s==MOVING?cloud->setVisible(true):cloud->setVisible(false);}                //设置运动状态，并在运动是显示运动特效
```
- Npc类
```
npc.h
class Npc: public Character{
    QTimer m_posTimer;                // 位置更新计时器
    QTimer m_dirTimer;                // 方向更新计时器
    QTimer m_beattack;                // 被远程攻击特效更新计时器
    int dir;                          // 人机移动方向
    QGraphicsEllipseItem *beattack;   // 被远程攻击的特效
}
```
```
npc.cpp
- void updatePos();                             // 更新位置
- void updateDir(){dir = rand()%4;}             // 更新移动方向
- void beAttack(){beattack->setVisible(true);   // 被远程攻击时显示特效
    m_beattack.stop();
    connect(&m_beattack, &QTimer::timeout,[&](){
          m_beattack.stop();
          beattack->setVisible(false);
        });
    m_beattack.start(1000);
  }
```
- MyItem类
```
class MyItem{
public:
    enum KIND{                        //三种道具类型
        KNIFE, SPEED, HP
    };
private:
    enum KIND kind;
public:
    enum KIND getKind(){return kind;} //返回道具类型
}
```
- MainWindow类
```
class MainWindow{
private:
    QStackedWidget *stack;            // 使用stackwidget结构
}
```
- MyGraphicsView
```
class MyGraphicsView{
private:
    void playMusic(enum MUSIC_TYPE type);
    void freshItem(Character *c);
    QList<Npc*> npcs;                                     // 人机
    QList<MyItem*> items;                                 // 道具
    std::vector<QGraphicsPixmapItem*> bushes;             // 草丛
    Role *m_role;                                         // 角色
    QGraphicsPixmapItem *m_background;                    // 背景图像项
    QMovie *m_movie;                                      // 用于播放 GIF

    QTimer m_update;                                      // 更新游戏总体逻辑计时器
    QTimer m_closeAttackTimer;                            // 计时器，用于计算每秒近战最多消耗5把飞刀
    QTimer m_npcRangedAttackTimer;                        // 计时器，用于计算人机每秒自动向锁定的角色发射飞刀

    QGraphicsLineItem* trace;                             // 用于绘制索敌特效
    int totalseconds = 0;                                 // 统计生存时长
private:
    bool w_pressed = false;                               // 记录按键按下状态
    bool a_pressed = false;
    bool s_pressed = false;
    bool d_pressed = false;
public:
    void updateGame();                                    // 更新游戏逻辑函数
    void npcRangedAttack();                               // npc自动向索敌发射飞刀
    void checkCloseAttack();                              // 定时检查近距离攻击
    Npc *findNear();                                      // 返回最近的人机

protected:
    // 键盘事件, 用来移动背景
    // 注意 override
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
signals:
    void showBtnToMenu();                                 // 游戏结束时发出信号，显示返回主页的按钮
}
```
