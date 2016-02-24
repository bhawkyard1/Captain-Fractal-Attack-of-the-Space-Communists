//General headers
#include <iostream>

//openGL headers
#include <GL/glew.h>
#include <GL/glu.h> 

//SDL headers
#include "SDL2/SDL.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_opengl.h>

#include "vectors.hpp"
#include "common.hpp"

#include "utility/renderer.cpp"
#include "utility/user_input.hpp"
#include "sim_time.cpp"

#ifdef _WIN32
#include <windows.h>
#endif

int main(int argc, char * argv[])
{	
	renderer r (WIN_WIDTH, WIN_HEIGHT);
	
	bool active = true;
	//Enable text input
	SDL_StartTextInput();

	sim_time clock (60.0f);
	clock.setStart();
	
	//While application is running
	while( active )
	{	
		SDL_Event e;
		//Handle events on queue
		while( SDL_PollEvent( &e ) != 0 )
		{
			//User requests quit
			if( e.type == SDL_QUIT )
			{
				active = false;
			}
			//Handle keypress with current mouse position
			else if( e.type == SDL_TEXTINPUT )
			{
				int x = 0, y = 0;
				SDL_GetMouseState( &x, &y );
				handleKeys( e.text.text[ 0 ], x, y, r );
			}
		}

		//Render game
		float diff_clamped = clock.getDiff();
		if(diff_clamped == 0.0f) diff_clamped = 0.01f;
		
		g_GLOBAL_TIME = clock.getTime();
				
		r.render( clock.getAcc() / diff_clamped, g_GLOBAL_TIME );

		clock.setCur();
	}
	
	r.close();
	//Disable text input
	SDL_StopTextInput();
        
	return 0;
}

/*
g++ -Wall -o "%e" -lmingw32  -mwindows -L C:\SDL2\x86_64-w64-mingw32\lib "%f" -I C:\SDL2\x86_64-w64-mingw32\include -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lSDL2main -lSDL2 -std=c++11

g++ -Wall -o "%e"  -L usr\bin\lib "%f" -I usr\bin\include -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lSDL2main -lSDL2 -lglut -lGL -lGLU -lGLEW -std=c++11
*/


