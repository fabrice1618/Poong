#include <SDL2/SDL.h>

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <iostream>

#include "Ball.h"
#include "Paddle.h"
#include "PongWindow.h"

#define SCORE_WIN 3
#define LOOP_TIME 20	// 50 frames par seconde

enum game_state { 
	GAME_MENU, 
	GAME_START, 
	GAME_LOOP, 
	GAME_OVER, 
	GAME_QUIT, 
};

// Program globals
Ball ball;
Paddle paddle[2];
PongWindow pongWindow;

int score[] = {0,0};
enum game_state state = GAME_MENU;

//initialise starting position and sizes of game elemements
static void init_game() 
{
	int screen_width = pongWindow.getScreenW(); 
	int screen_height = pongWindow.getScreenH();

	ball.setX(screen_width / 2);
	ball.setY(screen_height / 2);
	ball.setDX(1);
	ball.setDY(1);

	paddle[0].setX(20);
	paddle[0].setY(screen_height / 2 - 50);

	paddle[1].setX(screen_width - 20 - 10);
	paddle[1].setY(screen_height / 2 - 50);
}

// return the winner number, return 0 if no one wins
int check_score() 
{
	for(int i = 0; i < 2; i++) 
		if ( score[i] == SCORE_WIN ) 
			return(i+1);

	return 0;
}

static void move_ball() 
{
	enum ball_exit sortie;
	SDL_Rect ball_rect;
	SDL_Rect paddle_rect;

	/* Move the ball by its motion vector. */
	sortie = ball.move();
	
	// Balle sortie laterale
	if (sortie == BALL_EXIT_LEFT) {
		score[1] += 1;
		state  = GAME_START;
		return;
	}
	if (sortie == BALL_EXIT_RIGHT) { 
		score[0] += 1;
		state  = GAME_START;
		return;
	}

	ball_rect = ball.getRect();

	//check for collision with the paddle
	for (int i = 0; i < 2; i++) {
		paddle_rect = paddle[i].getRect();

		//collision detected
		if ( SDL_HasIntersection(&ball_rect, &paddle_rect) ) {

			//change ball angle based on where on the paddle it hit
			int hit_pos = (paddle[i].getY() + paddle[i].getH()) - ball.getY();
			int ball_dy = 4 - (int)(hit_pos / 7);

			ball.rebond(ball_dy);
		}
	}
}

static void move_paddle_ai() 
{
	int screen_height = pongWindow.getScreenH();
	
	if (ball.getDX() > 0) {
		//ball moving right	-> return to center position
		paddle[0].paddle_goto(screen_height / 2);
	} else {
		//ball moving left -> folllow ball
		paddle[0].paddle_goto(ball.getY());
	}
}

void resolution_change()
{
	int screen_width = pongWindow.getScreenW(); 
	int screen_height = pongWindow.getScreenH();

	// Fixer la zone de mouvement de la balle et du paddle
	ball.configLimit(0, screen_width - 10, 0, screen_height - 10);
	paddle[0].configLimit(20, (screen_width / 2) - 10, 0, screen_height - PADDLE_H);
	paddle[1].configLimit((screen_width / 2) + 10, screen_width -20 - 10, 0, screen_height - PADDLE_H);
}

