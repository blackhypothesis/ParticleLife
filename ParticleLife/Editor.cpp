#include <iostream>

#include "Editor.h"


Editor::Editor()
{
	position = { 0.0f, 0.0f };
	size = { 200.0f, 170.0f };
	border = 2.0f;

	shape.setSize({ size.x - 2 * border, size.y - 2 * border });
	shape.setFillColor(sf::Color::Black);
	shape.setOutlineColor(sf::Color{ 128, 0, 128 });
	shape.setOutlineThickness(border);

	p1p2Force = std::vector<float>(500);

	numberParticleType = 7;
	pType = std::array<std::array<eParticle, 2>, 7>();

	for (size_t x = 0; x < pType.size(); x++)
	{
		for (size_t y = 0; y < pType[x].size(); y++)
		{
			pType[x][y].type = x;
			pType[x][y].color = typeColor[x];
			pType[x][y].position = { 10.0f + (float)x * 22.0f, 5.0f + (float)y * 20.0f };
			pType[x][y].radius = 8.0f;
			pType[x][y].shape.setRadius(pType[x][y].radius);
			pType[x][y].shape.setFillColor(typeColor[x]);
		}
	}

	p1.type = 0;
	p1.color = typeColor[0];
	p1.position = { 10.0f, 50.0f };
	p1.radius = 8.0f;
	p1.shape.setRadius(p1.radius);
	p1.shape.setFillColor(typeColor[0]);

	p2.type = 0;
	p2.color = typeColor[0];
	p2.position = { 10.0f, 70.0f };
	p2.radius = 8.0f;
	p2.shape.setRadius(p2.radius);
	p2.shape.setFillColor(typeColor[0]);

	g.position = { 5.0f, 100.0f };
	g.shape.setFillColor(sf::Color{ 96, 96, 96 });
	g.shape.setSize({ 186.0f, 63.0f });
	
	g.posXAxis = { 5.0f, 132.0f };
	g.xAxis.setFillColor(sf::Color{ 192, 192, 192 });
	g.xAxis.setSize({ 186.0f, 1.0f });

	g.posYAxis = { 5.0f, 100.0f };
	g.yAxis.setFillColor(sf::Color{ 192, 192, 192 });
	g.yAxis.setSize({ 1.0f, 63.0f });

	g.forcePlot = std::array<sf::RectangleShape, 186>();

	g.calculateForcePlot(p1p2Force);

	for (size_t x = 0; x < ep.size(); x++)
	{
		for (size_t y = 0; y < ep[x].size(); y++)
		{
			ep[x][y].position = { 40.0f + (float)x * 30, 50.0f + (float)y * 20.0f };
			ep[x][y].size = { 18.0f, 18.0f };
			ep[x][y].shape.setSize(ep[x][y].size);
			ep[x][y].shape.setFillColor(sf::Color::White);
		}
	}
	setPosition(position);
}


Editor::~Editor()
{
}

sf::Vector2f Editor::getPosition() const { return position; }


void Editor::setPosition(sf::Vector2f inPosition)
{
	position = inPosition;
	shape.setPosition(position + sf::Vector2f{ border, border });
	for (size_t x = 0; x < pType.size(); x++)
		for (size_t y = 0; y < pType[x].size(); y++)
			pType[x][y].shape.setPosition({ position + pType[x][y].position });
	
	p1.shape.setPosition({ position + p1.position });
	p2.shape.setPosition({ position + p2.position });
	g.shape.setPosition(position + g.position);
	g.xAxis.setPosition(position + g.posXAxis);
	g.yAxis.setPosition(position + g.posYAxis);

	for (size_t x = 0; x < ep.size(); x++)
		for (size_t y = 0; y < ep[x].size(); y++)
			ep[x][y].shape.setPosition({ position + ep[x][y].position });

	for (auto& a : g.forcePlot)
		a.setPosition(position + a.getPosition());
}


sf::Vector2f Editor::getSize() const { return size; }

std::vector<float> Editor::getp1p2Force() const { return p1p2Force; }

