#include <SDL2/SDL_ttf.h>
extern DECLSPEC int SDLCALL TTF_Init(void);

#include <stdio.h>

#include "PongWindow.h"

PongWindow::PongWindow()
{

}

int PongWindow::open_window(enum pong_resolution resolution)
{
	Uint32 windowFlags = 0;
	int screen_width; 
	int screen_height;

    this->resolution = resolution;

	switch(this->resolution) {
		case SIZE_640_480:			
			screen_width = 640;
			screen_height = 480;
			break;
		case SIZE_800_600:			
			screen_width = 800;
			screen_height = 600;
			break;
		case SIZE_1024_768:			
			screen_width = 1024;
			screen_height = 768;
			break;
		case FULLSCREEN:			
			windowFlags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
			screen_width = 800;
			screen_height = 600;
			break;
	}

	if ( !this->init_screen( screen_width, screen_height, windowFlags) ) {
		return 1;
	}
	return 0;
}

int PongWindow::drawBackground()
{
	//draw background
	SDL_RenderClear(renderer);
	SDL_FillRect(screen, NULL, COLOR_BLACK);

	SDL_Rect bordure;
		
	bordure.x = 0;
	bordure.y = 0;
	bordure.w = screen->w;
	bordure.h = screen->h;

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	if ( SDL_RenderDrawRect(renderer, &bordure) != 0 ) {
		printf("SDL_RenderDrawRect: %s\n", SDL_GetError());
		return 1;
	}
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	return(0);
}

void PongWindow::update()
{
	//draw to the display
	SDL_UpdateTexture(screen_texture, NULL, screen->pixels, screen->w * sizeof (Uint32));
	SDL_RenderCopy(renderer, screen_texture, NULL, NULL);
	SDL_RenderPresent(renderer);
}

void PongWindow::screen_game_over(const char* messageWinner) 
{
	int posX, posY;

	posX = screen->w/2;
	posY = screen->h/2;

	drawTextCenter( posX, posY, messageWinner, fontBig );
	drawTextCenter( posX, posY+90, "Game over", fontNormal );
}

void PongWindow::draw_score(int score1, int score2)
{
	char str_score[] = "?";

	if (score1 <= 9) {
		str_score[0] = '0' + score1;
		drawTextCenter( (screen->w/2) - 55, 20, str_score, fontBig );
	}

	if (score1 <= 9) {
		str_score[0] = '0' + score2;
		drawTextCenter( (screen->w/2) + 55, 20, str_score, fontBig );
	}
}

void PongWindow::draw_net() 
{
	SDL_Rect net;
	
	net.x = screen->w / 2;
	net.y = 0;
	net.w = 5;
	net.h = 15;

	for(int posY = 20; posY < screen->h-30; posY += 30) {
		net.y = posY;
		
		if (SDL_FillRect(screen, &net, COLOR_WHITE) != 0) { 
			printf("fill rectangle failed in func draw_net()");
		}
	}
}

void PongWindow::draw_ball(SDL_Rect ball_rect) 
{
	if (SDL_FillRect(screen , &ball_rect, COLOR_WHITE) !=0) {
		printf("fill rectangle failed in func draw_ball()");
	}
}

void PongWindow::draw_paddle(SDL_Rect paddle_rect) 
{
	if (SDL_FillRect(screen, &paddle_rect, COLOR_WHITE) !=0){
		printf("fill rectangle failed in func draw_paddle()");
	}
}

void PongWindow::screen_menu() 
{
	int titleX, titleY;

	titleX = screen->w/2;
	titleY = screen->h/2;

	drawTextCenter( titleX, titleY, "Poong", fontBig );
	drawTextCenter( titleX, titleY+50, "<space> to play game", fontNormal );
	drawTextCenter( titleX, titleY+80, "<0> Fullscreen", fontNormal );
	drawTextCenter( titleX, titleY+110, "<1> 640 * 480", fontNormal );
	drawTextCenter( titleX, titleY+140, "<2> 800 * 600", fontNormal );
	drawTextCenter( titleX, titleY+170, "<3> 1024 * 768", fontNormal );
	drawTextCenter( titleX, titleY+200, "<esc> to quit game", fontNormal );
}


int PongWindow::init_screen( int screen_width, int screen_height, Uint32 windowFlags) 
{
	int width, height;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return 1;
	}
	
	//Create window	and renderer
	SDL_CreateWindowAndRenderer(screen_width, screen_height, windowFlags, &window, &renderer);
	if (window == NULL) { 
		printf("SDL_CreateWindowAndRenderer() SDL_Error: %s\n", SDL_GetError());
		return 1;
	}
	SDL_GetWindowSize(window, &width, &height);

	//create the screen surface where all the elemnts will be drawn onto (ball, paddles, net etc)
	screen = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, SDL_PIXELFORMAT_RGBA32);
	if (screen == NULL) {
		printf("Could not create the screen surface! SDL_Error: %s\n", SDL_GetError());
		return 1;
	}

	//create the screen texture to render the screen surface to the actual display
	screen_texture = SDL_CreateTextureFromSurface(renderer, screen);
	if (screen_texture == NULL) {
		printf("Could not create the screen_texture! SDL_Error: %s\n", SDL_GetError());
		return 1;
	}

	if (TTF_Init() < 0) {
    	printf( "Error ttf_init: %s\n", SDL_GetError() );
		return 1;
	}

	fontBig = TTF_OpenFont("ArcadeFuture.otf", 72);
	if(!fontBig) {
		printf("TTF_OpenFont: %s\n", SDL_GetError());
		return 1;
	}

	fontNormal = TTF_OpenFont("ArcadeFuture.otf", 24);
	if(!fontNormal) {
		printf("TTF_OpenFont: %s\n", SDL_GetError());
		return 1;
	}

	return 0;
}


void PongWindow::drawTextCenter( int centerX, int centerY, const char* message, TTF_Font* police )
{
	SDL_Surface *text_surface;
	SDL_Rect src;
	SDL_Rect dest;
	SDL_Color gameColor = {255,255,255,255};

	if (!(text_surface=TTF_RenderText_Blended( police, message, gameColor ) ) ) {
		printf("[TTF_RenderText_Blended] > %s", SDL_GetError());
	} else {
		src.x = 0;
		src.y = 0;
		src.w = text_surface->w;
		src.h = text_surface->h;

		dest.x = centerX - (src.w / 2);
		dest.y = centerY - (src.h / 2);
		dest.w = src.w;
		dest.h = src.h;

		SDL_BlitSurface( text_surface, &src, screen, &dest );
	}

	SDL_FreeSurface(text_surface);
}

void PongWindow::close_window()
{

	SDL_FreeSurface(screen);
	//free renderer and all textures used with it
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	TTF_CloseFont(fontBig);
	TTF_CloseFont(fontNormal);

	//Quit SDL subsystems 
	TTF_Quit();	
	SDL_Quit(); 

}