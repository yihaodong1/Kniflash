#include "mainwindow.h"

// 这个头文件是哪来的? 为什么需要 include 它? 头文件里的内容是什么意思?
// #include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    // , ui(new Ui::MainWindow)
{
    QMediaPlayer *player = new QMediaPlayer(this);
    QAudioOutput *audioOutput = new QAudioOutput(this);
    player->setAudioOutput(audioOutput);
    player->setSource(QUrl("qrc:/musics/background.mp3"));
    player->setLoops(QMediaPlayer::Infinite);
    audioOutput->setVolume(0.1);
    player->play();
    stack = new QStackedWidget(this);
    stack->setFixedSize(1080, 675);

    QWidget* menu = new QWidget;

    // 填充页面内容...

    QVBoxLayout *layout1 = new QVBoxLayout(menu);

    QPushButton *btnToGame = new QPushButton("Start game");

    QPushButton *btnExit = new QPushButton("Exit");
    layout1->addWidget(btnExit);
    layout1->addWidget(btnToGame);

    stack->addWidget(menu);

    setCentralWidget(stack);
    QObject::connect(btnToGame, &QPushButton::clicked, [&]() {
        if(stack->count() == 2){
            QWidget *page = stack->widget(1); // 获取索引1的页面
            stack->removeWidget(page);        // 从stack里移除
            delete page;                      // 删除上局游戏页面对象，释放内存
        }
        QWidget* game = new QWidget;
        QStackedLayout* layout2 = new QStackedLayout(game);
        layout2->setStackingMode(QStackedLayout::StackAll); // 关键：允许控件重叠
        QGraphicsScene *scene = new QGraphicsScene;
        MyGraphicsView *view = new MyGraphicsView(scene, game);
        view->resize(1080, 675);
        // 关闭滚动条
        view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        QPushButton *btnToMenu = new QPushButton("Return to menu", game);
        btnToMenu->setFixedSize(150, 50);
        // btnToMenu->setStyleSheet("background-color: transparent;");
        btnToMenu->hide();
        layout2->addWidget(view);
        // layout2->addWidget(btnToMenu);
        btnToMenu->move(480, 500);
        stack->addWidget(game);
        QObject::connect(btnToMenu, &QPushButton::clicked, [&]() {
            stack->setCurrentIndex(0);
        });
        QObject::connect(view, &MyGraphicsView::showBtnToMenu, [=]() {
            btnToMenu->show();
            btnToMenu->raise();
        });

        stack->setCurrentIndex(1);
    });

    QObject::connect(btnExit, &QPushButton::clicked, [&]() {
        this->close();
    });
    // ui->setupUi(this);

    // 连接按钮点击事件
    // 当 slot 命名为 `on_按钮名称_操作` 时, Qt 会自动将按钮与槽连接起来
    // 但还是建议显示地连接一下
    // connect(ui->pushButton, &QPushButton::clicked, this,
    //         &MainWindow::on_pushButton_clicked);
    // connect(ui->pushButton_1, &QPushButton::clicked, this,
    //         [=](){
    // 	//显示序号为0个界面
    //     ui->stackedWidget->setCurrentIndex(1);
    // });
    // connect(ui->pushButton_2, &QPushButton::clicked, this,
    //         [=](){
    // 	//显示序号为0个界面
    //     ui->stackedWidget->setCurrentIndex(0);
    // });
}

MainWindow::~MainWindow() { //delete ui; 
                            }

