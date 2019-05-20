#pragma once

#include <vector>
#include "Force.h"
#include "Particle.h"
#include "ParticleType.h"

class GroupParticle
{
public:
	GroupParticle(int, int);
	virtual ~GroupParticle();

private:
	std::vector<std::unique_ptr<Particle>> group;

public:
	void add(int, int);
	void remove(int, int);
	void move();
	void draw(sf::RenderTarget& target) const;

	void calculateVelocity(const Force&);
	void operator()(Particle&, const Force&);
	void operator()(std::vector<std::unique_ptr<Particle>>::iterator, std::vector<std::unique_ptr<Particle>>::iterator, const Force&);

	std::vector<std::unique_ptr<Particle>>& getGroup();
};

