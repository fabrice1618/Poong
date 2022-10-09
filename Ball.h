#pragma once
#include <cstddef>
#include <SDL2/SDL.h>

#define BALL_W 10
#define BALL_H 10

enum ball_exit { BALL_EXIT_NONE, BALL_EXIT_DOWN, BALL_EXIT_UP, BALL_EXIT_LEFT, BALL_EXIT_RIGHT };

using namespace std;

class Ball
{
private:
    SDL_Rect rect;  // x, y, w, h
    int dx; /* movement vector (dx, dy)*/
    int dy;
    int limit_xmin; // Zone de mouvement de la balle
    int limit_xmax;
    int limit_ymin;
    int limit_ymax;

public:
    Ball();
    int getX() const { return rect.x; };
    void setX(int x) { this->rect.x = x; };
    int getY() const { return rect.y; };
    void setY(int y) { this->rect.y = y; };
    SDL_Rect getRect() const { return this->rect; };
    enum ball_exit move();
    int getDX() const { return dx; };
    void setDX(int dx) { this->dx = dx; };
    int getDY() const { return dy; };
    void setDY(int dy) { this->dy = dy; };
    void rebond(int dy);
    void configLimit(int xmin, int xmax, int ymin, int ymax);
};