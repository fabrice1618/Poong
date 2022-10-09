#include "Paddle.h"


Paddle::Paddle()
{
    rect.x = 0;
    rect.y = 0;
    rect.w = PADDLE_W;
    rect.h = PADDLE_H;
    this->limit_xmin = 0;
    this->limit_xmax = 0;
    this->limit_ymin = 0;
    this->limit_ymax = 0;
}

void Paddle::setY(int y) 
{ 
	if ( y <= this->limit_ymin ) {
		y = this->limit_ymin;
	}
	if (y >= this->limit_ymax) {
		y = this->limit_ymax;
	}

    this->rect.y = y; 
}

void Paddle::move( enum paddle_direction direction, int paddle_step )
{

	if (paddle_step != 0 && direction == PADDLE_DOWN) {
		this->setY( this->rect.y + paddle_step );

	}
    if (paddle_step != 0 && direction == PADDLE_UP) {
        this->setY( this->rect.y - paddle_step );
	}
}

void Paddle::paddle_goto( int paddle_goto )
{
	int paddle_center;
	int paddle_speed;
	enum paddle_direction paddle_direction;
	int distance;
	
	paddle_center = this->rect.y + (PADDLE_H / 2);

	distance = paddle_goto - paddle_center;

	paddle_direction = (distance > 0) ? PADDLE_DOWN: PADDLE_UP;

	paddle_speed = abs(distance);
	paddle_speed = (paddle_speed > 5) ? 5 : paddle_speed;

	this->move(paddle_direction, paddle_speed);    
}

void Paddle::configLimit(int xmin, int xmax, int ymin, int ymax)
{
    this->limit_xmin = xmin;
    this->limit_xmax = xmax;
    this->limit_ymin = ymin;
    this->limit_ymax = ymax;
}