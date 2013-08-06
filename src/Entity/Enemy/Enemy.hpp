#include <SFML/Graphics.hpp>
#include "Entity/Entity.hpp"

class Enemy : public Entity
{
public:
	sf::Vector2f target;
	float speed;
	void update(const float &dt);
	void update_target(const sf::Vector2f &tgt);
	std::vector<sf::Vector2f> path;
	Enemy(const sf::Vector2f &tgt, const float &s, const std::vector<sf::Vector2f> &path_, const int &lep);
	Enemy(const sf::Vector2f &tgt, const float &s, const std::vector<sf::Vector2f> &path_);
};