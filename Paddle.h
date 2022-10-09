#pragma once
#include <cstddef>
#include <SDL2/SDL.h>

#define PADDLE_W 10
#define PADDLE_H 50
#define PADDLE_STEP 5

enum paddle_direction { PADDLE_DOWN, PADDLE_UP };

using namespace std;

class Paddle
{
private:
    SDL_Rect rect;  // x, y, w, h
    int limit_xmin; // Zone de mouvement du paddle
    int limit_xmax;
    int limit_ymin;
    int limit_ymax;

public:
    Paddle();
    int getX() const { return rect.x; };
    void setX(int x) { this->rect.x = x; };
    int getY() const { return rect.y; };
    void setY(int y);
    int getH() const { return rect.h; };
    SDL_Rect getRect() const { return this->rect; };
    void move( enum paddle_direction direction, int paddle_step );
    void paddle_goto( int paddle_goto );
    void configLimit(int xmin, int xmax, int ymin, int ymax);
};