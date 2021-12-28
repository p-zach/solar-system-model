#pragma once

#include <SFML\Graphics.hpp>
#include <vector>

using namespace sf;
using namespace std;

class Planet
{
private:
	CircleShape circle;
	VertexArray trail;
	Color color;
	float radius;
	float mass;
	Vector2f position;
	Vector2f velocity;
	Vector2f previousTrailPosition;

	Planet();
public:
	Planet(Color, float, float, Vector2f);

	void setPosition(Vector2f);
	Vector2f getPosition() const;

	float getMass() const;

	void setVelocity(Vector2f);

	void reactToGravity(const vector<Planet> &, int, float);
	void update(float);

	void draw(RenderWindow&);
};