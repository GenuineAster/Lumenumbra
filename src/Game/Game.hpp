#pragma once
#include <SFML/Graphics.hpp>
#ifdef _WIN32
#include <json/json.h>
#else
#include <jsoncpp/json/json.h>
#endif
#include <vector>
#include <fstream>
#include <iostream>
#include "LightingEngine/LightingEngine.hpp"
#include "Entity/Entity.hpp"
#include <memory>

class Game
{
protected:
private:
public:

	struct Level
	{
		int pos;
		float player_speed;
		sf::Vector2f player_pos;
		std::string name;
		std::string description;
		std::vector<Entity> walls;
		Entity winzone;
		bool winzone_in_use{false};
		std::vector<sf::Text> texts;
	};

	sf::RenderWindow window;
	LightingEngine light_engine;
	int current_level_pos;
	std::vector<Level> levels;
	Entity player;
	std::shared_ptr<Light> player_light;
	sf::View view;
	sf::Clock dt_clock;
	sf::Font font;
	float speed, frametime, old_frametime;
	int argc; char** argv;
	bool debug;

	void get_dt();
	void move_player_from_wall(Entity &wall);
	float get_integer_value(Json::Value &v);
	void parse_levels_file();
	void load_level();
	void parse_arguments();
	void initialize_game();
	void handle_events();
	void update();
	void draw();
	void game_loop();
	void run();

	Game(int argc_, char** argv_, sf::VideoMode vmode);

};
