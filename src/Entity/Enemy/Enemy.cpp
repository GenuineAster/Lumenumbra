#include "Enemy.hpp"
#include <SFML/Graphics.hpp>

void Enemy::update(const float &dt)
{
	sf::Vector2f vec = target - shape.getPosition();
	shape.setPosition(
		shape.getPosition() +
		(
			vec / sqrtf(vec.x*vec.x+vec.y*vec.y)
		) * speed * dt
	);
}

void Enemy::update_target(const sf::Vector2f &tgt)
{
	target = tgt;
}

Enemy::Enemy(const sf::Vector2f &tgt, const float &s, const std::vector<sf::Vector2f> &path_):
	Entity(),
	target{tgt},
	speed{s},
	path{path_}
{}

Enemy::Enemy(const sf::Vector2f &tgt, const float &s, const std::vector<sf::Vector2f> &path_, const int &lep):
	Entity(lep),
	target{tgt},
	speed{s},
	path{path_}
{}
