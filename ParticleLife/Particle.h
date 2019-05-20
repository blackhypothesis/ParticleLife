#pragma once

#include <SFML/Graphics.hpp>
#include "RandomFloat.h"
#include "ParticleType.h"

class Particle
{
public:
	Particle();
	Particle(int);
	Particle(int, sf::Vector2f);
	virtual ~Particle();


private:
	RandomFloat rnd;
	static int counter;
	int id;
	int type;
	bool dead;
	sf::CircleShape shapeParticle;
	sf::Vector2f position;
	sf::Vector2f velocity;

public:
	int getId() const;
	int getType() const;

	bool getDead() const;
	void setDead(bool);

	float getRadius() const;
	void setRadius();

	sf::Vector2f getPosition() const;
	void setPosition(sf::Vector2f);

	sf::Vector2f getVelocity() const;
	void setVelocity(sf::Vector2f);

	float getFriction() const;

	void move();

	void draw(sf::RenderTarget&) const;
};



