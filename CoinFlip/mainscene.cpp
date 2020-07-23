#include "mainscene.h"
#include "ui_mainscene.h"
#include <QPainter>
#include <QTimer>
#include <QSound>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //主场景配置

    //固定背景大小
    setFixedSize(320,588);

    //设置图标
    setWindowIcon(QIcon(":/res/Coin0001.png"));

    setWindowTitle("翻金币_v1.0");

    //点击开始，退出游戏
    connect(ui->actionquit,&QAction::triggered,[=](){
        this->close();
    });

    //开始按钮
    MyPushButton * startBtn = new MyPushButton(":/res/MenuSceneStartButton.png");
    startBtn->setParent(this);
    startBtn->move( this->width() * 0.5 - startBtn->width() * 0.5 ,this->height() * 0.7 );

    //准备开始按钮 音效
    QSound * startSound = new QSound (":/res/TapButtonSound.wav",this);


    //实例化选择关卡场景
    chooselevelscene * chooseScene = new chooselevelscene;

    //监听选择关卡的返回按钮的信号
    connect(chooseScene,&chooselevelscene::chooseSceneBack,this,[=](){
        chooseScene->hide(); //将选择关卡场景 隐藏掉

        //自身的位置同步为 选择关卡场景的位置
        this->setGeometry(chooseScene->geometry() );

        this->show(); //重新显示主场景
    });

    connect(startBtn,&MyPushButton::clicked,[=](){

        //播放开始音效
        startSound->play();

        //弹起特效
        startBtn->zoom1();
        startBtn->zoom2();

        //延时进入到选择关卡场景中
        QTimer::singleShot(500,this,[=](){
            //自身隐藏
            this->hide();

            //设置选中关卡场景的位置
            chooseScene->setGeometry(this->geometry());

            //显示选择关卡场景
            chooseScene->show();
            });
    });

}

//重新paintEvent事件 画背景图
void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);//指定一个设备，在窗口中画画
    QPixmap pix;//读取图片类QPixmap
    pix.load(":/res/PlayLevelSceneBg.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);

    pix.load(":/res/Title.png");
    pix=pix.scaled(pix.width()*0.5,pix.height()*0.5);

    painter.drawPixmap(10,30,pix);

}

MainWindow::~MainWindow()
{
    delete ui;
}
