
#include "gl_utilities.h"

#include <iostream>
#include <SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <OpenGL/glu.h>

#include <stdio.h>
#include <string>
#include <fstream>
#include <set>
#include <iostream>
#include <map>
#include <chrono>
#include <list>
#include <sstream>
#include <vector>
#include <cassert>
#include <cmath>

using std::runtime_error;



int main() {
	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
		throw runtime_error(concat("SDL could not initialize! SDL Error: ", SDL_GetError()));

	//Use OpenGL 3.2 core
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 2 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
	SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );

	//Create window
	SDL_Window* gWindow = SDL_CreateWindow("Fuck Pink", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
	if( gWindow == NULL )
		throw runtime_error(concat("Window could not be created! SDL Error:", SDL_GetError() ));

	//Create context
	SDL_GLContext gContext = SDL_GL_CreateContext( gWindow );
	if( gContext == NULL )
		throw runtime_error(concat("OpenGL context could not be created! SDL Error: ", SDL_GetError() ));

	check_gl_error();





	//Initialize GLEW
	glewExperimental = GL_TRUE; 
	GLenum glewError = glewInit();
	if( glewError != GLEW_OK )
		throw runtime_error(concat("Error initializing GLEW! ", glewGetErrorString( glewError ) ));

	// This returns an error, which is fine, see http://www.opengl.org/wiki/OpenGL_Loading_Library :
	// "You might still get GL_INVALID_ENUM (depending on the version of GLEW you use), but at least GLEW ignores glGetString(GL_EXTENSIONS)​ and gets all function pointers."
	check_gl_error();

	//Use Vsync
	if( SDL_GL_SetSwapInterval( 1 ) < 0 )
		printf( "Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError() );



	SDL_StartTextInput();





	check_gl_error();

	float red = .5;
	float green = .8;
	float blue = .3;
	glClearColor(red, green, blue, 1.0);
	check_gl_error();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	check_gl_error();


	SDL_GL_SwapWindow( gWindow );

	char moo;
	std::cin >> moo;

	//Destroy window	
	SDL_DestroyWindow( gWindow );
	check_gl_error();
	gWindow = NULL;





	//Disable text input
	SDL_StopTextInput();

	//Quit SDL subsystems
	SDL_Quit();
}
