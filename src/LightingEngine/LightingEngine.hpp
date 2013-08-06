#pragma once
#include <SFML/Graphics.hpp>
#include "Light/Light.hpp"
#include "Entity/Entity.hpp"
#include <vector>

/*
Lighting System code was adapted to SFML by Mischa Alff, but found 
originally on http://www.dannybirch.com/simple-2d-lighting-system/

Stop getting angry.
*/

class Entity;

class LightingEngine
{
protected:
private:
public:
	std::vector<Light> lights;
	std::vector<Rect> objects;

	void add_object(Entity &obj);
	void add_light(Light l);
	void render(sf::RenderTarget &target);
};