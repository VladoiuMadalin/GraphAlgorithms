#include "Graph.h"

#include <QtXml/qxml.h>
#include <QtXml/qdom.h>
#include <QFile>
#include <QtCore>
#include "QDebug"
#include <fstream>



void Graph::AddNode(const QPointF& coord)
{


	//m_edges.insert({ nodeToInsert,std::unordered_set<Node*>() });

	if (m_nodes.empty())
	{
		m_minX = m_maxX = coord.x();
		m_minY = m_maxY = coord.y();
	}
	else
	{
		if (coord.x() < m_minX)
		{
			m_minX = coord.x();
		}
		else if (coord.x() > m_maxX)
		{
			m_maxX = coord.x();
		}
		if (coord.y() < m_minY)
		{
			m_minY = coord.y();
		}
		else if (coord.y() > m_maxY)
		{
			m_maxY = coord.y();
		}
	}
	Node* nodeToInsert = new Node(coord);
	m_nodes.push_back(nodeToInsert);
	m_edges.emplace_back();
}

const std::vector<Node*>& Graph::GetNodes()
{
	return m_nodes;
}

const std::vector<std::unordered_set<uint32_t>>& Graph::GetEdges()
{
	return m_edges;
}

const std::unordered_map<std::pair<uint32_t, uint32_t>, uint32_t, HashIntPair>& Graph::GetCost()
{
	return m_cost;
}
void Graph::AddEdge(uint32_t first, uint32_t second, uint32_t cost)
{
	m_nodes;
	if (first != second)
	{
		m_edges[first].insert(second);
		m_cost[{first, second}] = cost;
	}
}

std::vector<uint32_t> Graph::FindShortedPathBetween(uint32_t firstNode, uint32_t secondNode)
{
	struct NodeWithCost
	{
		NodeWithCost(uint32_t node, uint32_t cost)
			:node(node)
			,cost(cost)
		{
		}

		uint32_t node;
		uint32_t cost;
	};

	std::vector<uint32_t> path;

	std::vector<bool> visited(m_nodes.size(), false);
	std::vector<std::optional<uint32_t>> prev(m_nodes.size(), std::nullopt);
	std::vector<uint32_t> smallestCost(m_nodes.size(), UINT32_MAX);
	smallestCost[firstNode] = 0;

	auto compare = [](NodeWithCost firstNode, NodeWithCost secondNode) ->bool {return firstNode.cost > secondNode.cost; };
	std::priority_queue<NodeWithCost, std::vector<NodeWithCost>, std::function<bool(NodeWithCost, NodeWithCost)>> q(compare);

	
	for (uint32_t node = 0; node < m_nodes.size(); ++node)
	{
		q.emplace(node, UINT32_MAX);
	}
	q.emplace(firstNode, 0);
	

	while (!q.empty())
	{
		NodeWithCost currentNode = q.top();
		q.pop();

		if (visited[currentNode.node])
		{
			continue;
		}
		/*if (ajuns la destinatie)
		{
			opreste alogritmul
		}*/

		visited[currentNode.node] = true;

		for (uint32_t neighbour : m_edges[currentNode.node])
		{
			if (!visited[neighbour])
			{
				uint32_t costBetweenNodes = m_cost[{currentNode.node, neighbour}];
				uint32_t newCost = currentNode.cost + costBetweenNodes;

				if (newCost < smallestCost[neighbour])
				{
					q.emplace(neighbour, newCost);
					prev[neighbour] = currentNode.node;
					smallestCost[neighbour] = newCost;
				}
			}
		}
	}

	uint32_t node = secondNode;
	
	while (node != firstNode)
	{
		path.push_back(node);
		node = prev[node].value();
	}
	path.push_back(firstNode);

	return path;
}



Graph::~Graph()
{
	Clear();
}

void Graph::Clear()
{
	for (Node* node : m_nodes)
	{
		delete node;
	}
	m_nodes.clear();
	m_edges.clear();
}


int Graph::ReadGraph()
{
	QDomDocument xml;
	QFile file("hartaLuxembourg.xml");
	std::ofstream out("console.txt");

	if (!file.open(QIODevice::ReadOnly))
	{
		// Error while loading file
		std::cerr << "Error while loading file" << std::endl;
		return 1;
	}
	xml.setContent(&file);
	file.close();
	QDomElement root = xml.documentElement();
	// Get root names and attributes


	QDomElement Component = root.firstChild().toElement();

	QDomElement Child = Component.firstChild().toElement();

	while (!Component.isNull())
	{
		// Check if the child tag name is COMPONENT
		if (Component.tagName() == "nodes")
		{

			// Get the first child of the component
			QDomElement Child = Component.firstChild().toElement();
			int id;
			int longitude;
			int latitude;

			// Read each child of the component node
			while (!Child.isNull())
			{

				int longitude = Child.attribute("longitude").toInt();
				int latitude = Child.attribute("latitude").toInt();

				AddNode(QPointF(longitude, latitude));

				Child = Child.nextSibling().toElement();
			}
		}
		if (Component.tagName() == "arcs")
		{

			// Get the first child of the component
			QDomElement Child = Component.firstChild().toElement();

			// Read each child of the component node
			while (!Child.isNull())
			{
				uint32_t from = Child.attribute("from").toInt();
				uint32_t to = Child.attribute("to").toInt();
				uint32_t length = Child.attribute("length").toInt();

				//out << from << " " << to << " " << length << "\n";

				AddEdge(from, to, length);

				// Next child
				Child = Child.nextSibling().toElement();
			}
		}

		// Next component
		Component = Component.nextSibling().toElement();
	}
	return 0;
}

float Graph::GetMaxX() const
{
	return m_maxX;
}

float Graph::GetMaxY() const
{
	return m_maxY;
}

float Graph::GetMinX() const
{
	return m_minX;
}

float Graph::GetMinY() const
{
	return m_minY;
}
