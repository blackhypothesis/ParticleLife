#pragma once

#include <array>
#include <vector>
#include "RandomFloat.h"
#include "Particle.h"
#include "ParticleType.h"



class Force
{
public:
	Force();
	virtual ~Force();

private:
	RandomFloat rnd;
	const size_t numberParticlesType;
	const size_t numberDistancePoints;   // used in forceMatrix 
	const int numberDistanceSteps;       // to calculate force = f(distance) in calculatedForceVector
	float distanceQuantisation;
	float maxDistance;
	std::vector<std::vector<std::vector<std::map<std::string, float>>>> forceMatrix;
	std::vector<std::vector<std::vector<float>>> forceVector;

public:
	const float windowXsize = 2200.0f;
	const float windowYsize = 1200.0f;
	void zeroForceMatrix();
	void calculateForceMatrix();
	void calculateForceVector();

	void zero();
	void randomize();
	void shift(int, int, float);
	void scale(int, int, float);

	void setMaxDistance(float);

	std::vector<std::map<std::string, float>>& getForceMatrixP1P2(int, int);
	void setForceMatrixP1P2(int, int, std::vector<std::map<std::string, float>>&);

	std::vector<float> getForceVectorP1P2(int, int) const;
	sf::Vector2f getForceVector(const Particle&, const Particle&) const;
};

