#include "chooselevelscene.h"
#include <QMenuBar>
#include <QPainter>
#include "mypushbutton.h"
#include <QDebug>
#include <QTimer>
#include <QLabel>
#include <QSound>

chooselevelscene::chooselevelscene(QWidget *parent) : QMainWindow(parent)
{
    //配置选择关卡场景
    this->setFixedSize(320,588);

    //设置图标
    this->setWindowIcon(QPixmap(":/res/Coin0001.png"));

    //设置标题
    this->setWindowTitle("选择关卡");

    //创建菜单栏
    QMenuBar *bar = menuBar();
    setMenuBar(bar);

    //创建开始菜单
    QMenu * startMenu = bar->addMenu("开始");

    //创建退出 菜单项
    QAction * quitAction = startMenu->addAction("退出");

    //点击退出 实现退出游戏
    connect(quitAction,&QAction::triggered,[=](){
        this->close();
    });

    //选择关卡按钮的音效
    QSound *chooseSound = new QSound(":/res/TapButtonSound.wav",this);

    //返回 按钮的音效
    QSound *backSound = new QSound(":/res/BackButtonSound.wav",this);

    //返回按钮
    MyPushButton * backBtn = new MyPushButton(":/res/BackButton.png" , ":/res/BackButtonSelected.png");
    backBtn->setParent(this);
    backBtn->move(this->width()-backBtn->width(),this->height()-backBtn->height());

    //点击返回
    connect(backBtn,&MyPushButton::clicked,[=](){
        //返回按钮 音效
        backSound->play();

        //告诉主场景L40 我返回了，主场景监听ChooseLevelScene的返回按钮
        //延时返回
        QTimer::singleShot(500,this,[=](){
            emit this->chooseSceneBack();
        });
    });

    playScene = NULL;

    //创建选择关卡的按钮
    for(int i=0;i<20;i++)
    {
        MyPushButton * menuBtn = new MyPushButton(":/res/LevelIcon.png");
        menuBtn->setParent(this);
        menuBtn->move(25 + i % 4 * 70,130 + i / 4 * 70);
        connect(menuBtn,&MyPushButton::clicked,[=](){
//            QString str = QString ("您选择的是第 %1 关 ").arg(i+1);
//            qDebug()<<str;

            //播放点击选择按钮的音效
            chooseSound->play();

            //进入到游戏场景
            //将选关场景隐藏
            this->hide();
            //创建游戏场景
            playScene= new PlayScene(i+1);

            //将游戏场景的位置 同步为 选择关卡场景的位置
            playScene->setGeometry(this->geometry());

            //显示游戏场景
            playScene->show();

            //监听 游戏 场景的 返回按钮
            connect(playScene,&PlayScene::chooseSceneBack,[=](){

                //设置选择关卡场景位置 为 游戏场景返回时候的位置
                this->setGeometry(playScene->geometry());

                playScene->close();
                delete playScene;
                playScene = NULL;

                //选择关卡场景 显示
                this->show();
            });
        });

        QLabel * label = new QLabel;
        label->setParent(this);
        label->setFixedSize(menuBtn->width(),menuBtn->height());
        label->setText(QString::number(i+1));
        label->move(25 + i%4 * 70,130 + i/4 *70);
        //设置label上的文字对齐方式为 水平居中 垂直居中
        label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        //设置让鼠标进行穿透
        label->setAttribute(Qt::WA_TransparentForMouseEvents);
    }
}

//重写绘图事件
void chooselevelscene::paintEvent(QPaintEvent *)
{
    //加载背景
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/OtherSceneBg.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);

    //加载标题
    pix.load(":/res/Title.png");
    painter.drawPixmap( (this->width() - pix.width())*0.5,30,pix.width(),pix.height(),pix);
}
