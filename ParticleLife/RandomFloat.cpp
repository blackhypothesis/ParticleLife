#include "RandomFloat.h"


RandomFloat::RandomFloat() : lower(-1.0f), upper(1.0f)
{
	setSeed();
	gen.seed(seed);
}


RandomFloat::RandomFloat(unsigned long long inSeed) : RandomFloat()
{
	seed = inSeed;
	gen.seed(seed);
}
 

void RandomFloat::setLower(float inLower) { lower = inLower; }
void RandomFloat::setUpper(float inUpper) { upper = inUpper; }

RandomFloat::~RandomFloat()
{
}

float RandomFloat::operator()()
{
	lastRand = lower + (float)(gen() / (float)((gen.max() - gen.min()) / (upper - lower)));
	return lastRand;
}

float RandomFloat::operator()(float inLower, float inUpper)
{
	lower = inLower;
	upper = inUpper;

	lastRand = lower + (float)(gen() / (float)((gen.max() - gen.min()) / (upper - lower)));
	return lastRand;
}


void RandomFloat::setSeed()
{
	seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	gen.seed(seed);
}


void RandomFloat::setSeed(unsigned long long inSeed)
{
	seed = inSeed;
	gen.seed(seed);
}


unsigned long long RandomFloat::getSeed() const
{
	return seed;
}


float RandomFloat::getLastRand() const { return lastRand; }