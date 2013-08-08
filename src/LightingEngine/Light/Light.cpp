#include "Light.hpp"
#include "Tools/Tools.hpp"
#include <SFML/OpenGL.hpp>
#include <cmath>
#include <vector>

bool Light::hit_test_bound(sf::Vector2f min, sf::Vector2f max, sf::Vector2f point)
{

	if(point.x >= min.x-0.002f)
		if(point.y >= min.y-0.002f)
			if(point.x <= max.x+0.002f)
				if(point.y <= max.y+0.002f)
					return true;
	return false;
}

bool Light::is_within_range(const Point &ray, const Rect &r)
{
	//Construct AABB out of ray and position
	float r1_min_x = std::min(position.x, ray.x);
	float r1_max_x = std::max(position.x, ray.x);
	float r1_min_y = std::min(position.y, ray.y);
	float r1_max_y = std::max(position.y, ray.y);

	float r2_min_x = std::min(r.position.x, r.position.x+r.size.x);
	float r2_max_x = std::max(r.position.x, r.position.x+r.size.x);
	float r2_min_y = std::min(r.position.y, r.position.y+r.size.y);
	float r2_max_y = std::max(r.position.y, r.position.y+r.size.y);

	float inter_left  = std::max(r1_min_x, r2_min_x);
	float inter_top   = std::max(r1_min_y, r2_min_y);
	float inter_right = std::min(r1_max_x, r2_max_x);
	float inter_bottom= std::min(r1_max_y, r2_max_y);

	if((inter_left < inter_right) && (inter_top < inter_bottom))
		return true;
	return false;
}



void Light::render(sf::RenderTarget &target, std::vector<Rect> &objects)
{
	sf::Color col = color;

	vertices.clear();

	vertices.append({{position.x, position.y}, col});
	float pi_a = 3.14159f/180.f;
	float normalized_x,normalized_y,t,angle;

	for(float i = 0.f; i < 361.f; i+=0.3f)
	{
		angle = i * pi_a;
		normalized_x = cosf(angle);
		normalized_y = sinf(angle);
		t = radius;

		sf::Vector2f ray{normalized_x*radius, normalized_y*radius};
		ray += position;

		for(auto &it : objects)
		{
			if(!is_within_range(ray, it))
			 	continue;
			for(const auto &edge : it.get_edges())
			{
				float tempt{radius}, ax, ay, rise{edge.B.y - edge.A.y}, run{edge.B.x - edge.A.x};
				if(rise <= 0.f)
					ay = 1.f, ax = 0.f;
				else if(run <= 0.0f)
					ay = 0.f, ax = 1.f;
				else
					ax = -run/rise, ay = 1.f;

				float yintersect = edge.A.y * ay + ax * edge.A.x;
				float a_dot_d{ax*normalized_x+ay*normalized_y};
				if(fabs(a_dot_d)<=0.0f)
					continue;
				tempt = (yintersect-(ax*position.x+position.y*ay)) / a_dot_d;

				Point p{position.x+normalized_x*tempt, position.y+normalized_y*tempt};

				if(hit_test_bound(edge.A, edge.B, p))
					if(tempt <= t && tempt >= 1.f)
						t = tempt;
			}
		}

		float alphascale = 0;//t / radius;
		col.a = 255.f - (255.f * alphascale);

		vertices.append({{position.x+normalized_x*t, position.y+normalized_y*t}, col});
	}
	target.draw(vertices);
}
