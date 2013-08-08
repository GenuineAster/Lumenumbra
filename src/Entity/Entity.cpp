#include "Entity.hpp"
#include <SFML/Graphics.hpp>

void Entity::update_lighting_engine()
{
	auto &obj = light_engine->objects[lighting_engine_pos];
	obj.position = shape.getPosition();
	// obj.left = shape.getPosition().x;
	// obj.top = shape.getPosition().y;
	obj.size = shape.getSize();
	// obj.width = shape.getSize().x;
	// obj.height = shape.getSize().y;
	obj.calculate_edges();
}

bool Entity::collides_with(const Entity &a)
{
	return shape.getGlobalBounds().intersects(a.shape.getGlobalBounds());
}

void Entity::render(sf::RenderTarget &target){target.draw(shape);}
sf::Vector2f Entity::get_position(){return shape.getPosition();}
bool Entity::set_texture(const std::string &fname)
{
	bool return_value{texture.loadFromFile(fname)};
	shape.setTexture(&texture);
	return return_value;
}

void Entity::set_texture(const sf::Texture &tex){texture = tex; shape.setTexture(&texture);}

Entity::Entity(const int &lep) :
	lighting_engine_pos{lep}
{
	;
}

Entity::Entity(){}
