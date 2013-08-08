#pragma once
#include <SFML/Graphics.hpp>
#include "Tools/Tools.hpp"
#include <vector>

//using Rect = sf::FloatRect;
//using Point = sf::Vector2f;

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

	bool is_within_range(const Point &ray, const Rect &r);
	bool hit_test_bound(sf::Vector2f min, sf::Vector2f max, sf::Vector2f point);
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