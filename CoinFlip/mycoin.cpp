#include "mycoin.h"
#include <QPixmap>
#include <QMessageBox>
#include <QTimer>

//参数 功能 显示默认是金币还是银币
MyCoin::MyCoin(QString CoinImg)
{
    QPixmap pix;
    bool ret = pix.load(CoinImg);
    if(!ret)
    {
        QString str = QString ("图片加载失败 %1").arg(CoinImg);
        QMessageBox::critical(this,"加载失败",str);
        return;
    }

    this->setFixedSize( pix.width() , pix.height() );
    this->setStyleSheet("QPushButton{border:0px;}");
    this->setIcon(pix);
    this->setIconSize( QSize( pix.width(),pix.height() ) );

    timer1 = new QTimer(this);
    timer2 = new QTimer(this);

    //初始化胜利的标志
    isWin = false ;

    //初始化按钮是否执行动画标志
    isAnimation = false ;

    //监听定时器
    //监听银币翻金币的信号
    connect(timer1,&QTimer::timeout,[=](){
        QPixmap pix;
        QString str = QString(":/res/Coin000%1.png").arg(this->min++);
        pix.load(str);

        this->setFixedSize( pix.width() , pix.height() );
        this->setStyleSheet("QPushButton{border:0px;}");
        this->setIcon(pix);
        this->setIconSize( QSize( pix.width(),pix.height() ) );

        //如果显示到 最后一张图片 停止定时器
        if(this->min > this->max)
        {
            this->min = 1;//重置最小值
            this->isAnimation = false;//做完动画后 执行动画标志改为false
            timer1->stop();
        }
    });

    //监听金币翻银币的信号
    connect(timer2,&QTimer::timeout,[=](){
        QPixmap pix;
        QString str = QString(":/res/Coin000%8.png").arg(this->max--);
        pix.load(str);

        this->setFixedSize( pix.width() , pix.height() );
        this->setStyleSheet("QPushButton{border:0px;}");
        this->setIcon(pix);
        this->setIconSize( QSize( pix.width(),pix.height() ) );

        //如果显示到 最后一张图片 停止定时器
        if(this->max < this->min)
        {
            this->max = 8;//重置最大值
            this->isAnimation = false;//做完动画后 执行动画标志改为false
            timer2->stop();
        }
    });

}

//改变标志，执行翻转效果
void MyCoin::changeFlag()
{
    if(this->flag == 1)//当前是正面
    {
        //执行动画标志
        isAnimation = true ;
        //开启 正面 翻 反面 的定时器
        timer1->start(30);
        //同步标志
        this->flag = false;
    }
    else //当前是反面
    {
        //执行动画标志
        isAnimation = true ;
        //开启 反面 翻 正面 的 定时器
        timer2->start(30);
        //同步标志
        this->flag = true;
    }
}

//鼠标按下事件重写
void MyCoin::mousePressEvent(QMouseEvent *e)
{
    if(this->isAnimation || this->isWin)
    {
        //正在做动画，不做鼠标响应
        return;
    }
    else
    {
        //交给父类 做默认处理
        QPushButton::mousePressEvent(e);
    }
}
