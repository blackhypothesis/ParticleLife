#pragma once
#include <vector>
#include "SFML/Graphics.hpp"
#include "Force.h"


class Editor
{
public:
	sf::Color typeColor[8]{ sf::Color::Green, sf::Color::Blue, sf::Color::Red,  sf::Color::Yellow,
		sf::Color::White,sf::Color::Magenta,  sf::Color::Cyan, sf::Color::Black };

public:
	Editor();
	~Editor();

private:
	sf::Vector2f position;
	sf::Vector2f size;
	float border;
	sf::RectangleShape shape;
	enum editType { FORCE_SHIFT = 0 , FORCE_SCALE = 1 };
	enum changeType { INCREASE = 0, DECREASE = 1 };

	std::vector<float> p1p2Force;

	class eParticle
	{
	public:
		int type;
		sf::CircleShape shape;
		sf::Vector2f position;
		float radius;
		sf::Color color;

	};

	int numberParticleType;
	std::array<std::array<eParticle, 2>, 7> pType;
	eParticle p1;
	eParticle p2;

	class Graph
	{
	public:
		sf::RectangleShape shape;
		sf::Vector2f position;
		sf::RectangleShape xAxis;
		sf::RectangleShape yAxis;
		sf::Vector2f posXAxis;
		sf::Vector2f posYAxis;
		std::array<sf::RectangleShape, 186> forcePlot;

		void calculateForcePlot(const std::vector<float>&);
	};

	Graph g;

	class EditParam
	{
	public:
		sf::RectangleShape shape;
		sf::Vector2f position;
		sf::Vector2f size;
		sf::Color color;
	};

	std::array<std::array<EditParam, 2>, 2 > ep;


public:
	sf::Vector2f getPosition() const;
	void setPosition(sf::Vector2f);

	sf::Vector2f getSize() const;

	std::vector<float> getp1p2Force() const;

	bool mouseAction(sf::Vector2i, Force&);
	void draw(sf::RenderTarget&) const;
};

