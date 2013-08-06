#pragma once
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>

using Point = sf::Vector2f;

class Line
{
public:
	float x, y, ex, ey;
	Line():x{0.f},y{0.f},ex{0.f},ey{0.f}{}
	Line(float x_, float y_, float ex_, float ey_):x{x_},y{y_},ex{ex_},ey{ey_}{}
};

class Rect
{
public:
	Point position;
	Point size;
	std::vector<Line> edges;
	void calculate_edges()
	{
		edges = 
		{
			{position.x,        position.y,        position.x+size.x, position.y       },
			{position.x,        position.y+size.y, position.x+size.x, position.y+size.y},
			{position.x+size.x, position.y,        position.x+size.x, position.y+size.y},
			{position.x,        position.y,        position.x,        position.y+size.y}
		};
	}

	Rect(float dx, float dy, float dw, float dh):position{dx,dy},size{dw,dh}{}

	const std::vector<Line> &get_edges()
	{
		return edges;
	}
	
};