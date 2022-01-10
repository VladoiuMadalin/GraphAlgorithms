#include "Luxemburg.h"

float dist(QPointF first, QPointF second)
{
	return std::sqrt((first.x() - second.x()) * (first.x() - second.x()) + (first.y() - second.y()) * (first.y() - second.y()));
}


Luxemburg::Luxemburg(QWidget* parent)
	: QMainWindow(parent)
	, m_graph()
	, m_firstSelectedNode(std::nullopt)
{
	ui.setupUi(this);
	m_graph.ReadGraph();
	connect(ui.FindPathButton, &QPushButton::clicked, this, &Luxemburg::on_FindPathButton_clicked);
}

void Luxemburg::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	QPen pen;
	painter.setRenderHint(QPainter::Antialiasing);

	drawEdges(painter);
	drawNodes(painter);

	drawPath(painter);

	update();
}

void Luxemburg::drawNodes(QPainter& painter)
{
	QPen pen = painter.pen();
	pen.setWidth(3);
	pen.setColor(Qt::black);
	painter.setPen(pen);

	const auto& nodes = m_graph.GetNodes();


	for (uint32_t iNode = 0; iNode < nodes.size(); iNode++)
	{
		QPointF scaledCoords = this->scaledCoords(nodes[iNode]->GetCoord());
		painter.drawPoint(scaledCoords);
	}


	if (m_firstSelectedNode.has_value())
	{
		QPen pen = painter.pen();
		pen.setColor(Qt::red);
		pen.setWidth(2);
		painter.setPen(pen);

		QPointF selectedNodeCoords = scaledCoords(nodes[*m_firstSelectedNode]->GetCoord());

		QRect rectToDrawInside(selectedNodeCoords.x() - kNodeRadius, selectedNodeCoords.y() - kNodeRadius,
			kNodeRadius * 2, kNodeRadius * 2);
		painter.drawEllipse(rectToDrawInside);


		pen.setColor(Qt::black);
		pen.setWidth(3);
		painter.setPen(pen);
	}

	if (m_secondSelectedNode.has_value())
	{
		QPen pen = painter.pen();
		pen.setColor(Qt::red);
		pen.setWidth(2);
		painter.setPen(pen);

		QPointF selectedNodeCoords = scaledCoords(nodes[*m_secondSelectedNode]->GetCoord());

		QRect rectToDrawInside(selectedNodeCoords.x() - kNodeRadius, selectedNodeCoords.y() - kNodeRadius,
			kNodeRadius * 2, kNodeRadius * 2);
		painter.drawEllipse(rectToDrawInside);


		pen.setColor(Qt::black);
		pen.setWidth(3);
		painter.setPen(pen);
	}
}

void Luxemburg::drawEdges(QPainter& painter)
{
	QPen pen = painter.pen();
	pen.setWidth(1);
	pen.setColor(Qt::black);
	painter.setPen(pen);

	const auto& edges = m_graph.GetEdges();

	for (uint32_t firstNode = 0; firstNode < m_graph.GetNodes().size(); ++firstNode)
	{
		for (uint32_t secondNode : edges[firstNode])
		{
			painter.drawLine(scaledCoords(m_graph.GetNodes()[firstNode]->GetCoord()),
				scaledCoords(m_graph.GetNodes()[secondNode]->GetCoord()));
		}
	}


	
}

void Luxemburg::drawPath(QPainter& painter)
{
	QPen pen = painter.pen();
	pen.setColor(Qt::red);
	pen.setWidth(4);
	painter.setPen(pen);


	/*for (uint32_t node : m_path)
	{
		painter.drawPoint(scaledCoords(m_graph.GetNodes()[node]->GetCoord()));
	}*/

	if (!m_path.empty())
	{
		pen = painter.pen();
		pen.setWidth(2);
		pen.setColor(Qt::red);
		painter.setPen(pen);
		for (uint32_t iPath = 0; iPath < m_path.size() - 1; ++iPath)
		{
			painter.drawLine(scaledCoords(m_graph.GetNodes()[m_path[iPath]]->GetCoord()),
				scaledCoords(m_graph.GetNodes()[m_path[iPath + 1]]->GetCoord()));
		}
	}


	pen.setColor(Qt::black);
	pen.setWidth(3);
}



void Luxemburg::mouseReleaseEvent(QMouseEvent* event)
{

	if (event->button() == Qt::LeftButton)
	{
		m_firstSelectedNode = GetClosestNode(event->pos());
	}
	else if (event->button() == Qt::RightButton)
	{
		m_secondSelectedNode = GetClosestNode(event->pos());
	}

	if (m_firstSelectedNode.has_value() && m_secondSelectedNode.has_value())
	{
		ui.FindPathButton->setEnabled(true);
	}
}

uint32_t Luxemburg::GetClosestNode(const QPointF& coord)
{
	uint32_t closestNode = 0;
	float smallestDistance = dist(scaledCoords(m_graph.GetNodes()[0]->GetCoord()), coord);
	float currentDistance;

	for (uint32_t currentNode = 1; currentNode < m_graph.GetNodes().size(); ++currentNode)
	{
		currentDistance = dist(scaledCoords(m_graph.GetNodes()[currentNode]->GetCoord()), coord);
		if (smallestDistance > currentDistance)
		{
			closestNode = currentNode;
			smallestDistance = currentDistance;
		}
	}

	return closestNode;
}

void Luxemburg::on_FindPathButton_clicked()
{
	m_path = m_graph.FindShortedPathBetween(*m_firstSelectedNode, *m_secondSelectedNode);
}

float Luxemburg::scaledX(float x)
{
	return this->width() / (m_graph.GetMaxX() - m_graph.GetMinX()) * (x - m_graph.GetMinX());
}

float Luxemburg::scaledY(float y)
{
	return this->height() / (m_graph.GetMaxY() - m_graph.GetMinY()) * (y - m_graph.GetMinY());
}

QPointF Luxemburg::scaledCoords(const QPointF& coords)
{
	return QPointF(scaledX(coords.x()), scaledY(coords.y()));
}

float Luxemburg::unscaledX(float x)
{
	return x * (m_graph.GetMaxX() - m_graph.GetMinX()) / this->width() + m_graph.GetMinX();
}

float Luxemburg::unscaledY(float y)
{
	return y * (m_graph.GetMaxY() - m_graph.GetMinY()) / this->width() + m_graph.GetMinY();
}

QPointF Luxemburg::unscaledCoords(const QPointF& coords)
{
	return QPointF(unscaledX(coords.x()), unscaledY(coords.y()));
}
