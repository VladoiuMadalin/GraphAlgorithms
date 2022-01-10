#pragma once

#include <vector>
#include "node.h"
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <stack>
#include <queue>

struct HashIntPair
{
	size_t operator()(const std::pair<uint32_t, uint32_t> intPair) const
	{
		return std::hash<int>()(intPair.first) ^ std::hash<int>()(intPair.second);
	}
};

class Graph
{
public:

	void AddNode(const QPointF& coord);

	const std::vector< Node*>& GetNodes();
	const std::vector<std::unordered_set<uint32_t>>& GetEdges();
	const std::unordered_map<std::pair<uint32_t, uint32_t>, uint32_t, HashIntPair>& GetCost();

	void AddEdge(uint32_t first, uint32_t second, uint32_t cost);

	std::vector<uint32_t> FindShortedPathBetween(uint32_t firstNode, uint32_t secondNode);



	Graph() = default;
	~Graph();

	void Clear();

	int ReadGraph();

	float GetMaxX() const;
	float GetMaxY() const;
	float GetMinX() const;
	float GetMinY() const;

private:
	std::vector<Node*> m_nodes;
	std::vector<std::unordered_set<uint32_t>> m_edges;
	std::unordered_map<std::pair<uint32_t, uint32_t>, uint32_t, HashIntPair> m_cost;


	float m_minX, m_maxX, m_minY, m_maxY;



	//friend class Luxemburg;
};


