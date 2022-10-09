#include "Ball.h"


Ball::Ball()
{
    rect.x = 0;
    rect.y = 0;
    rect.w = BALL_W;
    rect.h = BALL_H;
    this->limit_xmin = 0;
    this->limit_xmax = 0;
    this->limit_ymin = 0;
    this->limit_ymax = 0;
}

enum ball_exit Ball::move() 
{ 
    this->rect.x += this->dx;
    this->rect.y += this->dy; 

    if (this->rect.x < this->limit_xmin)
        return(BALL_EXIT_LEFT);

    if (this->rect.x > this->limit_xmax)
        return(BALL_EXIT_RIGHT);

	// Rebond sur partie haute ou basse
	if (this->rect.y < this->limit_ymin || this->rect.y > this->limit_ymax) {
		this->dy = -this->dy;
	}

    return(BALL_EXIT_NONE);
};

void Ball::rebond(int dy)
{
	// Accelerer la balle			
	this->dx +=  (this->dx < 0) ? -1: 1;
	//change ball direction
	this->dx = -this->dx;
				
    this->dy = dy;                
}

void Ball::configLimit(int xmin, int xmax, int ymin, int ymax)
{
    this->limit_xmin = xmin;
    this->limit_xmax = xmax;
    this->limit_ymin = ymin;
    this->limit_ymax = ymax;
}