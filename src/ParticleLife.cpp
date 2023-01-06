#include <iostream>
#include <thread>

#include <SFML/Graphics.hpp>

#include "RandomFloat.h"
#include "GroupParticle.h"
#include "Force.h"
#include "ForceEditor.h"
#include "Graph.h"
#include "Color.h"

/*
Compiler optimisation:
Solution -> Properties -> Configuration Properties -> C/C++ -> General -> Debug Information Format -> Program Database (/Zi)
Solution -> Properties -> Configuration Properties -> C/C++ -> Optimization -> Optimization -> Maximum Optimization
Solution -> Properties -> Configuration Properties -> C/C++ -> Optimization -> Whole Program Optimization -> Yes(/GL)
Solution -> Properties -> Configuration Properties -> C/C++ -> Code Generation -> Basic Runtime Checks -> Default
*/

void printNumberParticles(GroupParticle& group)
{
	std::array<int, 8> arrayT = { 0 };
	int total = 0;
	
	for (auto& g : group.getGroup())
	{
		arrayT[g->getType()]++;
		total++;
	}
	for (size_t i = 0; i < arrayT.size(); i++)
	{
		std::cout << "Type  " << i << " -> " << arrayT[i] << std::endl;
	}
	std::cout << "Total " << total << std::endl;
	std::cout << std::endl;

}


