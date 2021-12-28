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
	// Initialize SFML circle, the graphical representation of the planet
	circle.setFillColor(color);
	circle.setOrigin(radius, radius);

	// Initialize trail segments to the starting position
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

	// Find the net force on this planet
	for (int i = 0; i < planets.size(); i++)
	{
		// Prevent planet from being attracted to itself
		if (i != thisIndex)
		{
			Vector2f dist(planets[i].getPosition() - getPosition());
			float dist_mag = sqrtf(dist.x * dist.x + dist.y * dist.y);
			// Prevent divide by 0 error
			if (dist_mag == 0)
				continue;
			// Optional position scaling step (currently disabled)
			float real_dist = dist_mag;
			float force_mag = constants::GRAV * planets[i].getMass() * getMass() / (real_dist * real_dist);
			// Accumulate net force
			net_force += (dist / dist_mag) * force_mag;
		}
	}

	// Update velocity based on the net force
	velocity += net_force / getMass() * seconds;
}

void Planet::update(float seconds)
{
	// Update the position based on velocity and time passed
	setPosition(getPosition() + velocity * seconds);

	// Update trail segments
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