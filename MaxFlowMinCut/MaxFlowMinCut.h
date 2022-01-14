#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MaxFlowMinCut.h"
#include "Graph.h"
#include <QPoint>
#include <QPainter>

class MaxFlowMinCut : public QMainWindow
{
	Q_OBJECT

public:
	static constexpr float kNodeRadius = 20;
	static constexpr float kArrowSize = 15;
	static constexpr float kArrowAngle = 60;
	static constexpr float kRandomCircleRadius = 300;

	MaxFlowMinCut(QWidget* parent = Q_NULLPTR);
	std::optional<uint32_t> GetNodeInProx(const QPointF& coord);
	bool overLaps(uint32_t node) const;

	void paintEvent(QPaintEvent* event) override;

	void drawNodes(QPainter& painter);
	void drawEdges(QPainter& painter);
	void drawEdge(uint32_t firstNode,uint32_t secondNode , QPainter& painter);

	void GenerateCircleOfNodes();

private:
	Ui::MaxFlowMinCutClass m_ui;
	std::vector<QPointF> m_coord;
	Graph m_graph;
	std::unordered_set<std::pair<uint32_t, uint32_t>, Graph::HashPairOfInts> m_minCut;

};
