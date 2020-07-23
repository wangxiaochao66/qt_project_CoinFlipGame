#ifndef CHOOSELEVELSCENE_H
#define CHOOSELEVELSCENE_H

#include <QMainWindow>
#include "playscene.h"

class chooselevelscene : public QMainWindow
{
    Q_OBJECT
public:
    explicit chooselevelscene(QWidget *parent = 0);

    //重写绘图事件
    void paintEvent(QPaintEvent *);

    //游戏场景对象指针
    PlayScene * playScene;

signals:
    //写一个 自定义信号，告诉主场景 点击了返回
    //只需声明  不需要实现
    void chooseSceneBack();

public slots:
};

#endif // CHOOSELEVELSCENE_H