int main (int argc, char *args[]) 
{
	enum pong_resolution resolution = SIZE_800_600;
	int sleep_time = 0;
	Uint32 next_game_tick = SDL_GetTicks();
	SDL_Event evenements;

	// Recherche parametre -f pour fullscreen
	for (int i = 0; i < argc; i++) {
		if ( strcmp(args[i], "-f") == 0 ) {
			resolution = FULLSCREEN;
		}
	}

	//SDL Window setup
	if ( ! pongWindow.open_window(resolution) ) {
		cerr << "Erreur open_window\n";
		return EXIT_FAILURE;
	}
	
	resolution_change();

	//render loop
	while(state != GAME_QUIT) {

		// Fermeture de la fenetre
		while ( (state != GAME_QUIT) && SDL_PollEvent(&evenements) ) {
			if ((evenements.type==SDL_WINDOWEVENT) && (evenements.window.event == SDL_WINDOWEVENT_CLOSE))
				state = GAME_QUIT;
		}

		// Traiter les événements clavier
		const Uint8 *keystate = SDL_GetKeyboardState(NULL);
		
		if (keystate[SDL_SCANCODE_ESCAPE]) {
			state = GAME_QUIT;
		}
		switch(state) {
			case GAME_MENU:
				if (keystate[SDL_SCANCODE_0] || keystate[SDL_SCANCODE_KP_0] ) {
					pongWindow.close_window();
					pongWindow.open_window(FULLSCREEN);
					resolution_change();
				}
				if (keystate[SDL_SCANCODE_1] || keystate[SDL_SCANCODE_KP_1] ) {
					pongWindow.close_window();
					pongWindow.open_window(SIZE_640_480);
					resolution_change();
				}
				if (keystate[SDL_SCANCODE_2] || keystate[SDL_SCANCODE_KP_2] ) {
					pongWindow.close_window();
					pongWindow.open_window(SIZE_800_600);
					resolution_change();
				}
				if (keystate[SDL_SCANCODE_3] || keystate[SDL_SCANCODE_KP_3] ) {
					pongWindow.close_window();
					pongWindow.open_window(SIZE_1024_768);
					resolution_change();
				}
				if (keystate[SDL_SCANCODE_SPACE])
					state = GAME_START;
				break;
			case GAME_LOOP:
				if (keystate[SDL_SCANCODE_DOWN]) {
					paddle[1].move(PADDLE_DOWN, PADDLE_STEP);
				}
				if (keystate[SDL_SCANCODE_UP]) {
					paddle[1].move(PADDLE_UP, PADDLE_STEP);
				}
				if (keystate[SDL_SCANCODE_A]) {
					paddle[1].move(PADDLE_DOWN, PADDLE_STEP);
				}
				if (keystate[SDL_SCANCODE_Q]) {
					paddle[1].move(PADDLE_UP, PADDLE_STEP);
				}
				break;
			case GAME_OVER:
				if (keystate[SDL_SCANCODE_SPACE]) {
					state = GAME_START;
					score[0] = 0;	//reset scores
					score[1] = 0;
				}
				break;
		}

		if ( pongWindow.drawBackground() == 1 ) {
			cerr << "Erreur drawBackground\n";
			return EXIT_FAILURE;
		}

		switch(state) {
			case GAME_MENU:
				pongWindow.screen_menu();
				break;

			case GAME_START:
				init_game();
				state = GAME_LOOP;
				break;

			case GAME_LOOP:
				//check score
				if (check_score() != 0) {
					state = GAME_OVER;
				} else {
					//paddle ai movement
					move_paddle_ai();

					//* Move the balls for the next frame. 
					move_ball();					
					if ( state != GAME_START ) {
						pongWindow.draw_net();

						for (int i = 0; i < 2; i++) {
							pongWindow.draw_paddle( paddle[i].getRect() );
						}

						pongWindow.draw_ball( ball.getRect() );
						pongWindow.draw_score(score[0], score[1]);
					}
				}
				break;

			case GAME_OVER:
				if ( check_score() == 1 )
					pongWindow.screen_game_over("Machine wins");
				else
					pongWindow.screen_game_over("Player 2 wins");

				break;

			case GAME_QUIT:
				break;
		}

		pongWindow.update();

		//time it takes to render  frame in milliseconds
		next_game_tick += LOOP_TIME;
		sleep_time = next_game_tick - SDL_GetTicks();
		if ( sleep_time > 0 ) {
			SDL_Delay(sleep_time);
		}

	}

	pongWindow.close_window();

	cout << "A++\n";
	return EXIT_SUCCESS;
}

