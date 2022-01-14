#pragma once
#include <cstdint>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <optional>

class Graph
{

public:
	Graph();
	Graph(uint32_t nrNodes);
	Graph(const Graph& other) = default;
	Graph& operator=(const Graph& other) = default;

	void AddNode();
	void AddEdge(uint32_t firstNode, uint32_t secondNode, uint32_t capacity);
	void RemoveEdge(uint32_t firstNode, uint32_t secondNode);

	std::vector<uint32_t> FindPath(uint32_t sourceNode, uint32_t destNode) const;
	std::unordered_set<uint32_t> ReacheableNodes(uint32_t sourceNode) const;

	std::optional<uint32_t> GetCapacity(uint32_t firstNode, uint32_t secondNode);

	uint32_t MinCapacity(const std::vector<uint32_t>& path) const;
	uint32_t UpdateGraphAlongPath(const std::vector<uint32_t>& path);
	std::optional<uint32_t> EdgeCap(uint32_t firstNode, uint32_t secondNode) const;
	std::pair<uint32_t, std::vector<std::pair<uint32_t, uint32_t>>> MaxFlowMinCut() const;

	bool SetSourceNode(uint32_t node);
	bool SetSinkNode(uint32_t node);

	size_t GetNrNodes() const;
	const std::unordered_set<uint32_t>& GetNeighbours(uint32_t node) const;

	std::optional<uint32_t> GetSourceNode() const;
	std::optional<uint32_t> GetSinkNode() const;

	void Read(std::istream& in);

public:
	struct HashPairOfInts
	{
		size_t operator()(std::pair<uint32_t, uint32_t>) const;
	};
private:
	size_t m_nrNodes;
	std::optional<uint32_t> m_sourceNode;
	std::optional<uint32_t> m_sinkNode;
	std::vector <std::unordered_set<uint32_t>> m_neighbours;
	std::unordered_map<std::pair<uint32_t, uint32_t>, uint32_t, HashPairOfInts> m_capacity;
};