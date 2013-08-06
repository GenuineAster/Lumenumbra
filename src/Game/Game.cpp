#include "Game.hpp"

void Game::get_dt()
{
	old_frametime = frametime;
	frametime = dt_clock.restart().asMicroseconds()/1000000.f;
}

void Game::parse_levels_file()
{
	Json::Value levels_root;
	Json::Reader levels_reader;
	std::ifstream levels_file("assets/levels/levels.json");
	std::string file_contents;
	while(levels_file.good())
	{
		std::string a;
		getline(levels_file, a);
		file_contents += a + "\n";
	}
	levels_file>>file_contents;
	std::cout<<file_contents<<std::endl;
	if(!levels_reader.parse(file_contents, levels_root))
	{
		std::cout<<levels_reader.getFormattedErrorMessages();
		return;
	}
	Json::Value json_levels = levels_root["levels"];
	for(auto level_root : json_levels)
	{
		std::string name =  level_root["name"].asString();
		std::cout<<name;
		std::ifstream curr_level_file("assets/levels/" + name + ".json");
		file_contents.clear();
		while(curr_level_file.good())
		{
			std::string a;
			getline(curr_level_file, a);
			file_contents += a + "\n";
		}
		std::cout<<file_contents;
		Json::Value root;
		Json::Reader reader;
		if(!reader.parse(file_contents, root))
		{
			std::cout<<"AAAH YOU FUCKED UP, IDIOT!"
			         <<reader.getFormattedErrorMessages()
			         <<std::endl;
			return;
		}
		levels.push_back({});
		auto &level = levels.back();
		level.pos         = root["id"]  .asInt();
		level.name        = root["name"].asString();
		level.description = root["description"].asString();

		level.player_pos.x = root["player"][0].asFloat();
		level.player_pos.y = root["player"][1].asFloat();

		if(!root["winzone"].isNull())
		{
			level.winzone_in_use = true;
			level.winzone.shape.setPosition(
				{
					root["winzone"]["pos"][0].asFloat(),
					root["winzone"]["pos"][1].asFloat(),
				}
			);
			level.winzone.shape.setSize(
				{
					root["winzone"]["size"][0].asFloat(),
					root["winzone"]["size"][1].asFloat(),
				}
			);
			level.winzone.shape.setFillColor({125,125,125});
		}

		level.player_speed = root["player_speed"].asFloat();

		Json::Value json_walls = root["walls"];
		for(auto &json_wall : json_walls)
		{
			level.walls.emplace_back();
			auto &wall = level.walls.back();
			wall.use_in_lighting = json_wall["use_in_lighting"].asBool();
			wall.shape.setPosition(
				{
					json_wall["pos"][0].asFloat(),
					json_wall["pos"][1].asFloat()
				}
			);
			wall.shape.setSize(
				{
					json_wall["size"][0].asFloat(),
					json_wall["size"][1].asFloat()
				}
			);
			wall.shape.setFillColor(
				{
					static_cast<sf::Uint8>(json_wall["color"][0].asUInt()),
					static_cast<sf::Uint8>(json_wall["color"][1].asUInt()),
					static_cast<sf::Uint8>(json_wall["color"][2].asUInt()),
					static_cast<sf::Uint8>(json_wall["color"][3].asUInt())
				}
			);
		}

		Json::Value json_texts = root["texts"];
		for(auto &json_text : json_texts)
		{
			level.texts.emplace_back();
			auto &text = level.texts.back();
			text.setString(json_text["content"].asString());
			text.setCharacterSize(json_text["size"].asInt());
			text.setColor(
				{
					static_cast<sf::Uint8>(json_text["color"][0].asUInt()),
					static_cast<sf::Uint8>(json_text["color"][1].asUInt()),
					static_cast<sf::Uint8>(json_text["color"][2].asUInt()),
					static_cast<sf::Uint8>(json_text["color"][3].asUInt())
				}
			);
			text.setPosition(
				{
					json_text["pos"][0].asFloat(),
					json_text["pos"][1].asFloat()
				}
			);
			text.setFont(font);
		}

		std::cout<<level.pos<<level.name<<level.description<<std::endl;
	}
}

void Game::load_level()
{
	light_engine.objects.clear();
	for(auto &obj : levels[current_level_pos].walls)
	{
		if(obj.use_in_lighting)
			light_engine.add_object(obj);
	}
	player.shape.setPosition(levels[current_level_pos].player_pos);
	speed = levels[current_level_pos].player_speed;
}

void Game::initialize_game()
{
	font.loadFromFile("assets/fonts/DejaVuSans.ttf");
	parse_levels_file();
	++current_level_pos;
	load_level();

	light_engine.add_light(
		{
			{200,200}, 
		    (window.getSize().y > window.getSize().x) ? 
		    	static_cast<float>(window.getSize().y) : 
		    	static_cast<float>(window.getSize().x),
		    sf::Color::White
		}
	);
}

