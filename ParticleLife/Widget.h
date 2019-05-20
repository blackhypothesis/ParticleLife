#pragma once

#include <variant>
#include <vector>

#include <SFML/Graphics.hpp>


class Widget
{
public:
	Widget();
	~Widget();

protected:
	const enum action { SET_VALUE, };
private:
	sf::Vector2f position;
	std::variant<int, float> i, f;

public:
	virtual bool mouseAction(sf::Vector2i, bool, bool) = 0;
	virtual void draw(sf::RenderTarget&) const = 0;
};

