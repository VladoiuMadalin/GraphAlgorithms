#include "Node.h"

Node::Node(const QPointF& coord)
	: m_coord(coord)
{}

QPointF Node::GetCoord() const
{
	return m_coord;
}

void Node::SetCoord(QPointF newCoord)
{
	m_coord = newCoord;
}