void Game::handle_events()
{
	sf::Event event;
	while(window.pollEvent(event))
	{
		switch(event.type)
		{
		case sf::Event::Closed:
			window.close();
			break;
		case sf::Event::Resized:
			view.reset(
				{
					0,
					0,
					static_cast<float>(window.getSize().x),
					static_cast<float>(window.getSize().y)
				}
			);
			view.setViewport({0.f,0.f,1.f,1.f});
			view.zoom(600.f/window.getSize().y);
			std::cout<<window.getSize().x<<"\t"<<window.getSize().y<<std::endl;
			break;

		case sf::Event::KeyPressed:
			if(event.key.code == sf::Keyboard::Equal)
				view.zoom(2);
			else if(event.key.code == sf::Keyboard::Dash)
				view.zoom(0.5);
			break;

		default:
			break;
		}
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{	
		player.shape.setPosition(
			{player.shape.getPosition().x,
			 player.shape.getPosition().y-(speed*frametime)}
		);
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{	
		player.shape.setPosition(
			{player.shape.getPosition().x,
			 player.shape.getPosition().y+(speed*frametime)}
		);
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{	
		player.shape.setPosition(
			{player.shape.getPosition().x-(speed*frametime),
			 player.shape.getPosition().y}
		);
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{	
		player.shape.setPosition(
			{player.shape.getPosition().x+(speed*frametime),
			 player.shape.getPosition().y}
		);
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{	
		std::cout<<"Frametime:\t"<<frametime<<std::endl
		         <<"Player position:\t"<<player.shape.getPosition().x
		                        <<",\t"<<player.shape.getPosition().y
		         <<std::endl;
	}

}

void Game::move_player_from_wall(Entity &wall)
{
	sf::FloatRect wfr = wall.shape.getGlobalBounds();
	float px = player.shape.getPosition().x,
	      py = player.shape.getPosition().y,
	      w_pl = wfr.left,
	      w_pt = wfr.top,
	      w_pb = wfr.height+w_pt,
	      w_pr = wfr.width+w_pl;

	float dl{static_cast<float>(fabs(px-w_pl))}, 
	      dr{static_cast<float>(fabs(px-w_pr))},
	      dt{static_cast<float>(fabs(py-w_pt))}, 
	      db{static_cast<float>(fabs(py-w_pb))};

	bool left{dl < dr},
	     top {dt < db};

	float pm = old_frametime*speed*2;

	if(left && top)
	{
		if(dt < dl)
			player.shape.setPosition({player.shape.getPosition().x, py-pm});
		else
			player.shape.setPosition({px-pm, player.shape.getPosition().y});
		return;
	}
	if(left && !top)
	{
		if(db < dl)
			player.shape.setPosition({player.shape.getPosition().x, py+pm});
		else
			player.shape.setPosition({px-pm, player.shape.getPosition().y});
		return;
	}
	if(!left && top)
	{
		if(dt < dr)
			player.shape.setPosition({player.shape.getPosition().x, py-pm});
		else
			player.shape.setPosition({px+pm, player.shape.getPosition().y});
		return;
	}
	if(!left && !top)
	{
		if(db < dr)
			player.shape.setPosition({player.shape.getPosition().x, py+pm});
		else
			player.shape.setPosition({px+pm, player.shape.getPosition().y});
		return;
	}

}

void Game::update()
{
	/*for(auto &enemy : enemies)
	{
		enemy.update_target(player.shape.getPosition());
		enemy.update(dt);
	}*/
	view.setCenter(player.shape.getPosition());
	light_engine.lights[0].position = player.shape.getPosition();
	auto a = levels[current_level_pos].winzone.shape.getGlobalBounds();
	if(a.contains(player.shape.getPosition()))
	{
		++current_level_pos;
		if(static_cast<unsigned int>(current_level_pos) < levels.size())
			load_level();
	}
	for(auto &wall : levels[current_level_pos].walls)
	{
		sf::FloatRect a = wall.shape.getGlobalBounds();
		a.left -= 4;
		a.top -= 4;
		a.height += 5.8;
		a.width += 5.8;
		if(a.contains(player.shape.getPosition()))
			move_player_from_wall(wall);
	}
}

void Game::draw()
{
	window.clear();
	window.setView(view);
	auto &current_level = levels[current_level_pos];

	light_engine.render(window);

	for(auto &text : current_level.texts)
		window.draw(text);

	for(auto &wall : current_level.walls)
		wall.render(window);

	for(auto &enemy : current_level.enemies)
		enemy.render(window);

	current_level.winzone.render(window);

	player.render(window);
	window.setView(window.getDefaultView());
	window.display();
}

void Game::game_loop()
{
	get_dt();
	handle_events();
	update();
	draw();
}

void Game::run()
{
	/* Why not just have the game loop in here?
	 * Because I want to put some code in this
	 * like.. FPS limiters and things. :<
	*/
	initialize_game();
	while(window.isOpen())
	{
		game_loop();
	}
}

Game::Game(sf::VideoMode vmode) :
	window{vmode, "SFML Jam #1!"},
	current_level_pos{-1},
	speed{100}
{

	view.reset(
		{
			0,
			0,
			static_cast<float>(window.getSize().x),
			static_cast<float>(window.getSize().y)
		}
	);
	view.setViewport({0.f,0.f,1.f,1.f});
	window.setView(view);
	player.shape.setSize({5,5});
	player.shape.setFillColor(sf::Color::Black);
}