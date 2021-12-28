#include "Planet.hpp"
#include "constants.hpp"
#include <SFML\Graphics.hpp>
#include <cmath>
#include <vector>
#include <iostream>

using namespace sf;
using namespace std;

Planet::Planet(Color color, float radius, float mass, Vector2f position)
	: color(color), radius(radius), mass(mass), position(position), circle(radius), trail(LineStrip, constants::TRAIL_LENGTH), previousTrailPosition(position)
{ 
	circle.setFillColor(color);
	circle.setOrigin(radius, radius);

	for (int i = 0; i < constants::TRAIL_LENGTH; i++)
	{
		trail[i].position = position;
		trail[i].color = color;
		trail[i].color.a = ((constants::TRAIL_LENGTH - 1.0f - i) / (constants::TRAIL_LENGTH - 1)) * 255.0f;
	}
}

void Planet::setPosition(Vector2f pos)
{
	position = pos;
	circle.setPosition(pos);
}

Vector2f Planet::getPosition() const
{
	return position;
}

float Planet::getMass() const
{
	return mass;
}

void Planet::setVelocity(Vector2f vel)
{
	velocity = vel;
}

void Planet::reactToGravity(const vector<Planet> &planets, int thisIndex, float seconds)
{
	// F = (-GmM/r^2)r_norm

	Vector2f net_force;

	for (int i = 0; i < planets.size(); i++)
	{
		if (i != thisIndex)
		{
			Vector2f dist(planets[i].getPosition() - getPosition());
			float dist_mag = sqrtf(dist.x * dist.x + dist.y * dist.y);
			if (dist_mag == 0)
				continue;
			float real_dist = dist_mag;
			float force_mag = constants::GRAV * planets[i].getMass() * getMass() / (real_dist * real_dist);
			net_force += (dist / dist_mag) * force_mag;
		}
	}

	velocity += net_force / getMass() * seconds;
}

void Planet::update(float seconds)
{
	setPosition(getPosition() + velocity * seconds);




	if (pow(position.x - previousTrailPosition.x, 2) + pow(position.y - previousTrailPosition.y, 2) > radius * radius)
	{
		trail[0].position = position;
		for (int i = constants::TRAIL_LENGTH - 1; i >= 1; i--)
		{
			trail[i].position = trail[i - 1].position;
		}
		previousTrailPosition = position;
	}
}

void Planet::draw(RenderWindow& window)
{
	window.draw(trail);
	window.draw(circle);
}