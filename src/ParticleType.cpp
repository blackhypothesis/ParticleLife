#include "ParticleType.h"


int ParticleType::numberType = 8;

std::vector<sf::Color> ParticleType::color = { sf::Color::Green, sf::Color::Blue, sf::Color::Red,  sf::Color::Yellow,
		sf::Color::White, sf::Color::Magenta,  sf::Color::Cyan, sf::Color {215, 129, 126} };

std::vector<float> ParticleType::maxForceDistance = { 50.0f, 50.0f, 50.0f, 50.0f, 50.0f, 50.0f, 50.0f, 50.0f };
std::vector<float> ParticleType::radius = { 3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f, 3.0f };
std::vector<float> ParticleType::friction = { 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f };

int ParticleType::getNumberOfTypes() { return numberType; }
sf::Color ParticleType::getColor(int inType) { return color[inType]; }

void ParticleType::setAllMaxForceDistance(float inMaxForceDistance)
{
	std::fill(maxForceDistance.begin(), maxForceDistance.end(), inMaxForceDistance);
}

float ParticleType::getMaxForceDistance(int inType) { return maxForceDistance[inType]; }


void ParticleType::setAllRadius(float inRadius)
{
	std::fill(radius.begin(), radius.end(), inRadius);
}

float ParticleType::getRadius(int inType) { return radius[inType]; }


void ParticleType::setAllFriction(float inFriction)
{
	std::fill(friction.begin(), friction.end(), inFriction);
}


float ParticleType::getFriction(int inType) { return friction[inType]; }

