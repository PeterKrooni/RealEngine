#include "Ray.h"

Point Ray::cast(Vector2 vec)
{
	// Math found at wikipedia page for line-line intersection,
	// https://en.wikipedia.org/wiki/Line%E2%80%93line_intersection#Given_two_points_on_each_line_segment

	float x1 = ray.posx;
	float x2 = ray.dirx;
	float y1 = ray.posy;
	float y2 = ray.diry;

	float x3 = vec.posx;
	float x4 = vec.dirx;
	float y3 = vec.posy;
	float y4 = vec.diry;

	float den = (((x1 - x2) * (y3 - y4)) - ((y1 - y2) * (x3 - x4)));

	// Avoid dividing by zero
	if (den == 0) {
		return Point(NULL, NULL);
	}

	float t_exp = ((x1 - x3)) * (y3 - y4) - ((y1 - y3) * (x3 - x4));
	float u_exp = ((x2 - x1)) * (y1 - y3) - ((y2 - y1) * (x1 - x3));

	float t = t_exp / den;
	float u = u_exp / den;

	// x, y calculated with t
	float x = x1 + (t * (x2 - x1));
	float y = y1 + (t * (y2 - y1));

	// Check if point is intersecting 
	// Point falls within first line segment if 0<=t<=1 
	// and within the second line segment if 0<=u<=1
	if (t >= 0 && t <= 1 && u>=0 && u<=1) {
		return Point(x, y);
	}
	else {
		return Point(NULL, NULL);
	}


	//return ((((this->ray.attatched ? *this->ray.px : this->ray.posx) - ((this->ray.attatched ? *this->ray.px : this->ray.posx) + (this->ray.attatched ? *this->ray.dx : this->ray.dirx))) * ((vec.attatched ? *vec.py : vec.posy) - ((vec.attatched ? *vec.py : vec.posy) + (vec.attatched ? *vec.dy : vec.diry)))) - ((((this->ray.attatched ? *this->ray.py : this->ray.posy)) - ((this->ray.attatched ? *this->ray.py : this->ray.posy) + (this->ray.attatched ? *this->ray.dy : this->ray.diry))) * ((vec.attatched ? *vec.px : vec.posx) - ((vec.attatched ? *vec.px : vec.posx) + (vec.attatched ? *vec.dx : vec.dirx))))) ? SDL_Point(NULL, NULL) : (((((((this->ray.attatched ? *this->ray.px : this->ray.posx) - (vec.attatched ? *vec.px : vec.posx)) * ((vec.attatched ? *vec.py : vec.posy) - ((vec.attatched ? *vec.py : vec.posy) + (vec.attatched ? *vec.dy : vec.diry)))) - ((((this->ray.attatched ? *this->ray.py : this->ray.posy)) - (vec.attatched ? *vec.py : vec.posy)) * ((vec.attatched ? *vec.px : vec.posx) - ((vec.attatched ? *vec.px : vec.posx) + (vec.attatched ? *vec.dx : vec.dirx))))) / ((((this->ray.attatched ? *this->ray.px : this->ray.posx) - ((this->ray.attatched ? *this->ray.px : this->ray.posx) + (this->ray.attatched ? *this->ray.dx : this->ray.dirx))) * ((vec.attatched ? *vec.py : vec.posy) - ((vec.attatched ? *vec.py : vec.posy) + (vec.attatched ? *vec.dy : vec.diry)))) - ((((this->ray.attatched ? *this->ray.py : this->ray.posy)) - ((this->ray.attatched ? *this->ray.py : this->ray.posy) + (this->ray.attatched ? *this->ray.dy : this->ray.diry))) * ((vec.attatched ? *vec.px : vec.posx) - ((vec.attatched ? *vec.px : vec.posx) + (vec.attatched ? *vec.dx : vec.dirx)))))) > 0 && (((((this->ray.attatched ? *this->ray.px : this->ray.posx) - (vec.attatched ? *vec.px : vec.posx)) * ((vec.attatched ? *vec.py : vec.posy) - ((vec.attatched ? *vec.py : vec.posy) + (vec.attatched ? *vec.dy : vec.diry)))) - ((((this->ray.attatched ? *this->ray.py : this->ray.posy)) - (vec.attatched ? *vec.py : vec.posy)) * ((vec.attatched ? *vec.px : vec.posx) - ((vec.attatched ? *vec.px : vec.posx) + (vec.attatched ? *vec.dx : vec.dirx))))) / ((((this->ray.attatched ? *this->ray.px : this->ray.posx) - ((this->ray.attatched ? *this->ray.px : this->ray.posx) + (this->ray.attatched ? *this->ray.dx : this->ray.dirx))) * ((vec.attatched ? *vec.py : vec.posy) - ((vec.attatched ? *vec.py : vec.posy) + (vec.attatched ? *vec.dy : vec.diry)))) - ((((this->ray.attatched ? *this->ray.py : this->ray.posy)) - ((this->ray.attatched ? *this->ray.py : this->ray.posy) + (this->ray.attatched ? *this->ray.dy : this->ray.diry))) * ((vec.attatched ? *vec.px : vec.posx) - ((vec.attatched ? *vec.px : vec.posx) + (vec.attatched ? *vec.dx : vec.dirx)))))) < 1 && (-(((((this->ray.attatched ? *this->ray.px : this->ray.posx) + (this->ray.attatched ? *this->ray.dx : this->ray.dirx)) - (this->ray.attatched ? *this->ray.px : this->ray.posx)) * (((this->ray.attatched ? *this->ray.py : this->ray.posy)) - (vec.attatched ? *vec.py : vec.posy))) - ((((this->ray.attatched ? *this->ray.py : this->ray.posy) + (this->ray.attatched ? *this->ray.dy : this->ray.diry)) - ((this->ray.attatched ? *this->ray.py : this->ray.posy))) * ((this->ray.attatched ? *this->ray.px : this->ray.posx) - (vec.attatched ? *vec.px : vec.posx)))) / ((((this->ray.attatched ? *this->ray.px : this->ray.posx) - ((this->ray.attatched ? *this->ray.px : this->ray.posx) + (this->ray.attatched ? *this->ray.dx : this->ray.dirx))) * ((vec.attatched ? *vec.py : vec.posy) - ((vec.attatched ? *vec.py : vec.posy) + (vec.attatched ? *vec.dy : vec.diry)))) - ((((this->ray.attatched ? *this->ray.py : this->ray.posy)) - ((this->ray.attatched ? *this->ray.py : this->ray.posy) + (this->ray.attatched ? *this->ray.dy : this->ray.diry))) * ((vec.attatched ? *vec.px : vec.posx) - ((vec.attatched ? *vec.px : vec.posx) + (vec.attatched ? *vec.dx : vec.dirx))v)))) > 0) ? SDL_Point(((this->ray.attatched ? *this->ray.px : this->ray.posx) + ((((this->ray.attatched ? *this->ray.px : this->ray.posx) - (vec.attatched ? *vec.px : vec.posx)) * ((vec.attatched ? *vec.py : vec.posy) - ((vec.attatched ? *vec.py : vec.posy) + (vec.attatched ? *vec.dy : vec.diry)))) - ((((this->ray.attatched ? *this->ray.py : this->ray.posy)) - (vec.attatched ? *vec.py : vec.posy)) * ((vec.attatched ? *vec.px : vec.posx) - ((vec.attatched ? *vec.px : vec.posx) + (vec.attatched ? *vec.dx : vec.dirx))))) * (((this->ray.attatched ? *this->ray.px : this->ray.posx) + (this->ray.attatched ? *this->ray.dx : this->ray.dirx)) - (this->ray.attatched ? *this->ray.px : this->ray.posx))), (((this->ray.attatched ? *this->ray.py : this->ray.posy)) + ((((this->ray.attatched ? *this->ray.px : this->ray.posx) - (vec.attatched ? *vec.px : vec.posx)) * ((vec.attatched ? *vec.py : vec.posy) - ((vec.attatched ? *vec.py : vec.posy) + (vec.attatched ? *vec.dy : vec.diry)))) - ((((this->ray.attatched ? *this->ray.py : this->ray.posy)) - (vec.attatched ? *vec.py : vec.posy)) * ((vec.attatched ? *vec.px : vec.posx) - ((vec.attatched ? *vec.px : vec.posx) + (vec.attatched ? *vec.dx : vec.dirx))))) * (((this->ray.attatched ? *this->ray.py : this->ray.posy) + (this->ray.attatched ? *this->ray.dy : this->ray.diry)) - ((this->ray.attatched ? *this->ray.py : this->ray.posy))))) : SDL_Point(NULL, NULL));
}
