#ifndef PLAYSCENE_H
#define PLAYSCENE_H

#include <QMainWindow>
#include "mycoin.h"

class PlayScene : public QMainWindow
{
    Q_OBJECT
public:
    //explicit PlayScene(QWidget *parent = 0);

    PlayScene(int index);

    //使关卡号称为内部属性
    int levelIndex;

    //重写paintEvent事件
    void paintEvent(QPaintEvent *);

    //游戏二维数组 维护里面金币或者银币标志
    int gameArray[4][4];

    //金币按钮数组
    MyCoin * coinBtn[4][4];

    //是否胜利
    bool isWin;

signals:
    void chooseSceneBack();
public slots:
};

#endif // PLAYSCENE_H
