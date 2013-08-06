#include "Light.hpp"
#include "Tools/Tools.hpp"
#include <SFML/OpenGL.hpp>
#include <cmath>
#include <vector>

bool Light::is_within_range(const Rect &r)
	{
		//get closest point
		Point tl,tr,bl,br;
		tl = r.position - position;
		br = r.position+r.size - position;
		tr = r.position+Point{r.size.x,0} - position;
		bl = r.position+Point{0,r.size.y} - position;

		if(position.x > r.position.x && position.x < r.size.x+r.position.x)
		{	
			if(position.y+radius > r.position.y)
				return true;
			if(position.y-radius < r.position.y+r.size.y)
				return true;
		}

		if(position.y > r.position.y && position.y < r.size.y+r.position.y)
		{	
			if(position.x+radius > r.position.x)
				return true;
			if(position.x-radius < r.position.x+r.size.x)
				return true;
		}

		if((tl.x*tl.x + tl.y*tl.y) < radius*radius)
			return true;
		if((br.x*br.x + br.y*br.y) < radius*radius)
			return true;
		if((bl.x*bl.x + bl.y*bl.y) < radius*radius)
			return true;
		if((tr.x*tr.x + tr.y*tr.y) < radius*radius)
			return true;
		return false;
	}

void Light::render(sf::RenderTarget &target, std::vector<Rect> &objects)
{
	sf::Color col = color;

	vertices.clear();

	vertices.append({{position.x, position.y}, col});
	float pi_a = 3.14159f/180.f;
	float dx,dy,t,angle;

	for(float i = 0.f; i < 361.f; i+=0.5f)
	{
		angle = i * pi_a;
		dx = cosf(angle);
		dy = sinf(angle);
		t = radius;

		for(auto &it : objects)
		{
			if(!is_within_range(it))
				continue;
			for(const auto &edge : it.get_edges())
			{
				float tempt{radius}, ax, ay, rise{edge.ey - edge.y}, run{edge.ex - edge.x};
				if(rise <= 0.f)
					ay = 1.f, ax = 0.f;
				else if(run <= 0.0f)
					ay = 0.f, ax = 1.f;
				else
					ax = -run/rise, ay = 1.f;

				float yintersect = edge.y * ay + ax * edge.x;
				float adotd{ax*dx+ay*dy};
				if(fabs(adotd)<=0.0f)
					continue;
				tempt = (yintersect-(ax*position.x+position.y*ay)) / adotd;

				Point p{position.x+dx*tempt, position.y+dy*tempt};

				if(hit_test_bound({edge.x, edge.y}, {edge.ex, edge.ey}, p))
				{
					if(tempt <= t && tempt >= 1.f)
						t = tempt;
				}
			}
		}

		float alphascale = 0;//t / radius;
		col.a = 255.f - (255.f * alphascale);

		vertices.append({{position.x+dx*t, position.y+dy*t}, col});
	}
	target.draw(vertices);
}
