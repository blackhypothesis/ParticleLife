#include <iostream>
#include "Force.h"



Force::Force() : numberParticlesType(ParticleType::getNumberOfTypes()), numberDistancePoints(5), numberDistanceSteps(200)
{

	maxDistance = 50.0f;
	rnd.setSeed();

	distanceQuantisation = maxDistance / numberDistanceSteps;

	calculateForceMatrix();
	calculateForceVector();
}


Force::~Force()
{
}


void Force::calculateForceMatrix()
{
	// initialize forceMatrix (p1, p2, forcePoints with(distance and force))

	forceMatrix.clear();

	for (size_t p1 = 0; p1 < numberParticlesType; p1++)
	{
		forceMatrix.push_back(std::vector<std::vector<std::map<std::string, float>>>());
		//float maxDistance = ParticleType::getMaxForceDistance(p1);

		for (size_t p2 = 0; p2 < numberParticlesType; p2++)
		{
			forceMatrix[p1].push_back(std::vector<std::map<std::string, float>>());

			for (size_t point = 0; point < numberDistancePoints; point++)
			{
				forceMatrix[p1][p2].push_back(std::map<std::string, float>());

				forceMatrix[p1][p2][point].insert(std::make_pair("distance", 0.0f));
				forceMatrix[p1][p2][point].insert(std::make_pair("force", 0.0f));
			}

			float minForce = -0.5f;
			float maxForce = 0.5f;

			// start with distance = 0.0f
			forceMatrix[p1][p2][0]["distance"] = 0.0f;
			// force is negative in order the particles repel each other 
			forceMatrix[p1][p2][0]["force"] = rnd(minForce, -0.1f);

			for (size_t point = 1; point < numberDistancePoints - 1; point++)
			{
				forceMatrix[p1][p2][point]["distance"] = forceMatrix[p1][p2][point - 1]["distance"] + rnd(maxDistance / numberDistancePoints / 2.0f , maxDistance / (numberDistancePoints - 3.0f));
				if (forceMatrix[p1][p2][point]["distance"] > maxDistance - (numberDistancePoints - point) * maxDistance / numberDistancePoints / 2.0f)
					forceMatrix[p1][p2][point]["distance"] = maxDistance - (numberDistancePoints - point) * maxDistance / numberDistancePoints / 2.0f;
				forceMatrix[p1][p2][point]["force"] = rnd(minForce, maxForce);
			}

			// reset the second last force to 0.0f
			//forceMatrix[p1][p2][numberDistancePoints - 2]["force"] = 0.0f;

			// add a last point with distance = maxDistance and force = 0
			forceMatrix[p1][p2][numberDistancePoints - 1]["distance"] = maxDistance;
			forceMatrix[p1][p2][numberDistancePoints - 1]["force"] = 0.0f;
		}
	}
}


void Force::calculateForceVector()
{
	// use forceMatrix to fill forceVector with precalculated forces for each quantized distance

	forceVector.clear();

	for (size_t p1 = 0; p1 < numberParticlesType; p1++)
	{
		forceVector.push_back(std::vector<std::vector<float>>());

		for (size_t p2 = 0; p2 < numberParticlesType; p2++)
		{
			forceVector[p1].push_back(std::vector<float>());

			for (int qDistance = 0; qDistance < numberDistanceSteps; qDistance++)
				forceVector[p1][p2].push_back(float{});

			// calculate where the points are on the quantized distance
			std::vector<int> vecDistPoint(numberDistancePoints);
			for (size_t distancePoint = 0; distancePoint < numberDistancePoints; distancePoint++)
			{
				vecDistPoint[distancePoint] = (int)round(forceMatrix[p1][p2][distancePoint]["distance"] / distanceQuantisation);
			}

			// loop through quantized distance and calculate for each distance the force
			unsigned int i = 1;
			for (int qDistance = 0; qDistance < numberDistanceSteps; qDistance++)
			{
				if (i < vecDistPoint.size() && qDistance > vecDistPoint[i])
					i++;

				if (i < vecDistPoint.size())
				{
					float fDistance = (float)qDistance * distanceQuantisation;

					float distance0 = forceMatrix[p1][p2][i - 1]["distance"];
					float distance1 = forceMatrix[p1][p2][i]["distance"];
					float force0 = forceMatrix[p1][p2][i - 1]["force"];
					float force1 = forceMatrix[p1][p2][i]["force"];

					forceVector[p1][p2][qDistance] = (force0 - force1) * ((distance1 - fDistance) / (distance1 - distance0)) + force1;
				}
				else
					forceVector[p1][p2][qDistance] = 0.0f;
			}
		}
	}
}


void Force::randomize()
{
	calculateForceMatrix();
	calculateForceVector();
}


void Force::shift(int pType1, int pType2, float delta)
{
	for (int distance = 0; distance < numberDistanceSteps; distance++)
		forceVector[pType1][pType2][distance] += delta;
}


void Force::scale(int pType1, int pType2, float scale)
{
	for (int distance = 0; distance < numberDistanceSteps; distance++)
		forceVector[pType1][pType2][distance] *= scale;
}

void Force::setMaxDistance(float inMaxDistance)
{
	for (size_t p1 = 0; p1 < numberParticlesType; p1++)
	{
		for (size_t p2 = 0; p2 < numberParticlesType; p2++)
		{
			for (size_t point = 0; point < numberDistancePoints; point++)
			{
				forceMatrix[p1][p2][point]["distance"] = forceMatrix[p1][p2][point]["distance"] * inMaxDistance / maxDistance;
				forceMatrix[p1][p2][point]["force"] = forceMatrix[p1][p2][point]["force"] * inMaxDistance / maxDistance;
			}
		}
	}

	maxDistance = inMaxDistance;
	distanceQuantisation = maxDistance / numberDistanceSteps;

	calculateForceVector();
}


std::vector<std::map<std::string, float>>& Force::getForceMatrixP1P2(int p1, int p2)
{
	return forceMatrix[p1][p2];
}


void Force::setForceMatrixP1P2(int p1, int p2, std::vector<std::map<std::string, float>>& vecForcePoint)
{
	forceMatrix[p1][p2] = vecForcePoint;
	calculateForceVector();
}


std::vector<float> Force::getForceVectorP1P2(int pType1, int pType2) const
{
	return forceVector[pType1][pType2];
}


sf::Vector2f Force::getForceVector(const Particle& p1, const Particle& p2) const
{
	float deltaX = p1.getPosition().x - p2.getPosition().x;
	float deltaY = p1.getPosition().y - p2.getPosition().y;

	if (deltaX == 0 && deltaY == 0)
		return { 0.0f, 0.0f };


	// torus
	if (deltaX > 750.0f) { deltaX = -1500.0f + deltaX; }
	if (deltaX < -750.0f) { deltaX = 1500.0f + deltaX; }
	if (deltaY > 500.0f) { deltaY = -1000.0f + deltaY; }
	if (deltaY < -500.0f) { deltaY = 1000.0f + deltaY; }

	float distance = std::sqrtf(deltaX * deltaX + deltaY * deltaY);

	// scale distance to particle radius as unit
	int qDistance = (int)(distance / p1.getRadius() / distanceQuantisation);

	float force = 0.0f;
	if (qDistance < numberDistanceSteps)
		force = forceVector[p1.getType()][p2.getType()][qDistance];

	return { deltaX / distance * force, deltaY / distance * force };
}