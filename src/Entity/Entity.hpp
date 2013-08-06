#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "LightingEngine/LightingEngine.hpp"

class LightingEngine;

class Entity
{
public:
	sf::RectangleShape shape;
	sf::Texture texture;
	bool use_in_lighting;
	int lighting_engine_pos;
	std::shared_ptr<LightingEngine> light_engine;

	void update_lighting_engine();
	bool collides_with(const Entity &a);
	void render(sf::RenderTarget &target);
	sf::Vector2f get_position();
	bool set_texture(const std::string &fname);
	void set_texture(const sf::Texture &tex);
	Entity(const int &lep);
	Entity();
};