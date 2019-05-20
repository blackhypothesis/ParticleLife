#include "GroupParticle.h"


GroupParticle::GroupParticle(int inNumberType, int inPopulation)
{
	if (inNumberType > ParticleType::getNumberOfTypes())
		inNumberType = ParticleType::getNumberOfTypes();

	int typePopulation = inPopulation / inNumberType;
	for (int type = 0; type < inNumberType; type++)
	{
		for (int i = 0; i < typePopulation; i++)
		{
			group.push_back(std::make_unique<Particle>(type));
		}
	}
}


GroupParticle::~GroupParticle()
{
}


void GroupParticle::add(int inType, int inPopulation)
{
	for (int i = 0; i < inPopulation; i++)
	{
		group.push_back(std::make_unique<Particle>(inType));
	}
}

void GroupParticle::remove(int inType, int inPopulation)
{
	for (auto& p : group)
	{
		if (p->getType() == inType)
		{
			if (inPopulation == 0)
				break;
			p->setDead(true);
			inPopulation--;
		}
	}

	group.erase(std::remove_if(group.begin(), group.end(),[](std::unique_ptr<Particle> const& p)
	{
		return p->getDead();
	}), group.end());
}


void GroupParticle::move()
{
	for (auto& p : group)
	{
		p->move();
	}
}


void GroupParticle::draw(sf::RenderTarget& target) const
{
	for (auto& p : group)
	{
		p->draw(target);
	}
}


void GroupParticle::calculateVelocity(const Force& force)
{
	for (auto& p1 : group)
		(*this)(*p1, force);
}


// calculate force
void GroupParticle::operator()(Particle& p1, const Force& force)
{
	for (auto& p2 : group)
		if (p1.getId() != p2->getId())
		{
			// p1 gets moved by the force p1p2
			//p1.setVelocity(p1.getVelocity() + force.getForceVector(p1, *p2));

			// p2 gets moved by the force p1p2 (this is more logical)
			p2->setVelocity(p2->getVelocity() + force.getForceVector(p1, *p2));
		}
}

// calculate force - used for multithreading
void GroupParticle::operator()(std::vector<std::unique_ptr<Particle>>::iterator itStart, std::vector<std::unique_ptr<Particle>>::iterator itEnd, const Force& force)
{
	for (std::vector<std::unique_ptr<Particle>>::iterator itP1 = itStart; itP1 != itEnd; ++itP1)
	{
		for (auto& p2 : group)
			if ((*itP1)->getId() != p2->getId())
			{
				// p1 gets moved by the force p1p2
				//(*itP1)->setVelocity((*itP1)->getVelocity() + force.getForceVector(**itP1, *p2));

				// p2 gets moved by the force p1p2 (this is more logical)
				p2->setVelocity(p2->getVelocity() + force.getForceVector(**itP1, *p2));
			}
	}
}


std::vector<std::unique_ptr<Particle>>& GroupParticle::getGroup()
{
	return group;
}
