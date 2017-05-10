#include "mainwindow.h"
#include "zznake.h"
#include <QPainter>
#include <QKeyEvent>
#include <QFont>
#include <cstdlib>
#include <ctime>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      dir(RIGHT),
      gameOver(false)
{
    border = 10;
    setFixedSize(640, 480);
    snake = new ZZnake(this);
    srand(time(NULL));
    makeFood();
    connect(snake, &ZZnake::omnonon, this, &MainWindow::eaten);
    connect(snake, &ZZnake::bite, this, &MainWindow::biten);
    startTimer(100);
}

MainWindow::~MainWindow()
{

}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPen pen;
    QBrush brush;
    pen.setColor(QColor(128, 128, 0));
    pen.setWidth(border);
    brush.setColor(QColor(181, 230, 29));
    brush.setStyle(Qt::SolidPattern);
    painter.setPen(pen);
    painter.setBrush(brush);
    painter.drawRect(border / 2, border / 2, width() - border, height() - border);
    drawSnake(&painter);
    drawFood(&painter);
    drawEyes(&painter);
    QString tmp;
    tmp = count;
    if(gameOver)
    {
        QFont font;
        font.setPointSize(64);
        painter.setFont(font);
        pen.setColor(Qt::red);
        painter.setPen(pen);
        QString tmp = "score: ";
        tmp += QString::number(count);
        painter.drawText(width() / 2 - 200, height() / 2 - 200,
                         400, 100, Qt::AlignCenter, tmp);
        painter.drawText(0, 0, width(), height(), Qt::AlignCenter, "Game Over!");
        brush.setColor(QColor(194, 71, 220));
        painter.setBrush(brush);
        painter.drawRoundedRect(width() / 2 - 200, height() / 2 + 50, 400, 100, 25, 25);
        pen.setColor(Qt::yellow);
        painter.setPen(pen);
        painter.drawText(width() / 2 - 200, height() / 2 + 50,
                         400, 100, Qt::AlignCenter, "Restart");
    }
}

void MainWindow::drawSnake(QPainter *painter)
{
    QPen pen;
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(QColor(228, 155, 15));
    pen.setColor(QColor(178, 34, 34));
    pen.setWidth(4);
    painter->save();
    painter->setPen(pen);
    painter->setBrush(brush);

    for(const Node *node = snake->getHead(); node != nullptr; node = node->prev)
    {
        painter->drawEllipse(node->x * 20 + border, node->y * 20 + border, 20, 20);
    }
    painter->restore();
}

void MainWindow::drawFood(QPainter *painter)
{
    QPen pen;
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::blue);
    pen.setColor(QColor(255 ,117, 24));
    pen.setWidth(4);
    painter->setPen(pen);
    painter->setBrush(brush);
    painter->save();
    painter->drawRect(food[0] * 20 + border, food[1] * 20 + border, 20, 20);
    painter->restore();
}

void MainWindow::drawEyes(QPainter *painter)
{
    int r = 7;
    QPen pen;
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::black);
    pen.setColor(Qt::white);
    pen.setWidth(3);
    painter->save();
    painter->setBrush(brush);
    painter->setPen(pen);
    switch (dir) {
    case UP:
        painter->drawEllipse(snake->getHead()->x * 20 + border,
                             snake->getHead()->y * 20 + border, r, r);
        painter->drawEllipse(snake->getHead()->x * 20 + (20 - r) + border,
                             snake->getHead()->y * 20 + border, r, r);
        break;

    case DOWN:
        painter->drawEllipse(snake->getHead()->x * 20 + border,
                             snake->getHead()->y * 20 + (20-r) + border, r, r);
        painter->drawEllipse(snake->getHead()->x * 20 + (20-r) + border,
                             snake->getHead()->y * 20 + (20-r) + border, r, r);
        break;

    case LEFT:
        painter->drawEllipse(snake->getHead()->x * 20 + border,
                             snake->getHead()->y * 20 + border, r, r);
        painter->drawEllipse(snake->getHead()->x * 20 + border,
                             snake->getHead()->y * 20 + (20-r) + border, r, r);
        break;

    case RIGHT:
        painter->drawEllipse(snake->getHead()->x * 20 + (20-r) + border,
                             snake->getHead()->y * 20 + border, r, r);
        painter->drawEllipse(snake->getHead()->x * 20 + (20-r) + border,
                             snake->getHead()->y * 20 + (20-r) + border, r, r);
        break;
    }
    painter->restore();
}

void MainWindow::timerEvent(QTimerEvent *)
{
    if(!gameOver)
    {
        KeyPressFlag = true;
        snake->move(dir, food);
    }
    update();
}

void MainWindow::keyPressEvent(QKeyEvent *ev)
{
    Direction tmpDir;
    if (KeyPressFlag)
    {
        switch (ev->key())
        {
        case Qt::Key_Up:
            tmpDir = UP;
            if (dir != DOWN)
            {
                dir = tmpDir;
            }
            break;

        case Qt::Key_Down:
            tmpDir = DOWN;
            if (dir != UP)
            {
                dir = tmpDir;
            }
            break;

        case Qt::Key_Left:
            tmpDir = LEFT;
            if (dir != RIGHT)
            {
                dir = tmpDir;
            }
            break;

        case Qt::Key_Right:
            tmpDir = RIGHT;
            if (dir != LEFT)
            {
                dir = tmpDir;
            }
            break;

        case Qt::Key_Escape:
            close();
        }
        KeyPressFlag = false;
    }
}

void MainWindow::makeFood()
{
    bool ok;
    do
    {
        ok = true;
        food[0] = rand() % 31;
        food[1] = rand() % 23;
        for(const Node *node = snake->getHead(); node != nullptr; node = node->prev)
        {
            if((node->x == food[0]) && (node->y == food[1]))
            {
                ok = false;
                continue;
            }
        }
    }
    while(!ok);
}

void MainWindow::eaten()
{
    count++;
    makeFood();
}

void MainWindow::biten()
{
    gameOver = true;
}

void MainWindow::restart()
{
    count = 0;
    gameOver = false;
    delete snake;
    snake = new ZZnake(this);
    connect(snake, &ZZnake::omnonon, this, &MainWindow::eaten);
    connect(snake, &ZZnake::bite, this, &MainWindow::biten);
    dir = RIGHT;
    makeFood();
}

void MainWindow::mousePressEvent(QMouseEvent *ev)
{
    if(gameOver && ev->button() == Qt::LeftButton)
    {
        if((ev->x() > width() / 2 - 200) && (ev->x() < width() / 2 + 200) &&
           (ev->y() > height() / 2 + 50) && (ev->y() < height() / 2 + 150))
        {
            restart();
        }
    }
}
