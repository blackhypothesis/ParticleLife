#pragma once

#include <vector>

#include <SFML/Graphics.hpp>
#include "ParticleType.h"


class Selector
{
public:
	Selector();
	virtual ~Selector();

private:
	sf::Vector2f position;
	float border;
	float radius;
	float gap;
	sf::RectangleShape backGroundRect;
	std::vector<std::unique_ptr<sf::CircleShape>> vecParticleSelector;
	sf::Color colorParticleSelected;

	void init();

public:
	void setPosition(sf::Vector2f);

	std::vector<int> getParticleSelected() const;

	bool mouseAction(sf::Vector2i, bool, bool);
	void draw(sf::RenderTarget&) const;

};

