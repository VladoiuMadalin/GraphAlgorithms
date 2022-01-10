#pragma once
#include <QPoint>

class Node
{
public:
    Node() = delete;
    Node(const QPointF& coord);

    QPointF GetCoord() const;
    void SetCoord(QPointF newCoord);

private:
    QPointF m_coord;
};


