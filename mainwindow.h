#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QStackedLayout>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QWidget>
#include "mygraphicsview.h"
#include <QMediaPlayer>
#include <QAudioOutput>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    /**
     * 注: Qt 中使用信号与槽机制的步骤
     * 1) 在你需要通知的类添加信号(信号也可以传参)
     * 2) 书写信号触发之后的函数
     * 3) 连接该信号与函数
     * 4) 在你想要触发的地方 emit 这个信号
     */
    // 当开始界面窗口的按钮被按下时, 会触发这个信号
    // 这里对应步骤(1), 定义一个信号
    void evokeGameSignal();

private slots:
    void on_pushButton_clicked();
    void handleShowBtnToMenu();

private:
    QStackedWidget *stack;
    Ui::MainWindow *ui;
    QPushButton *btnToMenu;
};
#endif // MAINWINDOW_H
