#include "utility.h"


float dist(QPointF first, QPointF second)
{
	return std::sqrt((first.x() - second.x()) * (first.x() - second.x()) + (first.y() - second.y()) * (first.y() - second.y()));
}


std::pair<float, float> SecondDegreeEq(float a, float b, float c)
{
	float delta = b * b - 4 * a * c;
	if (delta < 0) throw "Negative Delta";
	return std::make_pair((-b + sqrt(delta)) / (2 * a), (-b - sqrt(delta)) / (2 * a));
}

std::pair<float, float> GetParamOfLineEq(const QPointF& A, const QPointF& B)
{
	// y = mx+n
	//A.x != B.x

	if (B.x() == A.x())
	{
		//qDebug()<<"hi";
	}

	float m = (B.y() - A.y()) / (B.x() - A.x());
	float n = A.y() - A.x() * (B.y() - A.y()) / (B.x() - A.x());

	return std::make_pair(m, n);
}

std::pair<float, float> GetPerpendicularLineEq(std::pair<float, float> eq, const QPointF& point)
{
	return { -1 / eq.first, point.y() + 1 / eq.first * point.x() };
}

QPointF GetPointOnLineAtDistFromPoint(std::pair<float, float> eq, const QPointF& point, float dist)
{
	float a = 1 + eq.first * eq.first;
	float b = -2 * point.x() + 2 * eq.first * eq.second - 2 * eq.first * point.y();
	float c = point.x() * point.x() + eq.second * eq.second - 2 * eq.second * point.y() + point.y() * point.y() - dist * dist;

	auto xs = SecondDegreeEq(a, b, c);

	return { xs.first,eq.first * xs.first + eq.second };
}



std::pair<QPointF, QPointF> GetIntersectionBetweenLineAndCircle(float R, QPointF centre, float m, float n)
{
	//y=mx+n    R - radius
	// (x-a)^2 +(y-a)^2 = R^2

	float a = centre.x();
	float b = centre.y();

	float A = 1 + m * m;
	float B = -2 * a + 2 * m * n - 2 * b * m;
	float C = a * a + n * n + b * b - R * R - 2 * b * n;

	std::pair<float, float> xPair = SecondDegreeEq(A, B, C);
	std::pair<float, float> yPair = { m * xPair.first + n,m * xPair.second + n };

	return { QPointF(xPair.first,yPair.first),QPointF(xPair.second,yPair.second) };
}

std::pair<QPointF, QPointF> GetClosestPoints(std::pair<QPointF, QPointF> firstPointsPair, std::pair<QPointF, QPointF> secondPointsPair)
{
	std::pair<QPointF, QPointF> closestPair = { firstPointsPair.first,secondPointsPair.first };
	float shortestDist = dist(firstPointsPair.first, secondPointsPair.first);
	float currentDist;

	currentDist = dist(firstPointsPair.first, secondPointsPair.second);
	if (currentDist < shortestDist)
	{
		shortestDist = currentDist;
		closestPair = { firstPointsPair.first,secondPointsPair.second };
	}

	currentDist = dist(firstPointsPair.second, secondPointsPair.first);
	if (currentDist < shortestDist)
	{
		shortestDist = currentDist;
		closestPair = { firstPointsPair.second,secondPointsPair.first };
	}

	currentDist = dist(firstPointsPair.second, secondPointsPair.second);
	if (currentDist < shortestDist)
	{
		shortestDist = currentDist;
		closestPair = { firstPointsPair.second,secondPointsPair.second };
	}
	return closestPair;
}



bool equalF(float first, float second)
{
	const float epsilon = 1e-2;
	return std::abs(first - second) <= epsilon * std::abs(first);
}

void printMatrix(std::vector<std::vector<bool>> adjacencyMatrix, std::ostream& out)
{
	out << adjacencyMatrix.size() << '\n';
	for (const std::vector<bool>& vect : adjacencyMatrix)
	{
		for (bool value : vect)
		{
			if (value)
			{
				out << 1 << ' ';
			}
			else
			{
				out << 0 << ' ';
			}
		}
		out << '\n';
	}
}