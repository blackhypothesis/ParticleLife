#pragma once

#include <vector>

#include <SFML/Graphics.hpp>
#include <memory>
#include "Color.h"


class ParticleType
{
public:
	ParticleType() = delete;

private:
	static int numberType;
	static std::vector<sf::Color> color;
	static std::vector<float> maxForceDistance;
	static std::vector<float> radius;
	static std::vector<float> friction;

public:
	static int getNumberOfTypes();
	static sf::Color getColor(int);

	static void setAllMaxForceDistance(float);
	static float getMaxForceDistance(int);

	static void setAllRadius(float);
	static float getRadius(int);

	static void setAllFriction(float);
	static float getFriction(int);
};

