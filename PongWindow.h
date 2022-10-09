#pragma once
#include <cstddef>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
extern DECLSPEC int SDLCALL TTF_Init(void);

#define COLOR_WHITE 0xffffffff
#define COLOR_BLACK 0x000000ff

using namespace std;

enum pong_resolution { FULLSCREEN, SIZE_640_480, SIZE_800_600, SIZE_1024_768};

class PongWindow
{
private:
    enum pong_resolution resolution;
    SDL_Window* window = NULL;	
    SDL_Renderer* renderer;
    SDL_Surface* screen;
    SDL_Texture* screen_texture;
    TTF_Font* fontBig;
    TTF_Font* fontNormal;

public:
    PongWindow();
    int open_window(enum pong_resolution resolution);
    void close_window();
    int getScreenH() const { return this->screen->h; };
    int getScreenW() const { return this->screen->w; };
    void drawTextCenter( int centerX, int centerY, const char* message, TTF_Font* police );
    int drawBackground();
    void screen_menu();
    void screen_game_over(const char* messageWinner);
    void update();
    void draw_score(int score1, int score2);
    void draw_net();
    void draw_ball(SDL_Rect ball_rect);
    void draw_paddle(SDL_Rect paddle_rect);
private:    
    int init_screen( int screen_width, int screen_height, Uint32 windowFlags);
};
