#pragma once
#include <chrono>
#include <random>

class RandomFloat
{
public:
	RandomFloat();
	RandomFloat(unsigned long long);
	~RandomFloat();

private:
	unsigned long long seed;
	float lower;
	float upper;
	float lastRand;
	
public:
	std::mt19937 gen;
	float operator()();
	float operator()(float, float);

	void setSeed(unsigned long long);
	void setSeed();

	void setLower(float);
	void setUpper(float);

	unsigned long long getSeed() const;
	float getLastRand() const;
};

