#include "Graph.h"
#include <queue>
#include <algorithm>
#include <iostream>

Graph::Graph()
	:m_nrNodes(0)
{
}

Graph::Graph(uint32_t nrNodes)
	: m_nrNodes(nrNodes)
{
	m_neighbours.resize(nrNodes);
}

void Graph::AddNode()
{
	++m_nrNodes;
	m_neighbours.emplace_back();
}

//if there already exists an edge this method overrides it
//if the nodes are the same one, this method does nothing
void Graph::AddEdge(uint32_t firstNode, uint32_t secondNode, uint32_t capacity)
{
	if (firstNode != secondNode && capacity > 0)
	{
		m_neighbours[firstNode].insert(secondNode);
		m_capacity[{firstNode, secondNode}] = capacity;
	}
}

void Graph::RemoveEdge(uint32_t firstNode, uint32_t secondNode)
{
	m_neighbours[firstNode].erase(secondNode);
	m_capacity.erase({ firstNode,secondNode });
}

std::vector<uint32_t> Graph::FindPath(uint32_t sourceNode, uint32_t destNode) const
{
	std::queue<uint32_t> q;
	std::vector<std::optional<uint32_t>> pred(m_nrNodes, std::nullopt);

	q.push(sourceNode);
	pred[sourceNode] = sourceNode; //?


	bool foundDest = false;
	while (!q.empty() && !foundDest)
	{
		uint32_t currentNode = q.front();
		q.pop();


		for (uint32_t neighbour : m_neighbours[currentNode])
		{
			if (!pred[neighbour].has_value())
			{
				q.push(neighbour);
				pred[neighbour] = currentNode;

				if (neighbour == destNode)
				{
					foundDest = true;
					break;
				}
			}
		}

	}


	if (!foundDest)
	{
		return std::vector<uint32_t>();
	}

	std::vector<uint32_t> path;
	uint32_t currentNode = destNode;

	while (currentNode != sourceNode)
	{
		path.push_back(currentNode);
		currentNode = pred[currentNode].value();
	}
	path.push_back(currentNode);

	std::reverse(path.begin(), path.end());

	return path;
}

std::unordered_set<uint32_t> Graph::ReacheableNodes(uint32_t sourceNode) const
{
	std::queue<uint32_t> q;
	std::unordered_set<uint32_t> visited;

	q.push(sourceNode);
	visited.insert(sourceNode);

	while (!q.empty())
	{
		uint32_t currentNode = q.front();
		q.pop();

		for (uint32_t neighbour : m_neighbours[currentNode])
		{
			if (visited.find(neighbour) == visited.end())
			{
				q.push(neighbour);
				visited.insert(neighbour);
			}
		}
	}

	return visited;
}

std::optional<uint32_t> Graph::GetCapacity(uint32_t firstNode, uint32_t secondNode)
{
	auto it = m_capacity.find({ firstNode,secondNode });
	if (it != m_capacity.end())
	{
		return it->second;
	}
	return std::nullopt;
}

uint32_t Graph::MinCapacity(const std::vector<uint32_t>& path) const
{
	if (path.size() >= 2)
	{
		uint32_t minCapacity = m_capacity.at({ path[0], path[1] });

		for (size_t index = 1; index < path.size() - 1; index++)
		{
			uint32_t capacity = m_capacity.at({ path[index] , path[index + 1] });
			if (capacity < minCapacity)
			{
				minCapacity = capacity;
			}
		}
		return minCapacity;
	}
	//else throw?
}

uint32_t Graph::UpdateGraphAlongPath(const std::vector<uint32_t>& path)
{
	uint32_t minCap = MinCapacity(path);

	for (size_t index = 0; index < path.size() - 1; ++index)
	{
		if (minCap == m_capacity[{path[index], path[index + 1]}])
		{
			RemoveEdge(path[index], path[index + 1]);
		}
		else
		{
			m_capacity[{path[index], path[index + 1]}] -= minCap;
		}

		if (EdgeCap(path[index + 1], path[index]).has_value())
		{
			m_capacity[{path[index + 1], path[index]}] += minCap;
		}
		else
		{
			AddEdge(path[index + 1], path[index], minCap);
		}
	}
	return minCap;
}

std::optional<uint32_t> Graph::EdgeCap(uint32_t firstNode, uint32_t secondNode) const
{
	auto it = m_capacity.find({ firstNode,secondNode });

	if (it == m_capacity.end())
	{
		return std::nullopt;
	}
	else
	{
		return it->second;
	}
}

std::pair<uint32_t, std::vector<std::pair<uint32_t, uint32_t>>> Graph::MaxFlowMinCut() const
{

	if (!m_sourceNode || !m_sinkNode)
	{
		return { 0 ,std::vector<std::pair<uint32_t, uint32_t>>() };
	}

	Graph residualGraph = *this;
	uint32_t maxFlow = 0;

	while (true)
	{
		std::vector<uint32_t> path = residualGraph.FindPath(m_sourceNode.value(), m_sinkNode.value());

		if (path.empty())
		{
			break;
		}

		uint32_t minCap = residualGraph.UpdateGraphAlongPath(path);
		maxFlow += minCap;
	}

	auto visited = residualGraph.ReacheableNodes(m_sourceNode.value());


	std::vector < std::pair<uint32_t, uint32_t>> minCut;
	for (uint32_t node = 0; node < m_neighbours.size(); node++)
	{
		if (visited.find(node) == visited.end())
		{
			continue;
		}

		for (uint32_t neighbour : m_neighbours[node])
		{
			if (visited.find(neighbour) == visited.end())
			{
				minCut.emplace_back(node, neighbour);
			}
		}
	}

	return { maxFlow ,minCut };
}

bool Graph::SetSourceNode(uint32_t node)
{
	if (node >= m_nrNodes)
	{
		return false;
	}

	/*for (const auto& adjList : m_edges)
	{
		if (adjList.find(node) != adjList.end())
		{
			return false;
		}
	}*/

	m_sourceNode = node;
	return true;
}

bool Graph::SetSinkNode(uint32_t node)
{
	if (node >= m_nrNodes)
	{
		return false;
	}

	m_sinkNode = node;
	return true;
}

size_t Graph::GetNrNodes() const
{
	return m_nrNodes;
}

const std::unordered_set<uint32_t>& Graph::GetNeighbours(uint32_t node) const
{

	if (node < m_nrNodes)
	{
		return m_neighbours[node];
	}
	return std::unordered_set<uint32_t>();
}



std::optional<uint32_t> Graph::GetSourceNode() const
{
	return m_sourceNode;
}

std::optional<uint32_t> Graph::GetSinkNode() const
{
	return m_sinkNode;
}

void Graph::Read(std::istream& in)
{
	in >> m_nrNodes;
	m_neighbours.resize(m_nrNodes);

	uint32_t sourceNode, sinkNode;

	in >> sourceNode >> sinkNode;
	SetSourceNode(sourceNode);
	SetSinkNode(sinkNode);

	uint32_t firstNode, secondNode, capacity;
	while (in >> firstNode >> secondNode >> capacity)
	{
		AddEdge(firstNode, secondNode, capacity);
	}
}

size_t Graph::HashPairOfInts::operator()(std::pair<uint32_t, uint32_t> pair) const
{
	size_t h = (size_t(pair.first) << 32) + size_t(pair.second);
	h *= 1231231557ull; // "random" uneven integer 
	h ^= (h >> 32);
	return h;
}