#include "MaxFlowMinCut.h"
#include "utility.h"
#include <QMouseEvent>
#include <QLine>
#include <fstream>
#include <QString>
#include <algorithm>

MaxFlowMinCut::MaxFlowMinCut(QWidget* parent)
	: QMainWindow(parent)
{
	m_ui.setupUi(this);
	std::ifstream in("input.txt");
	m_graph.Read(in);
	GenerateCircleOfNodes();

	auto [maxFlow, minCut] = m_graph.MaxFlowMinCut();
	m_ui.labelMaxFlowValue->setText(QString::number(maxFlow));
	std::copy(minCut.begin(), minCut.end(), std::inserter(m_minCut, m_minCut.end()));
}

std::optional<uint32_t> MaxFlowMinCut::GetNodeInProx(const QPointF& coord)
{
	for (uint32_t node = 0; node < m_graph.GetNrNodes(); node++)
	{
		if (dist(m_coord[node], coord) < kNodeRadius)
		{
			return node;
		}
	}

	return std::nullopt;
}

bool MaxFlowMinCut::overLaps(uint32_t searchedNode) const
{
	for (uint32_t node = 0; node < m_graph.GetNrNodes(); node++)
	{
		if (dist(m_coord[node], m_coord[searchedNode]) < kNodeRadius * 2 && node != searchedNode)
		{
			return true;
		}
	}
	return false;
}

void MaxFlowMinCut::paintEvent(QPaintEvent* event)
{

	QPainter painter(this);
	QPen pen;
	pen.setWidth(3);

	painter.setPen(pen);
	painter.setRenderHint(QPainter::Antialiasing);

	drawNodes(painter);
	drawEdges(painter);


	update();
}




void MaxFlowMinCut::drawNodes(QPainter& painter)
{
	for (uint32_t node = 0; node < m_graph.GetNrNodes(); node++)
	{
		QRect rectToDrawInside(m_coord[node].x() - kNodeRadius, m_coord[node].y() - kNodeRadius, kNodeRadius * 2, kNodeRadius * 2);
		painter.drawEllipse(rectToDrawInside);
		painter.drawText(rectToDrawInside, Qt::AlignCenter, std::to_string(node).c_str());
	}
}

void MaxFlowMinCut::drawEdges(QPainter& painter)
{


	for (uint32_t node = 0; node < m_graph.GetNrNodes(); node++)
	{
		for (uint32_t neighbour : m_graph.GetNeighbours(node))
		{
			if (m_minCut.find({ node,neighbour }) != m_minCut.end())
			{
				QPen pen = painter.pen();
				pen.setColor(Qt::red);
				painter.setPen(pen);
				drawEdge(node, neighbour, painter);
				pen = painter.pen();
				pen.setColor(Qt::black);
				painter.setPen(pen);
			}
			else
			{
				drawEdge(node, neighbour, painter);
			}
		}
	}
}


void MaxFlowMinCut::drawEdge(uint32_t firstNode, uint32_t secondNode, QPainter& painter)
{
	QPointF firstPoint = m_coord[firstNode];
	QPointF secondPoint = m_coord[secondNode];
	QPointF pointForCapacity;

	if (!equalF(firstPoint.x(), secondPoint.x()))
	{
		std::pair<float, float> paramOfLineEq = GetParamOfLineEq(firstPoint, secondPoint);

		std::pair<QPointF, QPointF> intersectionPoints1 = GetIntersectionBetweenLineAndCircle
		(kNodeRadius, firstPoint, paramOfLineEq.first, paramOfLineEq.second);

		std::pair<QPointF, QPointF> intersectionPoints2 = GetIntersectionBetweenLineAndCircle
		(kNodeRadius, secondPoint, paramOfLineEq.first, paramOfLineEq.second);


		std::pair<QPointF, QPointF> closestPoints = GetClosestPoints(intersectionPoints1, intersectionPoints2);

		firstPoint = closestPoints.first;
		secondPoint = closestPoints.second;
	}
	else
	{
		if (firstPoint.y() > secondPoint.y())
		{
			firstPoint.setY(firstPoint.y() - kNodeRadius);
			secondPoint.setY(secondPoint.y() + kNodeRadius + 1);
		}
		else
		{
			firstPoint.setY(firstPoint.y() + kNodeRadius + 1);
			secondPoint.setY(secondPoint.y() - kNodeRadius);
		}

	}

	QLineF line(secondPoint, firstPoint);
	painter.drawLine(line);

	if (equalF(firstPoint.x(), secondPoint.x()))
	{
		if (firstPoint.x() < secondPoint.x())
		{
			pointForCapacity = { (firstPoint.x() + secondPoint.x()) / 2 - 10, (firstPoint.y() + secondPoint.y()) / 2 };
		}
		else
		{
			pointForCapacity = { (firstPoint.x() + secondPoint.x()) / 2 + 10, (firstPoint.y() + secondPoint.y()) / 2 };
		}
	}
	else if (equalF(firstPoint.y(), secondPoint.y()))
	{
		if (firstPoint.y() < secondPoint.y())
		{
			pointForCapacity = { (firstPoint.x() + secondPoint.x()) / 2 , (firstPoint.y() + secondPoint.y()) / 2 - 10 };
		}
		else
		{
			pointForCapacity = { (firstPoint.x() + secondPoint.x()) / 2 , (firstPoint.y() + secondPoint.y()) / 2 + 10 };
		}
	}
	else
	{
		auto eq = GetPerpendicularLineEq(GetParamOfLineEq(firstPoint, secondPoint), line.center());
		pointForCapacity = GetPointOnLineAtDistFromPoint(eq, line.center(), 10);
	}




	//painter.drawPoint(pointForCapacity);
	painter.drawText(pointForCapacity, QString::number(m_graph.GetCapacity(firstNode, secondNode).value()));

	QLineF lineForArrow = line;
	lineForArrow.setLength(kArrowSize);
	lineForArrow.setAngle(line.angle() + kArrowAngle / 2);
	painter.drawLine(lineForArrow);

	lineForArrow.setAngle(line.angle() - kArrowAngle / 2);
	painter.drawLine((lineForArrow));
}

void MaxFlowMinCut::GenerateCircleOfNodes()
{
	m_coord.resize(m_graph.GetNrNodes());
	QPointF centerOfCircle = this->rect().center();
	for (size_t iNode = 0; iNode < m_graph.GetNrNodes(); ++iNode)
	{
		QPointF coord;
		coord.setX(kRandomCircleRadius * std::cos(iNode * 6.28 / m_graph.GetNrNodes()) + centerOfCircle.x());
		coord.setY(kRandomCircleRadius * std::sin(iNode * 6.28 / m_graph.GetNrNodes()) + centerOfCircle.y());
		m_coord[iNode] = coord;
	}
}

