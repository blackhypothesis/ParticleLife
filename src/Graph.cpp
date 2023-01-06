#include "Graph.h"



Graph::Graph(sf::Vector2f inPosition, int inP1, int inP2, Force& inForce) :
	position(inPosition), p1(inP1), p2(inP2), force(inForce), editMode(false), pointSelected(-1), 
	minX(0.0f), maxX(50.0f), minY(-1.0f), maxY(1.0f),
	xDeltaGrid(10.0f), yDeltaGrid(0.5f), xRatio(1.0f), yRatio(1.0f), hoverInfo(false)
{	
	vecForceP1P2 = std::make_unique<std::vector<std::map<std::string, float>>>();
	vecGraphP1P2 = std::make_unique<std::vector<std::map<std::string, float>>>();

	*vecForceP1P2 = force.getForceMatrixP1P2(p1, p2);
	*vecGraphP1P2 = *vecForceP1P2;


	border = 2.0f;
	size = { graphSize.x - 2 * border, graphSize.y - 2 * border};
	gridBorder = 4.0f;
	gridColor = { 96, 96, 96 };
	gridZeroColor = { 192, 192, 192 };
	
	position += sf::Vector2f{ border, border };

	backGroundRect.setSize({ size.x, size.y });
	backGroundRect.setFillColor(sf::Color{ 32, 32, 32 });
	backGroundRect.setOutlineColor(sf::Color{ 64, 64, 64 });
	backGroundRect.setOutlineThickness(border);
	backGroundRect.setPosition(position);

	p1Circle.setFillColor(ParticleType::getColor(p1));
	p1Circle.setRadius(5.0f);
	p1Circle.setPosition(position.x + 10.0f, position.y + graphSize.y - border - 20.0f);

	p2Circle.setFillColor(ParticleType::getColor(p2));
	p2Circle.setRadius(5.0f);
	p2Circle.setPosition(position.x + 25.0f, position.y + graphSize.y - border - 20.0f);

	
	infoBox.setSize({ 65, 40 });
	infoBox.setFillColor(sf::Color{ 0, 0, 32 });
	infoBox.setOutlineColor(sf::Color{ 0, 0, 128 });
	infoBox.setOutlineThickness(1);

	fontConsolas.loadFromFile("/home/marcel/eclipse-workspace/ParticleLife/src/Consolas.ttf");
	pointInfo.setFont(fontConsolas);
	pointInfo.setCharacterSize(14);
	pointInfo.setFillColor(sf::Color::White);

	calculateGrid();
	calculateVecGraphPointRatio();
	calculateVecPlotPoint();
}


Graph::~Graph()
{
}


sf::Vector2f Graph::graphSize = { 170.0f, 120.0f };


void Graph::calculateGrid()
{
	xDeltaGrid = 10.0f;
	yDeltaGrid = 0.5f;

	minX = 0.0f; 
	maxX = std::floor(ParticleType::getMaxForceDistance(p1) / xDeltaGrid) * xDeltaGrid;
	minY = -1.0f, maxY = 1.0f;
	xRatio = (size.x - 2 * gridBorder) / (maxX - minX);
	yRatio = (size.y - 2 * gridBorder) / (maxY - minY);


	xGrid.resize((int)(maxX / xDeltaGrid) + 1);
	yGrid.resize(5);

	for (size_t i = 0; i < xGrid.size(); i++)
	{
		xGrid[i].setSize({ 1.0f, yRatio * (maxY - minY) });
		xGrid[i].setFillColor(gridColor);
		//xGrid[i].setPosition({ i * xRatio * xDeltaGrid + position.x + gridBorder, position.y + gridBorder });

		xGrid[i].setPosition({ i * xRatio * xDeltaGrid + position.x + gridBorder, position.y + gridBorder });

	}
	for (size_t i = 0; i < yGrid.size(); i++)
	{
		yGrid[i].setSize({ xRatio * maxX, 1.0f });
		i != 2 ? yGrid[i].setFillColor(gridColor) : yGrid[i].setFillColor(gridZeroColor);
		yGrid[i].setPosition({ position.x + gridBorder, i * yRatio * yDeltaGrid + position.y + gridBorder });
	}
}


void Graph::calculateVecGraphPointRatio()
{
	for (size_t i = 0; i < vecGraphP1P2->size(); i++)
	{
		vecGraphP1P2->at(i)["distance"] = vecForceP1P2->at(i)["distance"] * xRatio;
		vecGraphP1P2->at(i)["force"] = vecForceP1P2->at(i)["force"] * yRatio;
	}
}


void Graph::calculateVecPlotPoint()
{
	vecPlotPoint = std::vector<sf::RectangleShape>(vecGraphP1P2->size());

	for (size_t i = 0; i < vecPlotPoint.size(); i++)
	{
		vecPlotPoint[i].setSize({ 1.0f, 1.0f });
		vecPlotPoint[i].setFillColor(sf::Color::White);
		vecPlotPoint[i].setOutlineColor(sf::Color::Green);
		vecPlotPoint[i].setOutlineThickness(3.0f);
		vecPlotPoint[i].setPosition({ position.x + gridBorder + vecGraphP1P2->at(i)["distance"],
			position.y + gridBorder + maxY * yRatio - vecGraphP1P2->at(i)["force"] });
	}

	vertexPlotLine.clear();
	vertexPlotLine.setPrimitiveType(sf::LineStrip);

	for (size_t i = 0; i < vecGraphP1P2->size(); i++)
	{
		vertexPlotLine.append(sf::Vertex({ position.x + gridBorder + vecGraphP1P2->at(i)["distance"],
			position.y + gridBorder + maxY * yRatio + -(vecGraphP1P2->at(i)["force"]) }));
	}
}

