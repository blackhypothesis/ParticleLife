#include "Particle.h"


Particle::Particle()
{
	type = 0;
	dead = false;
}

Particle::Particle(int inType) : type(inType)
{
	id = counter++;
	dead = false;

	rnd.setSeed();
	// 0 < x < 1200, 0 < y < 800
	position = { rnd(0.0f, 1500.0f), rnd(0.0f, 1000.0f) };
	// between -1 and 1
	velocity = { rnd(-1.0f, 1.0f), rnd(-1.0f, 1.0f) };
	velocity = velocity * ParticleType::getRadius(type);

	shapeParticle.setRadius(ParticleType::getRadius(type));
	shapeParticle.setFillColor(ParticleType::getColor(type));
	shapeParticle.setPosition(position);
}


Particle::Particle(int inType, sf::Vector2f inPosition)
{
	id = counter++;
	dead = false;
	position = inPosition;
	velocity = { rnd(-1.0f, 1.0f), rnd(-1.0f, 1.0f) };
	velocity = velocity * ParticleType::getRadius(type);

	shapeParticle.setRadius(ParticleType::getRadius(type));
	shapeParticle.setFillColor(ParticleType::getColor(type));
	shapeParticle.setPosition(position);
}


Particle::~Particle()
{
}

int Particle::counter = 0;

int Particle::getId() const { return id; }
int Particle::getType() const { return type; }

bool Particle::getDead() const { return dead; }
void Particle::setDead(bool inDead) { dead = inDead; }

float Particle::getRadius() const { return ParticleType::getRadius(type); }

void Particle::setRadius() { shapeParticle.setRadius(ParticleType::getRadius(type)); }


sf::Vector2f Particle::getPosition() const { return position; }

void Particle::setPosition(sf::Vector2f inPosition) { position = inPosition; }


sf::Vector2f Particle::getVelocity() const { return velocity; }
void Particle::setVelocity(sf::Vector2f inVelocity) { velocity = inVelocity; }

float Particle::getFriction() const { return ParticleType::getFriction(type); }


void Particle::move()
{

	
	// reflection
    /*
	sf::Vector2f posSave = position;
	float mult = 4.0f;

	position = position + velocity;
	if (position.x < 0.0f + ParticleType::getRadius(type) || position.x > 1500.0f - ParticleType::getRadius(type))
	{
		velocity.x = -velocity.x * mult;
		velocity.y = velocity.y * mult;
		position = posSave;
	}
	if (position.y < 0.0f + ParticleType::getRadius(type) || position.y > 1000.0f - ParticleType::getRadius(type))
	{
		velocity.x = velocity.x * mult;
		velocity.y = -velocity.y * mult;
		position = posSave;
	}
	shapeParticle.setPosition(position);
	*/

	// torus

	position = position + velocity;
	if (position.x < 0.0f) { position.x += 1500.0f; }
	if (position.x > 1500.0f) { position.x -= 1500.0f; }
	if (position.y < 0.0f) { position.y += 1000.0f; }
	if (position.y > 1000.0f) { position.y -= 1000.0f; }
	shapeParticle.setPosition(position);


	// Friction
	velocity.x = velocity.x * ParticleType::getFriction(type);
	velocity.y = velocity.y * ParticleType::getFriction(type);
}

void Particle::draw(sf::RenderTarget& target) const
{
	target.draw(shapeParticle);
}

