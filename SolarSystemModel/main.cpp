#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

#include "constants.hpp"
#include "Planet.hpp"

using namespace std;
using namespace sf;

int main()
{
	RenderWindow window(VideoMode(constants::SCREEN_WIDTH, constants::SCREEN_HEIGHT), "Solar System Model");
	window.setFramerateLimit(60);

	vector<Planet> planets{ 
		Planet(Color::Red, 30.0f, 2e7, Vector2f(750, 500)), // Sun
		Planet(Color::Blue, 10.0f, 6e1, Vector2f(1000, 500)), // Earth (Blue)
		Planet(Color::Magenta, 20.0f, 6e4, Vector2f(200, 500)), // Planet X (Magenta)
		Planet(Color::Green, 15.0f, 6e2, Vector2f(750, 200)) // Planet Y (Green)
	};
	planets[1].setVelocity(Vector2f(0, 60)); // set Earth velocity
	planets[2].setVelocity(Vector2f(0, -40)); // Set Planet X velocity
	planets[3].setVelocity(Vector2f(30, 0)); // Set Planet Y velocity (highly elliptical)

	Clock clock;

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}
		window.clear();
		
		float seconds = clock.restart().asSeconds();

		for (int i = 0; i < planets.size(); i++)
		{
			planets[i].reactToGravity(planets, i, seconds);
		}

		for (int i = 0; i < planets.size(); i++)
		{
			planets[i].update(seconds);
			planets[i].draw(window);
		}

		window.display();
	}

	return 0;
}