sf::Vector2f Graph::getGraphSize() { return graphSize; }
void Graph::setGraphSize(sf::Vector2f inGraphSize) { graphSize = inGraphSize; }


int Graph::getP1() const { return p1; }
int Graph::getP2() const { return p2; }

float Graph::getMinX() const { return minX; }
void Graph::setMinX(float inMinX) { minX = inMinX; }

float Graph::getMaxX() const { return maxX; }
void Graph::setMaxX(float inMaxX) { maxX = inMaxX; }

float Graph::getMinY() const { return minY; }
void Graph::setMinY(float inMinY) { minY = inMinY; }

float Graph::getMaxY() const { return maxY; }
void Graph::setMaxY(float inMaxY) { maxY = inMaxY; }


void Graph::updateVecForceP1P2()
{
	*vecForceP1P2 = force.getForceMatrixP1P2(p1, p2);
	*vecGraphP1P2 = *vecForceP1P2;
	calculateVecGraphPointRatio();
	calculateVecPlotPoint();
}


bool Graph::mouseAction(sf::Vector2i mousePos, bool buttonPressed, bool buttonReleased)
{
	// mouse position within graph rectangle?

	if (!(mousePos.x > position.x && mousePos.x < position.x + size.x && mousePos.y > position.y + gridBorder && mousePos.y < position.y + size.y - gridBorder))
	{
		hoverInfo = false;
		return false;
	}

	if (hoverInfo)
		pointSelected = -1;

	// go through all points and check, if mouse position hits a point
	// last point can only be selected for hoverInfo
	for (size_t i = 0; i < vecPlotPoint.size(); i++)
	{
		if (mousePos.x > vecPlotPoint[i].getPosition().x - 5.0f && mousePos.x < vecPlotPoint[i].getPosition().x + 5.0f
			&& mousePos.y > vecPlotPoint[i].getPosition().y - 5.0f && mousePos.y < vecPlotPoint[i].getPosition().y + 5.0f)
		{
			pointSelected = i;

			// last point cannot be selectd for editMode
			if (buttonPressed && i != vecPlotPoint.size() - 1)
			{
				hoverInfo = false;
				editMode = true;
			}
				
			break;
		}
	}

	// finish edit mode and calculate new distance & force for point
	if (buttonReleased)
	{		
		// if there was a point selected ...
		if (pointSelected >= 0)
		{
			// calculate new force ...
			*vecForceP1P2 = *vecGraphP1P2;
			for (auto& p : *vecForceP1P2)
			{
				p["distance"] = p["distance"] / xRatio;
				p["force"] = p["force"] / yRatio;
			}
			// ... and update force object
			force.setForceMatrixP1P2(p1, p2, *vecForceP1P2);
		}
		// reset all points to green
		std::for_each(vecPlotPoint.begin(), vecPlotPoint.end(), [](auto& i) { i.setOutlineColor(sf::Color::Green); });

		hoverInfo = false;
		editMode = false;
		pointSelected = -1;

		return true;
	}

	// drag point to new position
	if (editMode && pointSelected >= 0)
	{
		float xMinPos, xMaxPos;

		// point was previously selected: calculate if mousePos.x is between the neighbourpoints
		if (pointSelected == 0)
		{
			xMinPos = position.x + gridBorder;
			xMaxPos = vecPlotPoint[pointSelected + 1].getPosition().x;
		}
		else
		{
			xMinPos = vecPlotPoint[pointSelected - 1].getPosition().x;
			xMaxPos = vecPlotPoint[pointSelected + 1].getPosition().x;
		}

		// drag and drop functionality
		// if mousePos is within the range, calculate the new position of the point
		if (mousePos.x > xMinPos && mousePos.x < xMaxPos)
		{
			// point 0 should have always distance = 0
			if (pointSelected == 0)
				vecGraphP1P2->at(pointSelected)["distance"] = 0.0f;
			else
				vecGraphP1P2->at(pointSelected)["distance"] = mousePos.x - position.x - gridBorder;

			vecGraphP1P2->at(pointSelected)["force"] = -(mousePos.y - (position.y + gridBorder + maxY * yRatio));
			calculateVecPlotPoint();
			// set selected point to orange
			vecPlotPoint[pointSelected].setOutlineColor(sf::Color{ 255, 114, 0 });
		}
	}

	if (!editMode && pointSelected >= 0)
	{
		hoverInfo = true;
		std::stringstream ss;
		ss << std::fixed << std::setprecision(2) << "d=" << vecForceP1P2->at(pointSelected)["distance"] << "\nf=" << vecForceP1P2->at(pointSelected)["force"];
		if (vecForceP1P2->at(pointSelected)["force"] > 0.0f)
		{
			infoBox.setPosition(position.x + size.x - 70 - gridBorder, position.y + size.y - 45 - gridBorder);
			pointInfo.setPosition(position.x + size.x - 65 - gridBorder, position.y + size.y - 43 - gridBorder);
		}
		else
		{
			infoBox.setPosition(position.x + size.x - 70 - gridBorder, position.y + 5 + gridBorder);
			pointInfo.setPosition(position.x + size.x - 65 - gridBorder, position.y + 7 + gridBorder);
		}

		pointInfo.setString(ss.str());
	}
	
	if (pointSelected < 0)
		hoverInfo = false;
	
	return true;
}

void Graph::draw(sf::RenderTarget& target) const
{
	target.draw(backGroundRect);

	for (auto xg : xGrid)
		target.draw(xg);
	for (auto yg : yGrid)
		target.draw(yg);

	target.draw(p1Circle);
	target.draw(p2Circle);

	for (auto p : vecPlotPoint)
		target.draw(p);

	target.draw(vertexPlotLine);

	if (hoverInfo)
	{
		target.draw(infoBox);
		target.draw(pointInfo);
	}
}

