#pragma once
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>

using Point = sf::Vector2f;

struct ray_collision
{
	bool collided;
	sf::Vector2f point_of_collision;
	ray_collision(const bool &col, const sf::Vector2f &poc):collided{col},point_of_collision{poc}{}
	ray_collision():collided{false}{}
};


class Line
{
public:
	sf::Vector2f A;
	sf::Vector2f B;
	ray_collision intersects(Line &b)
	{
		Point CmP{b.A.x-A.x, b.A.y-A.y};
		Point r{B.x-A.x, B.y-A.y};
		Point s{b.B.x-b.A.x, b.B.y-b.A.y};

		float rxs = r.x*s.y - s.y*s.x;
		if(rxs == 0.f)
			return {false, {0,0}};

		float CmPxr = CmP.x*r.y - CmP.y*r.x;
		if(CmPxr == 0.f)
			return {false, {0,0}};

		float CmPxs = CmP.x*s.y - CmP.y*s.x;
		float rxsr = 1.f/rxs;
		float t = CmPxs*rxsr;
		float u = CmPxr*rxsr;

		if(t>=0.f && t<=1.f && u>=0.f && u<=1.f)
		{
			return {true, A+t*r};
		}

	}
	Line(const float &ax, const float &ay, const float &bx, const float &by):A{ax,ay},B{bx,by}{}
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

	const std::vector<Line> &get_edges()
	{
		return edges;
	}

	Rect(const float &dx, const float &dy, const float &dw, const float &dh):position{dx,dy},size{dw,dh}{}
	
};