int main()
{
	const unsigned int windowX = 1500;
	const unsigned int windowY = 1000;

	sf::Font font;

	if (!font.loadFromFile("/home/marcel/eclipse-workspace/ParticleLife/src/Consolas.ttf"))
	{
		std::cerr << "cannot open font consolas\n";
		return EXIT_FAILURE;
	}

	sf::Text text;
	text.setFont(font);


	sf::Vector2i mousePos;
	bool buttonPressed;
	bool buttonReleased;

	sf::Clock clock;
	bool multiThreaded = true;
	unsigned int numThreads = std::thread::hardware_concurrency();

	sf::RenderWindow window;
	window.create(sf::VideoMode(windowX, windowY), "Particle Life");
	window.setFramerateLimit(60);

	int nFPS = 60;

	bool drawEditor = true;
	bool pause = false;

	int selectedType = 0;
	bool allTypes = false;

	int groupSize = 0;
	GroupParticle group(1, groupSize);
	
	Force force;
	ForceEditor fe(force, group);

	while (window.isOpen())
	{
		sf::Time elapsed = clock.restart();

		sf::Event event;

		while (window.pollEvent(event))
		{
			//std::cout << "Event:  " << event.type << std::endl;

			if (event.type == sf::Event::Closed)
			{
				window.close();
			}

			if (event.type == sf::Event::MouseMoved)
				mousePos = { event.mouseMove.x, event.mouseMove.y };

			buttonPressed = false;
			buttonReleased = false;

			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					buttonPressed = true;
					mousePos = { event.mouseButton.x, event.mouseButton.y };
				}
				else
					buttonPressed = false;
			}

			if (event.type == sf::Event::MouseButtonReleased)
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					buttonReleased = true;
					mousePos = { event.mouseButton.x, event.mouseButton.y };
				}

			if (drawEditor)
			{
				// editor.mouseAction(mousePos, force);
				fe.mouseAction(mousePos, buttonPressed, buttonReleased);
			}

			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::A)
					allTypes = !allTypes;
				if (event.key.code == sf::Keyboard::Num0)
					selectedType = 0;
				if (event.key.code == sf::Keyboard::Num1)
					selectedType = 1;
				if (event.key.code == sf::Keyboard::Num2)
					selectedType = 2;
				if (event.key.code == sf::Keyboard::Num3)
					selectedType = 3;
				if (event.key.code == sf::Keyboard::Num4)
					selectedType = 4;
				if (event.key.code == sf::Keyboard::Num5)
					selectedType = 5;
				if (event.key.code == sf::Keyboard::Num6)
					selectedType = 6;
				if (event.key.code == sf::Keyboard::Num7)
					selectedType = 7;
				if (event.key.code == sf::Keyboard::Up)
				{
					if (allTypes)
					{
						for (int i = 0; i <= selectedType; i++)
							group.add(i, 5);
					}
					else
						group.add(selectedType, 5);

					printNumberParticles(group);
				}
					

				if (event.key.code == sf::Keyboard::Down)
				{
					if (allTypes)
					{
						for (int i = 0; i < 8; i++)
							group.remove(i, 5);
					}
					else
						group.remove(selectedType, 5);

					printNumberParticles(group);
				}


				if (event.key.code == sf::Keyboard::R)
				{
					force.randomize();
					// editor.mouseAction(mousePos, force);
					fe.updateVecForceP1P2();
				}

				if (event.key.code == sf::Keyboard::Z)
				{
					force.zero();
					fe.updateVecForceP1P2();
				}


				// reposition all particles from the center
				if (event.key.code == sf::Keyboard::S)
					for (auto& p : group.getGroup())
					{
						RandomFloat rnd;
						float f = 800.0f;
						float pos = 200.0f;
						p->setPosition({ 750.0f + rnd(-pos, pos), 500.0f + rnd(-pos, pos) });
						p->setVelocity({ rnd(-f, f), rnd(-f, f) });
					}


				// hide editor window
				if (event.key.code == sf::Keyboard::E)
					drawEditor = !drawEditor;

				// pause simulation 
				if (event.key.code == sf::Keyboard::P)
					pause = !pause;

				// print list of particles
				if (event.key.code == sf::Keyboard::C)
				{
					std::vector<std::unique_ptr<Particle>>& g = group.getGroup();
					for (auto& p : g)
						std::cout << "type= " << p->getType() << "   id=" << p->getId() << "   dead=" << p->getDead() << std::endl;
				}

				// slide view
				if (event.key.code == sf::Keyboard::Left)
					for (auto& p : group.getGroup())
						p->setPosition(p->getPosition() + sf::Vector2f{ 100.0f, 100.0f });


				// multithreading
				if (event.key.code == sf::Keyboard::T)
				{
					multiThreaded = !multiThreaded;
					std::cout << "multithreading: ";

					if (multiThreaded)
						std::cout << "on, #threads=" << numThreads << std::endl;
					else
						std::cout << "off" << std::endl;
				}
				if (event.key.code == sf::Keyboard::Y)
				{
					numThreads += 1;
					if (numThreads > 32)
						numThreads = 1;
					std::cout << "Number of Threads: " << numThreads << std::endl;
				}
			}
		}

		if (!pause)
		{
			if (multiThreaded)
			{
				std::vector<std::thread> vecThread;

				// split group of particles into numThreads groups

				int numParticlesPerThread = group.getGroup().size() / numThreads;

				std::vector<std::unique_ptr<Particle>>::iterator itStart;
				std::vector<std::unique_ptr<Particle>>::iterator itEnd;
				int offset = 0;

				vecThread.clear();

				// each thread calculates for numParticlesPerThread the forces
				for (size_t i = 0; i < numThreads - 1; i++)
				{
					itStart = group.getGroup().begin() + offset;
					itEnd = group.getGroup().begin() + offset + numParticlesPerThread;

					vecThread.push_back(std::thread(std::ref(group), itStart, itEnd, force));

					offset += numParticlesPerThread;
				}
				// the last thread gets the rest of the particles to calculate the forces
				itStart = group.getGroup().begin() + offset;
				itEnd = group.getGroup().end();
				vecThread.push_back(std::thread(std::ref(group), itStart, itEnd, force));

				for (auto& t : vecThread)
					t.join();
			}
			else
			{
				group.calculateVelocity(force);
				
				// alternative way to calculate force
				for (auto& p : group.getGroup())
					group(*p, force);
			}

			group.move();
		}

		window.clear(sf::Color::Black);
		group.draw(window);
		if (drawEditor)
		{
			//editor.draw(window);
			fe.draw(window);
		}

		window.display();

		if (!nFPS--)
		{
			nFPS = (int)(1000000 / elapsed.asMicroseconds());
			std::cout << "FPS: " << nFPS << std::endl;
		}
	}

	return EXIT_SUCCESS;
}
