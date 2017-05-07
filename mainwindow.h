#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <zznake.h>

// 32 * 24

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void paintEvent(QPaintEvent *);
    void timerEvent(QTimerEvent *);
    void keyPressEvent(QKeyEvent *ev);
    void mousePressEvent(QMouseEvent *ev);

private:
    ZZnake *snake;
    Direction dir;
    bool KeyPressFlag;
    int food[2];
    bool gameOver;

    void drawSnake(QPainter *painter);
    void drawFood(QPainter *painter);
    void makeFood();
    void restart();

public slots:
    void eaten();
    void biten();
};

#endif // MAINWINDOW_H
