#include "LightingEngine.hpp"

void LightingEngine::add_object(Entity &obj)
{
	sf::FloatRect rect{obj.shape.getGlobalBounds()};
	objects.emplace_back(rect.left, rect.top, rect.width, rect.height);
	objects.back().calculate_edges();
	obj.lighting_engine_pos = objects.size()-1;
}

void LightingEngine::add_light(Light l)
{
	lights.push_back(l);
}

void LightingEngine::render(sf::RenderTarget &target)
{
	for(auto &it : lights)
		it.render(target, objects);
}