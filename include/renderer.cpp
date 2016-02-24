#ifndef CLASS_RENDERER
#define CLASS_RENDERER
#include "renderer.hpp"
#include "util.hpp"

renderer::renderer(int w, int h)
{
	if(!init(w, h) or !initGL())
	{
		std::cerr << "There was an error in the initialisation of the renderer! Please contact the support desk." << std::endl;
		close();
	}
}

bool renderer::init(int w, int h)
{
	//Initialization flag
    bool success = true;

    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) != 0 )
    {
        std::cerr << "SDL could not initialize! SDL Error: " << SDL_GetError() << std::endl;
        success = false;
    }
    else
    {
        //Use OpenGL 2.1
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );

        //Create window
        gWindow = SDL_CreateWindow( "Elite Dangerous 2.0", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
        if( gWindow == NULL )
        {
            std::cerr << "Window could not be created! SDL Error: " << SDL_GetError() << std::endl;
            success = false;
        }
        else
        {
            //Create context
            gContext = SDL_GL_CreateContext( gWindow );
            if( gContext == NULL )
            {
                std::cerr << "OpenGL context could not be created! SDL Error: " << SDL_GetError() << std::endl;
                success = false;
            }
            else
            {
                //Use Vsync
                if( SDL_GL_SetSwapInterval( 1 ) < 0 )
                {
                    std::cerr << "Warning: Unable to set VSync! SDL Error: " << SDL_GetError() << std::endl;
                }
            }
        }
    }

    return success;
}

bool renderer::initGL()
{
	bool success = true;
	GLenum error = GL_NO_ERROR;
	
	//Initialize Projection Matrix
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	
	//Check for error
	error = glGetError();
	if( error != GL_NO_ERROR )
	{
		std::cerr << "Error initializing OpenGL! " << gluErrorString( error ) << std::endl;
		success = false;
	}
	
	//Initialize Modelview Matrix
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	
	//Check for error
	error = glGetError();
	if( error != GL_NO_ERROR )
	{
		std::cerr << "Error initializing OpenGL! " << gluErrorString( error ) << std::endl;
		success = false;
	}
	
	//Initialize clear color
	glClearColor( 1.f, 0.f, 0.f, 1.f );
	
	//Check for error
	error = glGetError();
	if( error != GL_NO_ERROR )
	{
		std::cerr << "Error initializing OpenGL! " << gluErrorString( error ) << std::endl;
		success = false;
	}
    
	return success;
}

void renderer::render(float dt, double gt)
{
    //Clear color buffer
    glClear( GL_COLOR_BUFFER_BIT );
    
    //Render quad
    if( gRenderQuad )
    {
		double globRad = rad( g_GLOBAL_TIME ) * 50.0;
        glBegin( GL_QUADS );
			/*
			//Top left
            glVertex3f( -sin(globRad), -0.5f, 0.f);
            //Top right
            glVertex3f( 0.5f, -0.5f, 0.f );
            //Bottom left
            glVertex3f( -0.5f, 0.5f, 0.f);
            //Bottom right
            glVertex3f( 0.5f, 0.5f, 0.f);
            */
            
			//Top left
            glVertex3f( -cos(globRad) * .5f, -.5f, sin(globRad) * .5f );
            //Top right
            glVertex3f( cos(globRad) * .5f, -.5f, -sin(globRad) * .5f );
            //Bottom left
            glVertex3f( -cos(globRad) * .5f, .5f, sin(globRad) * .5f );
            //Bottom right
            glVertex3f( cos(globRad) * .5f, .5f, -sin(globRad) * .5f );
        glEnd();
    }
    
    SDL_GL_SwapWindow( gWindow );
}

void renderer::close()
{
	SDL_Quit();
}

renderer::~renderer()
{
	//Do code
}

#endif
