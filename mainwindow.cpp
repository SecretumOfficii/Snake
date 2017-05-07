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
    painter.setPen(Qt::NoPen);
    brush.setColor(QColor(181, 230, 29));
    brush.setStyle(Qt::SolidPattern);
    painter.setPen(pen);
    painter.setBrush(brush);
    painter.drawRect(0, 0, width(), height());
    drawSnake(&painter);
    drawFood(&painter);
    if(gameOver)
    {
        QFont font;
        font.setPointSize(64);
        painter.setFont(font);
        pen.setColor(Qt::red);
        painter.setPen(pen);
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
    brush.setColor(Qt::blue);
    pen.setColor(Qt::gray);
    pen.setWidth(5);
    painter->save();
    painter->setPen(pen);
    painter->setBrush(brush);

    const Node *node = snake->getHead();
    painter->drawEllipse(node->x * 20, node->y * 20, 20, 20);

    brush.setColor(Qt::white);
    pen.setColor(Qt::black);
    pen.setWidth(1);
    painter->setPen(pen);
    painter->setBrush(brush);
    switch (dir) {
    case UP:
        painter->drawEllipse(node->x * 20 + 5, node->y * 20 + 5, 5, 5);
        painter->drawEllipse(node->x * 20 + 10, node->y * 20 + 5, 5, 5);
        break;
    case DOWN:
        painter->drawEllipse(node->x * 20 + 5, node->y * 20 + 15, 5, 5);
        painter->drawEllipse(node->x * 20 + 10, node->y * 20 + 15, 5, 5);
        break;
    case LEFT:
        painter->drawEllipse(node->x * 20 + 5, node->y * 20 + 5, 5, 5);
        painter->drawEllipse(node->x * 20 + 5, node->y * 20 + 10, 5, 5);
        break;
    case RIGHT:
        painter->drawEllipse(node->x * 20 + 10, node->y * 20 + 5, 5, 5);
        painter->drawEllipse(node->x * 20 + 10, node->y * 20 + 10, 5, 5);
        break;
    }

    brush.setColor(Qt::blue);
    pen.setColor(Qt::gray);
    pen.setWidth(5);
    painter->setPen(pen);
    painter->setBrush(brush);

    for( node = node->prev; node != nullptr; node = node->prev)
    {
        painter->drawEllipse(node->x * 20, node->y * 20, 20, 20);
    }
    painter->restore();
}

void MainWindow::drawFood(QPainter *painter)
{
    QPen pen;
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::red);
    pen.setColor(Qt::yellow);
    pen.setWidth(5);
    painter->setPen(pen);
    painter->setBrush(brush);
    painter->save();
    painter->drawRect(food[0] * 20, food[1] * 20, 20, 20);
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
        food[0] = rand() % 32;
        food[1] = rand() % 24;
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
    makeFood();
}

void MainWindow::biten()
{
    gameOver = true;
}

void MainWindow::restart()
{
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
