#include <iostream>
#include "Selector.h"



Selector::Selector() : position({ 0.0f, 0.0f }), radius(10.0f), gap(10)
{
}


Selector::~Selector()
{
}

void Selector::init()
{
	border = 2.0f;
	position += sf::Vector2f{ border, border };
	colorParticleSelected = { 128, 128, 128 };
	vecParticleSelector.clear();

	for (int i = 0; i < ParticleType::getNumberOfTypes(); i++)
	{
		vecParticleSelector.push_back(std::make_unique<sf::CircleShape>());
		vecParticleSelector[i]->setRadius(radius);
		vecParticleSelector[i]->setFillColor(ParticleType::getColor(i));
		vecParticleSelector[i]->setOutlineColor(sf::Color::Black);
		vecParticleSelector[i]->setOutlineThickness(4.0f);
		vecParticleSelector[i]->setPosition(position.x + gap + i * (2 * radius + gap), position.y + gap);
	}	
	
	backGroundRect.setSize({ 2 * gap + ParticleType::getNumberOfTypes() * (2 * radius + gap), 2 * (radius + gap) });
	backGroundRect.setFillColor(sf::Color{ 32, 32, 32 });
	backGroundRect.setOutlineColor(sf::Color{ 64, 64, 64 });
	backGroundRect.setOutlineThickness(border);
	backGroundRect.setPosition(position);
	backGroundRect.setPosition(position);
}


void Selector::setPosition(sf::Vector2f inPosition)
{
	position = inPosition;
	init();
}

std::vector<int> Selector::getParticleSelected() const
{
	std::vector<int> vecParticleSelected;
	for (size_t i = 0; i < vecParticleSelector.size(); i++)
	{
		if (vecParticleSelector[i]->getOutlineColor() == colorParticleSelected)
			vecParticleSelected.push_back(i);
	}
	std::cout << vecParticleSelected.size() << std::endl;
	for (auto& v : vecParticleSelected)
	{
		std::cout << v << std::endl;
	}
	return vecParticleSelected;
}


bool Selector::mouseAction(sf::Vector2i mousePos, bool buttonPressed, bool buttonReleased)
{
	bool isEdited = false;
	if (buttonPressed)
	{
		for (size_t i = 0; i < vecParticleSelector.size(); i++)
		{
			if (mousePos.x > vecParticleSelector[i]->getPosition().x && mousePos.x < vecParticleSelector[i]->getPosition().x + 2 * radius
				&& mousePos.y > vecParticleSelector[i]->getPosition().y && mousePos.y < vecParticleSelector[i]->getPosition().y + 2 * radius)
			{
				isEdited = true;

				if (vecParticleSelector[i]->getOutlineColor() == colorParticleSelected)
					vecParticleSelector[i]->setOutlineColor(sf::Color::Black);
				else
					vecParticleSelector[i]->setOutlineColor(colorParticleSelected);

				break;
			}
		}
	}
	return isEdited;
}


void Selector::draw(sf::RenderTarget& target) const
{
	target.draw(backGroundRect);

	for (auto& s : vecParticleSelector)
		target.draw(*s);
}