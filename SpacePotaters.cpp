#include <cmath>
#include <unistd.h>
#include <iostream>
extern "C"{
#include "gfx.h"
}
#include "Aliens.h"
#include "BattleShips.h"
#include <algorithm>
#include <vector>
#include <time.h>
#include <string>
using namespace std;

bool intersected(vector<int> seg1, vector<int> seg2)
{
	vector<int> p0 = {seg1[0], seg1[1]};
	vector<int> p1 = {seg1[2], seg1[3]};
	vector<int> p2 = {seg2[0], seg2[1]};
	vector<int> p3 = {seg2[2], seg2[3]};
	double num_a = (p3[0] - p2[0]) * (p0[1] - p2[1]) - (p3[1] - p2[1]) * (p0[0] - p2[0]);
	double num_b = (p1[0] - p0[0]) * (p0[1] - p2[1]) - (p1[1] - p0[1]) * (p0[0] - p2[0]);
	double den = (p3[1] - p2[1]) * (p1[0] - p0[0]) - (p3[0] - p2[0]) * (p1[1] - p0[1]);
	if(den == 0)
		return false;
	else
	{
		double u_a = num_a/den;
		double u_b = num_b/den;
		int intersectx = p0[0] + u_a * (p1[0] - p0[0]);
		int intersecty = p0[1] + u_a * (p1[1] - p0[1]);
		if(u_a == u_b)
			return false;

		if((u_a >= 0 && u_a <= 1) && (u_b >= 0 && u_b <= 1))
		{
			return true;
		}
		else
			return false;
	}
}
int main()
{
	gfx_open(500, 500, "Space Potaters");

	int x[] = {30, 50, 23, 17, 120};
	int y[] = {12, 55, 52, 19, 100};

	//gfx_color(255, 0, 0);
	//draw_polygon(x, y, 5);
	gfx_color(0, 255, 0);
	//draw_circle(250, 250, 50);
	AlienArmy army(4, 7);

	gfx_flush();
	usleep(4000000);
	
	GreenAlien alien1(250, 250);
	alien1.draw_alien();
	RedAlien alien2(100, 50);
	alien2.draw_alien();
	BlueAlien alien3(200, 50);
	alien3.draw_alien();
	ship_base ship;
	ship.draw_base();

	std::vector<bullet> bullet_list;
	std::vector<AlienBase> aliens;
	aliens.push_back(alien1);
	aliens.push_back(alien2);
	aliens.push_back(alien3);
	std::string dir = " ";
    clock_t deltaT;
    double dt;
	bool canFire;
	while(true)
	{
        deltaT = clock();
		if(gfx_event_waiting())
		{
			int button = gfx_wait();
			// left arrow
			if ((button == 81 || button == 65430 || button == 65361)&&(dir != "L"))
			{
			//	gfx_clear();
                dir = "L";
				ship.move("L", dt);
			}

			// right arrow
			else if ((button == 83 || button == 65432 || button == 65363)&&(dir != "R"))
			{
			//	gfx_clear();
                dir = "R";
				ship.move("R", dt);
			}

			if(((button != 81)||(button!=83))&&((dir=="R")||(dir == "L")))
			{
				dir = " ";
			}

			if (button == 82 || button == 65431 || button == 65362)
			{
				ship.fire();
			}

			if(button == 112)
			{
				std::cout << "---Aliens---" << std::endl;
				for(auto alien:aliens)
					alien.printCoords();
				std::cout << "---Bullets---" << std::endl;
				for(auto bullet:ship.playerbullets)
					std::cout << "(" << bullet.bounds[0] << ", " << bullet.bounds[1] << ") (" << bullet.bounds[2] << ", " << bullet.bounds[3] << ")" << std::endl;
			}

			//std::cout << "got event: " << button << std::endl;
			if(button == 27)
				break;
		}
		else
		{
			usleep(16667);
			gfx_clear();
            deltaT = (clock() - deltaT);
            dt = (double)deltaT/1000.0f;

            //Update movement after calculating deltaT
            alien1.move();
			alien2.move();
			alien3.move();
			for(auto alien: aliens)
				alien.move();
			ship.move(dir, dt);
			ship.draw_base();
			for(int i = 0; i < ship.playerbullets.size(); i++)
			{
				ship.playerbullets[i].move(dt);
                if(ship.playerbullets[i].getY() < 0)
                {
                    ship.playerbullets.erase(ship.playerbullets.begin() + i);
                }
			}

			//Collision Detection
			
			for(auto bullet: ship.playerbullets)
			{
				for (auto bound: alien1.bounds)
				{
					bool collided = intersected(bound, bullet.bounds);
					if(collided)
					{
						std::cout << "HIT!" << std::endl;
					}
				}
			}
		}
	}
}
