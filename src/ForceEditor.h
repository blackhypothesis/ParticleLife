#pragma once

#include <vector>
#include "Graph.h"
#include "Selector.h"
#include "ValueSetter.h"
#include "ParticleType.h"
#include "Force.h"
#include "GroupParticle.h"

class ForceEditor
{
public:
	ForceEditor(Force&, GroupParticle&);
	virtual ~ForceEditor();
	
private:
	int p1, p2;
	Force& force;
	GroupParticle& groupParticle;
	size_t numberParticlesType;
	sf::Vector2f position;
	std::vector<std::unique_ptr<Graph>> vecGraph;
	Selector selector;
	std::vector<std::unique_ptr<ValueSetter>> vecValSetter;

	void initGraphs();

public:
	void updateVecForceP1P2();

	bool mouseAction(sf::Vector2i, bool, bool);
	void draw(sf::RenderTarget&) const;

};

