#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Luxemburg.h"
#include <QPainter>
#include "Graph.h"
#include <QMouseEvent>
#include <optional>

class Luxemburg : public QMainWindow
{
	Q_OBJECT

public:
	static const int kNodeRadius = 10;
public:
	Luxemburg(QWidget* parent = Q_NULLPTR);
	void paintEvent(QPaintEvent* event) override;
	void drawNodes(QPainter& painter);
	void drawEdges(QPainter& painter);
	void drawPath(QPainter& painter);

	void mouseReleaseEvent(QMouseEvent* event) override;
	uint32_t GetClosestNode(const QPointF& coords);

	void on_FindPathButton_clicked();

private:
	float scaledX(float x);
	float scaledY(float y);
	QPointF scaledCoords(const QPointF& coords);
	float unscaledX(float x);
	float unscaledY(float y);
	QPointF unscaledCoords(const QPointF& coords);


	Ui::LuxemburgClass ui;
	Graph m_graph;
	std::optional<uint32_t> m_firstSelectedNode;
	std::optional<uint32_t> m_secondSelectedNode;
	std::vector<uint32_t> m_path;

};
