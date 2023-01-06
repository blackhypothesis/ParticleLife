#pragma once

#include <algorithm>
#include <iterator>
#include <iomanip>
#include <sstream>
#include <memory>

#include <SFML/Graphics.hpp>
#include "ParticleType.h"
#include "Force.h"

#include <iostream>
#include <cmath>


class Graph
{
public:
	Graph(sf::Vector2f, int, int, Force&);
	virtual ~Graph();

private:
	int p1, p2;
	sf::Vector2f position;
	sf::Vector2f size;
	float border, gridBorder;
	sf::Color gridColor;
	sf::Color gridZeroColor;
	sf::RectangleShape backGroundRect;
	std::vector<sf::RectangleShape> xGrid;
	std::vector<sf::RectangleShape> yGrid;
	std::unique_ptr<std::vector<std::map<std::string, float>>> vecForceP1P2;   // holds the distances and forces from Force object
	std::unique_ptr<std::vector<std::map<std::string, float>>> vecGraphP1P2;   // distances and forces scaled to grid rectangle
	std::vector<sf::RectangleShape> vecPlotPoint;                              // position of points to display force = f(distance)
	sf::VertexArray vertexPlotLine;                                            // line to connect points
	sf::CircleShape p1Circle;
	sf::CircleShape p2Circle;

	static sf::Vector2f graphSize;

	float minX, maxX, minY, maxY;
	float xDeltaGrid, yDeltaGrid;
	float xRatio, yRatio;

	bool editMode; 
	int pointSelected;

	bool hoverInfo;
	sf::RectangleShape infoBox;
	sf::Font fontConsolas;
	sf::Text pointInfo;

	Force& force;

private:
	void calculateGrid();
	void calculateVecGraphPointRatio();
	void calculateVecPlotPoint();

public:
	static sf::Vector2f getGraphSize();
	static void setGraphSize(sf::Vector2f);


	int getP1() const;
	int getP2() const;

	float getMinX() const;
	void setMinX(float);

	float getMaxX() const;
	void setMaxX(float);

	float getMinY() const;
	void setMinY(float);

	float getMaxY() const;
	void setMaxY(float);

	void updateVecForceP1P2();

	bool mouseAction(sf::Vector2i, bool, bool);
	void draw(sf::RenderTarget&) const;
};

