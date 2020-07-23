#ifndef MYCOIN_H
#define MYCOIN_H

#include <QPushButton>

class MyCoin : public QPushButton
{
    Q_OBJECT
public:
    //explicit MyCoin(QWidget *parent = 0);

    //参数 功能 显示默认是金币还是银币
    MyCoin(QString CoinImg);

    int posX; //x坐标
    int posY; //y坐标
    bool flag;//正反标志
    bool isWin;//是否胜利的标志

    void changeFlag();//改变标志，执行翻转效果
    QTimer *timer1;   //正面翻反面 定时器
    QTimer *timer2;   //反面翻正面 定时器
    int min=1;        //最小图片
    int max=8;        //最大图片

    //创建是否执行动画标志
    bool isAnimation;

    //鼠标按下事件重写
    void mousePressEvent(QMouseEvent *e);

signals:

public slots:
};


#endif // MYCOIN_H
