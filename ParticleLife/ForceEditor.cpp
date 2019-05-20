#include "ForceEditor.h"



ForceEditor::ForceEditor(Force& inForce, GroupParticle& inGroupParticle)
	: p1(0), p2(0), force(inForce), groupParticle(inGroupParticle), numberParticlesType(ParticleType::getNumberOfTypes() - 0), position({ 5.0f, 5.0f })
{
	initGraphs();
	selector.setPosition( position + sf::Vector2f{ 0.0f, 0.0f });

	// force max Distance 
	vecValSetter.push_back(std::make_unique<ValueSetter>());
	vecValSetter[0]->setParameterName("Force Dist.");
	vecValSetter[0]->setMinMaxValue(5.0f, 50.0f);
	vecValSetter[0]->setParameterValue(50.0f);
	vecValSetter[0]->setStep(1.0f);
	vecValSetter[0]->setPosition(position + sf::Vector2f{ 265.0f, 0.0f });

	// friction
	vecValSetter.push_back(std::make_unique<ValueSetter>());
	vecValSetter[1]->setParameterName("Friction");
	vecValSetter[1]->setMinMaxValue(-1.0f, 1.0f);
	vecValSetter[1]->setParameterValue(0.5f);
	vecValSetter[1]->setStep(0.05f);
	vecValSetter[1]->setPosition(position + sf::Vector2f{ 377.0f, 0.0f });

	// radius particle
	vecValSetter.push_back(std::make_unique<ValueSetter>());
	vecValSetter[2]->setParameterName("Radius");
	vecValSetter[2]->setMinMaxValue(1.0f, 15.0f);
	vecValSetter[2]->setParameterValue(3.0f);
	vecValSetter[2]->setStep(1.0f);
	vecValSetter[2]->setPosition(position + sf::Vector2f{ 489.0f, 0.0f });
}


ForceEditor::~ForceEditor()
{
}


void ForceEditor::initGraphs()
{
	float gap = 1.0f;

	// update the graphs, which are visible for the selected particle types
	vecGraph.clear();

	std::vector<int> vecParticlesSelected = selector.getParticleSelected();
	int numberSelectedParticles = vecParticlesSelected.size();
	float graphMaxSize = 170.0f;
	float graphRatio = 1.3f;
	sf::Vector2f graphSize = { 1200.0f / numberSelectedParticles, 1200.0f / graphRatio / numberSelectedParticles };

	if (graphSize.x > graphMaxSize)
		graphSize = { graphMaxSize, graphMaxSize / graphRatio };

	Graph::setGraphSize(graphSize);

	for (size_t i = 0; i < numberParticlesType * numberParticlesType; i++)
	{
		int p1 = i % numberParticlesType;
		int p2 = (int)std::floor(i / numberParticlesType);

		auto iP1 = std::find(vecParticlesSelected.begin(), vecParticlesSelected.end(), p1);
		auto iP2 = std::find(vecParticlesSelected.begin(), vecParticlesSelected.end(), p2);

		if (iP1 != vecParticlesSelected.end() && iP2 != vecParticlesSelected.end())
		{
			int posP1 = std::distance(vecParticlesSelected.begin(), iP1);
			int posP2 = std::distance(vecParticlesSelected.begin(), iP2);

			sf::Vector2f graphPos = { position.x + 0.0f + posP2 * (graphSize.x + gap), position.y + 45.0f + posP1 * (graphSize.y + gap) };
			vecGraph.push_back(std::make_unique<Graph>(graphPos, p1, p2, force));
		}
	}
}

void ForceEditor::updateVecForceP1P2()
{
	for (auto& g : vecGraph)
		g->updateVecForceP1P2();
}
	

bool ForceEditor::mouseAction(sf::Vector2i mousePos, bool buttonPressed, bool buttonReleased)
{
	bool edit = false;

	for (auto& g : vecGraph)
	{
		if (g->mouseAction(mousePos, buttonPressed, buttonReleased))
			edit = true;
	}

	if (selector.mouseAction(mousePos, buttonPressed, buttonReleased))
	{
		edit = true;
		initGraphs();
	}

	// max Distance of force
	if (vecValSetter[0]->mouseAction(mousePos, buttonPressed, buttonReleased))
	{
		force.setMaxDistance(vecValSetter[0]->getParameterValue());
		updateVecForceP1P2();
	}

	// Friction
	if (vecValSetter[1]->mouseAction(mousePos, buttonPressed, buttonReleased))
	{
		ParticleType::setAllFriction(vecValSetter[1]->getParameterValue());
	}

	// Radius
	if (vecValSetter[2]->mouseAction(mousePos, buttonPressed, buttonReleased))
	{
		ParticleType::setAllRadius(vecValSetter[2]->getParameterValue());
		for (auto& p : groupParticle.getGroup())
		{
			p->setRadius();
		}
	}

	return edit;
}


void ForceEditor::draw(sf::RenderTarget& target) const
{
	for (auto& g : vecGraph)
	{
		g->draw(target);
	}

	selector.draw(target);

	for (auto& s : vecValSetter)
		s->draw(target);
}
