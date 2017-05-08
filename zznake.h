#ifndef ZZNAKE_H
#define ZZNAKE_H

#include <QObject>

struct Node
{
    int x;
    int y;
    Node *next;
    Node *prev;
};


enum Direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

class ZZnake : public QObject
{
    Q_OBJECT
public:
    explicit ZZnake(QObject *parent = 0);
    const Node *getHead() const {return head;}
    const Node *getTail() const {return tail;}
    void move(Direction dir, int food[]);

private:
    Node *head;
    Node *tail;

signals:
    void omnonon();
    void bite();

public slots:
};

#endif // ZZNAKE_H
