#pragma once
#include <SFML/Graphics.hpp>
#include "Tools/Tools.hpp"
#include <vector>

class Light
{
protected:
private:
public:
	sf::Vector2f position;
	float radius, angle, anglespread;
	sf::Color color;
	sf::VertexArray vertices;
	bool dynamic;

	bool is_within_range(const Rect &r);

	bool hit_test_bound(sf::Vector2f min, sf::Vector2f max, sf::Vector2f point)
	{

		if(point.x >= min.x-0.002f)
			if(point.y >= min.y-0.002f)
				if(point.x <= max.x+0.002f)
					if(point.y <= max.y+0.002f)
						return true;
		return false;
	}

	void render(sf::RenderTarget &target, std::vector<Rect> &objects);


	Light(sf::Vector2f pos, float rad, sf::Color col):
	position{pos},
	radius{rad},
	color{col},
	dynamic{true}
	{
		vertices.setPrimitiveType(sf::PrimitiveType::TrianglesFan);
	}
};