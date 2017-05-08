#include "zznake.h"

ZZnake::ZZnake(QObject *parent) : QObject(parent)
{
    head = new Node{3, 0, nullptr, nullptr};
    Node *tmp = new Node{2, 0, head, nullptr};
    Node *tmp2 = new Node{1, 0, tmp, nullptr};
    tail = new Node{0, 0, tmp2, nullptr};
    head->prev = tmp;
    tmp->prev = tmp2;
    tmp2->prev = tail;
}

void ZZnake::move(Direction dir, int food[])
{
    Node *n = new Node{head->x, head->y, nullptr, head};
    head->next = n;
    switch (dir)
    {
    case UP:
        n->y--;
        break;

    case DOWN:
        n->y++;
        break;

    case LEFT:
        n->x--;
        break;

    case RIGHT:
        n->x++;
        break;
    }
    head = n;
    if(head->x >= 0)
    {
        head->x %= 31;
    }
    else
    {
        head->x = 31 + head->x;
    }
    if(head->y >= 0)
    {
        head->y %= 23;
    }
    else
    {
        head->y = 23 + head->y;
    }
    if((head->x == food[0]) && (head->y == food[1]))
    {
        emit omnonon();
    }
    else
    {
        tail = tail->next;
        delete tail->prev;
        tail->prev = nullptr;
    }
    for(Node *node = head->prev; node != nullptr; node = node->prev)
    {
        if((head->x == node->x) && (head->y == node->y))
        {
            emit bite();
        }
    }
}

