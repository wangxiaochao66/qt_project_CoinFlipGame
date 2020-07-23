#include "playscene.h"
#include "mypushbutton.h"
#include "dataconfig.h"

#include <QDebug>
#include <QMenuBar>
#include <QPainter>
#include <QTimer>
#include <QLabel>
#include <QPropertyAnimation>
#include <QSound>

PlayScene::PlayScene(int index)
{
    this->levelIndex = index;

    //初始化游戏场景
    //设置固定大小
    this->setFixedSize(320,588);
    //设置图标
    this->setWindowIcon(QPixmap(":/res/Coin0001.png"));
    //设置标题
    this->setWindowTitle("翻金币场景");

    //创建菜单栏
    QMenuBar * bar = menuBar();
    setMenuBar(bar);

    //创建开始菜单
    QMenu * startMenu = bar->addMenu("开始");

    //创建退出 菜单项
    QAction *  quitAction = startMenu->addAction("退出");

    //点击退出 实现退出游戏
    connect(quitAction,&QAction::triggered,[=](){
        this->close();
    });

    //默认未胜利
    isWin = false ;

    //翻金币 音效
    QSound *flipSound = new QSound(":/res/CoinFlipSound.wav",this);

    //胜利 的音效
    QSound *winSound = new QSound(":/res/LevelWinSound.wav",this);

    //返回 按钮的音效
    QSound *backSound = new QSound(":/res/BackButtonSound.wav",this);

    //返回按钮
    MyPushButton * backBtn = new MyPushButton(":/res/BackButton.png" , ":/res/BackButtonSelected.png");
    backBtn->setParent(this);
    backBtn->move(this->width() - backBtn->width() , this->height() - backBtn->height());

    //点击返回   与选择场景 的 L86 对应
    connect(backBtn,&MyPushButton::clicked,[=](){

        //返回按钮音效
        backSound->play();

        QTimer::singleShot(300,this,[=](){
            emit this->chooseSceneBack();
        });
    });

    //显示当前关卡数
    QLabel * label = new QLabel;
    label->setParent(this);
    QFont font;
    font.setFamily("华文新魏");
    font.setPointSize(20);
    QString str1 = QString("Level: %1").arg(this->levelIndex);
    //将字体设置到标签控件中
    label->setFont(font);
    label->setText(str1);
    label->setGeometry(30,this->height()-50,120,50);

    dataConfig data;
    //初始化游戏中的二维数组
    for(int i=0;i<4;i++)
    {
        for(int j=0;j<4;j++)
        {
            this->gameArray[i][j] = data.mData[this->levelIndex][i][j];
        }
    }

    //创建胜利的图片  先放在窗口上端，看不到，等胜利再下来
    QLabel* winLabel = new QLabel;
    QPixmap pix;
    pix.load(":/res/LevelCompletedDialogBg.png");
    winLabel->setGeometry(0,0,pix.width(),pix.height());
    winLabel->setPixmap(pix);
    winLabel->setParent(this);
    winLabel->move( (this->width() - pix.width())*0.5 , -pix.height());

    //创建金币的背景图片
    for(int i = 0;i<4;i++)
    {
        for(int j =0;j<4;j++)
        {
            //QLabel显示金币背景图片
            QLabel * bg = new QLabel(this);
            QPixmap pix;
            pix.load(":/res/BoardNode.png");
            bg->setGeometry(0,0,pix.width(),pix.height());
            bg->setPixmap(pix);
            bg->move( 57 + i*50,200+j*50);

            //创建金币
            QString str;
            if(this->gameArray[i][j] == 1 )
            {
                //金币
                str = ":/res/Coin0001.png";
            }
            else
            {
                //银币
                str = ":/res/Coin0008.png";
            }
            MyCoin * coin = new MyCoin(str);
            coin->setParent(this);
            coin->move( 59 + i*50 , 204+j*50);
            coin->posX = i; //记录x坐标
            coin->posY = j; //记录y坐标
            coin->flag =gameArray[i][j]; //记录正反标志


            //将coin放入到维护的金币的二维数组中
            coinBtn[i][j] = coin;

            connect(coin,&MyCoin::clicked,[=](){
                // 点击金币 音效
                flipSound->play();

                //!!!!点击后 将所有按钮先禁止
                for(int i=0;i<4;i++)
                {
                    for(int j=0;j<4;j++)
                    {
                        this->coinBtn[i][j]->isWin = true ;
                    }
                }

                coin->changeFlag();
                //同步内部维护二维数组中的数据
                gameArray[i][j] = gameArray[i][j] ==0 ? 1 : 0;

                //继续翻动 周围的硬币
                //延时
                QTimer::singleShot(300,this,[=](){
                    //翻右侧硬币
                    if(coin->posX + 1 <=3)
                    {
                        coinBtn[coin->posX + 1][coin->posY]->changeFlag();
                        gameArray[coin->posX+1][coin->posY] = gameArray[coin->posX+1][coin->posY] == 0 ? 1 : 0;
                    }
                    //翻左侧硬币
                    if(coin->posX - 1 >=0)
                    {
                        coinBtn[coin->posX - 1][coin->posY]->changeFlag();
                        gameArray[coin->posX-1][coin->posY] = gameArray[coin->posX-1][coin->posY] == 0 ? 1 : 0;
                    }
                    //翻下侧硬币
                    if(coin->posY + 1 <=3)
                    {
                        coinBtn[coin->posX][coin->posY + 1]->changeFlag();
                        gameArray[coin->posX][coin->posY+1] = gameArray[coin->posX][coin->posY+1] == 0 ? 1 : 0;
                    }
                    //翻上侧硬币
                    if(coin->posY - 1 >=0)
                    {
                        coinBtn[coin->posX][coin->posY - 1]->changeFlag();
                        gameArray[coin->posX][coin->posY-1] = gameArray[coin->posX][coin->posY-1] == 0 ? 1 : 0;
                    }

                    //!!!!翻完所有银币后 将所有的按钮解禁
                    for(int i=0;i<4;i++)
                    {
                        for(int j=0;j<4;j++)
                        {
                            this->coinBtn[i][j]->isWin = false ;
                        }
                    }

                    //在翻完周围硬币后，判断是否都是金币 ，如果是，则游戏胜利
                    this->isWin = true ;
                    for(int i=0;i<4;i++)
                    {
                        for(int j=0;j<4;j++)
                        {
                            if(coinBtn[i][j]->flag == false)
                            {
                                this->isWin = false;
                                break;
                            }
                        }
                    }
                    if(this->isWin == true)
                    {
                        //胜利音效
                        winSound->play();

                        //将每一个按钮都设置为true，激活鼠标不可以按的事件
                        //防止胜利后还可以按金币 造成bug
                        for(int i=0;i<4;i++)
                        {
                            for(int j=0;j<4;j++)
                            {
                                coinBtn[i][j]->isWin = true ;
                            }
                        }

                        //将胜利的图片显示出来
                        QPropertyAnimation * animation1 =  new QPropertyAnimation(winLabel,"geometry");
                        animation1->setDuration(1000);
                        animation1->setStartValue(QRect(winLabel->x(),winLabel->y(),winLabel->width(),winLabel->height()));
                        animation1->setEndValue(QRect(winLabel->x(),winLabel->y()+114,winLabel->width(),winLabel->height()));
                        animation1->setEasingCurve(QEasingCurve::OutBounce);
                        animation1->start();
                    }

                });
            });
        }
    }

}

void PlayScene::paintEvent(QPaintEvent *)
{
    //创建背景
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/PlayLevelSceneBg.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);

    //加载标题
    pix.load(":/res/Title.png");
    pix = pix.scaled(pix.width()*0.5,pix.height()*0.5);
    painter.drawPixmap( 10,30,pix.width(),pix.height(),pix);

}
