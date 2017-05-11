#include "mainwindow.h"
#include "zznake.h"
#include <QPainter>
#include <QKeyEvent>
#include <QFont>
#include <QMediaPlayer>
#include <cstdlib>
#include <ctime>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      snake(nullptr)
{
    border = 10;
    unicorn.load(":/images/food.png");
    setFixedSize(640, 520);
    srand(time(NULL));
    restart();
    player = new QMediaPlayer(this);
    player->setMedia(QUrl::fromLocalFile("../Snake/bite.mp3"));
    player->setVolume(80);
    startTimer(10);
}

MainWindow::~MainWindow()
{

}

void MainWindow::restart()
{
    timer = 0;
    speed = 10;
    gameOver = false;
    score = 0;
    length = 4;
    pauseFlag = false;
    if(snake != nullptr)
    {
        delete snake;
    }
    snake = new ZZnake(this);
    connect(snake, &ZZnake::omnonon, this, &MainWindow::eaten);
    connect(snake, &ZZnake::bite, this, &MainWindow::biten);
    dir = RIGHT;
    makeFood();
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
    painter.translate(0, 40);
    painter.drawRect(border / 2, border / 2 - 40, width() - border, 40 + border / 2);
    painter.drawRect(border / 2, border / 2, width() - border, height() - border - 40);
    drawSnake(&painter);
    drawFood(&painter);
    drawEyes(&painter);
    QString scoreStr = "Score: ";
    scoreStr += QString::number(score);
    QFont font;
    font.setPointSize(16);
    painter.setFont(font);
    pen.setColor(QColor(16, 69, 3));
    painter.setPen(pen);
    painter.drawText(border * 2, -40,
                     640 - border * 2, 40 + border / 2,
                     Qt::AlignLeft | Qt::AlignVCenter, scoreStr);

    QString speedStr = "Speed: ";
    speedStr += QString::number(20 - speed);
    painter.drawText(border * 2, -40,
                     640 - border * 2, 40 + border / 2,
                     Qt::AlignCenter | Qt::AlignVCenter, speedStr);

    QString lengthStr = "Length: ";
    lengthStr += QString::number(length);
    painter.drawText(border * 2, -40,
                     640 - border * 4, 40 + border / 2,
                     Qt::AlignRight | Qt::AlignVCenter, lengthStr);


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
    if(food[2] < 8)
    {
        brush.setColor(Qt::blue);
        pen.setColor(QColor(255 ,117, 24));
        pen.setWidth(4);
        painter->save();
        painter->setPen(pen);
        painter->setBrush(brush);
        painter->drawRect(food[0] * 20 + border, food[1] * 20 + border, 20, 20);
        painter->restore();
    }
    else
    {
        painter->drawImage(food[0] * 20 + border, food[1] * 20 + border, unicorn);
    }
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
    if((!gameOver) and (!pauseFlag))
    {
        if (speed <= timer)
        {
            KeyPressFlag = true;
            snake->move(dir, food);
            timer = 0;
        }
        timer++;
    }
    update();
}

void MainWindow::keyPressEvent(QKeyEvent *ev)
{
    int key = ev->key();
    if  (key == Qt::Key_Space){
        pauseFlag = !pauseFlag;
    }

    if (KeyPressFlag)
    {
        switch (key)
        {
        case Qt::Key_W:
        case Qt::Key_Up:
            switch (dir)
            {
            case LEFT:
            case RIGHT:
                dir = UP;
                break;
            case DOWN:
                speed+=1;
                break;
            case UP:
                if (speed > 1){speed -=1;}
                break;
            }
            break;

        case Qt::Key_S:
        case Qt::Key_Down:
            switch (dir)
            {
            case LEFT:
            case RIGHT:
                dir = DOWN;
                break;
            case UP:
                speed+=1;
                break;
            case DOWN:
                if (speed > 1){speed -=1;}
                break;
            }
            break;

        case Qt::Key_A:
        case Qt::Key_Left:
            switch (dir)
            {
            case UP:
            case DOWN:
                dir = LEFT;
                break;
            case RIGHT:
                speed+=1;
                break;
            case LEFT:
                if (speed > 1){speed -=1;}
                break;
            }
            break;

        case Qt::Key_D:
        case Qt::Key_Right:
            switch (dir)
            {
                case UP:
                case DOWN:
                    dir = RIGHT;
                    break;
                case LEFT:
                    speed+=1;
                    break;
                case RIGHT:
                    if (speed > 1){speed -=1;}
                    break;
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
        food[2] = rand() % 10;
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
    player->play();
    score +=  20 - speed;
    if(food[2] >= 8)
    {
        score += 9 * (20 - speed);
    }
    length++;
    makeFood();
}

void MainWindow::biten()
{
    gameOver = true;
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