bool Editor::mouseAction(sf::Vector2i mousePos, Force& force)
{
	// select source (p1) and target (p2) particle
	for (size_t x = 0; x < pType.size(); x++)
	{
		for (size_t y = 0; y < pType[x].size(); y++)
		{
			if (mousePos.x > position.x + pType[x][y].position.x && mousePos.x < position.x + pType[x][y].position.x + pType[x][y].radius * 2
				&&  mousePos.y > position.y + pType[x][y].position.y && mousePos.y < position.y + pType[x][y].position.y + pType[x][y].radius * 2)
			{
				if (y == 0)
				{
					p1.type = x;
					p1.color = typeColor[x];
					p1.shape.setFillColor(typeColor[p1.type]);
				}
				else if (y == 1)
				{
					p2.type = x;
					p2.color = typeColor[x];
					p2.shape.setFillColor(typeColor[p2.type]);
				}
				// update graph
				p1p2Force = force.getForceVectorP1P2(p1.type, p2.type);
				g.calculateForcePlot(p1p2Force);
				setPosition(position);
			}
		}
	}

	for (size_t x = 0; x < ep.size(); x++)
	{
		for (size_t y = 0; y < ep[x].size(); y++)
		{
			if (mousePos.x > position.x + ep[x][y].position.x && mousePos.x < position.x + ep[x][y].position.x + ep[x][y].size.x
				&& mousePos.y > position.y + ep[x][y].position.y && mousePos.y < position.y + ep[x][y].position.y + ep[x][y].size.y)
			{
				if (x == FORCE_SHIFT && y == INCREASE)
				{
					//std::cout << "p1 " << p1.type << "  p2 " << p2.type << "   editType " << x << "  chanetType " << y << std::endl;
					force.shift(p1.type, p2.type, 0.1f);
				}
				else if (x == FORCE_SHIFT && y == DECREASE)
				{
					//std::cout << "p1 " << p1.type << "  p2 " << p2.type << "   editType " << x << "  chanetType " << y << std::endl;
					force.shift(p1.type, p2.type, -0.1f);
				}
				else if (x == FORCE_SCALE && y == INCREASE)
					force.scale(p1.type, p2.type, 1.05f);
				else if (x == FORCE_SCALE && y == DECREASE)
					force.scale(p1.type, p2.type, 0.95f);
			}
			// update graph
			p1p2Force = force.getForceVectorP1P2(p1.type, p2.type);
			g.calculateForcePlot(p1p2Force);
			setPosition(position);
		}
	}
	if (mousePos.x > position.x && mousePos.x < position.x + size.x && mousePos.y > position.y && mousePos.y < position.y + size.y)
		return true;
	else
		return false;
}


void Editor::draw(sf::RenderTarget& target) const
{
	target.draw(shape);
	
	for (size_t x = 0; x < pType.size(); x++)
	{
		for (size_t y = 0; y < pType[x].size(); y++)
			target.draw(pType[x][y].shape);
	}
	target.draw(p1.shape);
	target.draw(p2.shape);
	target.draw(g.shape);
	target.draw(g.xAxis);
	target.draw(g.yAxis);

	for (size_t x = 0; x < ep.size(); x++)
		for (size_t y = 0; y < ep[x].size(); y++)
			target.draw(ep[x][y].shape);

	for (size_t x = 0; x < g.forcePlot.size(); x++)
		target.draw(g.forcePlot[x]);

}


void Editor::Graph::calculateForcePlot(const std::vector<float>& p1p2f)
{
	// transform the 500 values from 
	int radiusMarker = (int)((float)forcePlot.size() / 500.0f * 10.0f);
	int radius10Marker = (int)((float)forcePlot.size() / 500.0f * 100.0f);
	int radius20Marker = (int)((float)forcePlot.size() / 500.0f * 200.0f);
	int radius30Marker = (int)((float)forcePlot.size() / 500.0f * 300.0f);
	int radius40Marker = (int)((float)forcePlot.size() / 500.0f * 400.0f);


	for (size_t x = 0; x < forcePlot.size(); x++)
	{
		forcePlot[x].setPosition({ (float)x + 5.0f, -10 * p1p2f[(int)((float)500 / (float)forcePlot.size() * (float)x)] + 132.0f });

		forcePlot[x].setFillColor(sf::Color::White);
		forcePlot[x].setSize({ 1.0f, 1.0f });

		if (x == radiusMarker)
		{
			forcePlot[x].setFillColor(sf::Color::Green);
			forcePlot[x].setSize({ 8.0f, 8.0f });
		}
		if (x == radius10Marker || x == radius20Marker || x == radius30Marker || x == radius40Marker)
		{
			forcePlot[x].setFillColor(sf::Color::Red);
			forcePlot[x].setSize({ 8.0f, 8.0f });
		}
	}
}