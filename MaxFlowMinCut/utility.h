#pragma once

#include <utility>
#include <cmath>
#include <QPoint>
#include <vector>
#include <ostream>

float dist(QPointF first, QPointF second);
std::pair<float, float> SecondDegreeEq(float a, float b, float c);
std::pair<float, float> GetParamOfLineEq(const QPointF& A, const QPointF& B);
std::pair<float, float> GetPerpendicularLineEq(std::pair<float, float> eq, const QPointF& point);
QPointF GetPointOnLineAtDistFromPoint(std::pair<float, float> eq, const QPointF& point, float dist);


std::pair<QPointF, QPointF> GetIntersectionBetweenLineAndCircle(float R, QPointF centre, float m, float n);
std::pair<QPointF, QPointF> GetClosestPoints(std::pair<QPointF, QPointF> firstPointsPair, std::pair<QPointF, QPointF> secondPointsPair);
bool equalF(float first, float second);
void printMatrix(std::vector<std::vector<bool>>, std::ostream& out);
