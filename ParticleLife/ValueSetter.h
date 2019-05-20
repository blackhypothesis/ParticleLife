#pragma once

#include <iomanip>
#include <sstream>
#include <vector>

#include <SFML/Graphics.hpp>
#include "ParticleType.h"



class ValueSetter
{
public:
	ValueSetter();
	virtual ~ValueSetter();

private:
	sf::Vector2f position;
	sf::RectangleShape backGroundRect;

	sf::CircleShape particleType;
	std::vector<std::unique_ptr<sf::CircleShape>> vecUpDown;

	sf::Font fontConsolas;
	sf::Text parameter;
	std::string sParameterName;
	sf::Text value;
	sf::String sValue;

	float minValue, maxValue, parameterValue, step;
	std::stringstream ss;

public:
	void setPosition(sf::Vector2f);
	void setParameterName(std::string);
	void setMinMaxValue(float, float);
	void setParameterValue(float);
	float getParameterValue() const;
	void setStep(float);

	bool mouseAction(sf::Vector2i, bool, bool);
	void draw(sf::RenderTarget&) const;

 };

