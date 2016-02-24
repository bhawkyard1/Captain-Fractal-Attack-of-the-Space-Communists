#include "../common.hpp"

//Starts up SDL, creates window, and initializes OpenGL
bool init();

//Initializes matrices and clear color
bool initGL();

//Per frame update
void update();

//Renders quad to the screen
void render();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window * gWindow = NULL;

//OpenGL context
SDL_GLContext gContext;

//Render flag
bool gRenderQuad = true;

bool init()
{
	//Initialization flag
    bool success = true;

    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) != 0 )
    {
        std::cout << "SDL could not initialize! SDL Error: " << SDL_GetError() << std::endl;
        success = false;
    }
    else
    {
        //Use OpenGL 2.1
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
        SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );

        //Create window
        gWindow = SDL_CreateWindow( "Elite Dangerous 2.0", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
        if( gWindow == NULL )
        {
            std::cout << "Window could not be created! SDL Error: " << SDL_GetError() << std::endl;
            success = false;
        }
        else
        {
            //Create context
            gContext = SDL_GL_CreateContext( gWindow );
            if( gContext == NULL )
            {
                std::cout << "OpenGL context could not be created! SDL Error: " << SDL_GetError() << std::endl;
                success = false;
            }
            else
            {
                //Use Vsync
                if( SDL_GL_SetSwapInterval( 1 ) < 0 )
                {
                    std::cout << "Warning: Unable to set VSync! SDL Error: " << SDL_GetError() << std::endl;
                }

                //Initialize OpenGL
                if( !initGL() )
                {
                    std::cout << "Unable to initialize OpenGL! " << std::endl;
                    success = false;
                }
            }
        }
    }

    return success;
}

bool initGL()
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
		std::cout << "Error initializing OpenGL! " << gluErrorString( error ) << std::endl;
		success = false;
	}
	
	//Initialize Modelview Matrix
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	
	//Check for error
	error = glGetError();
	if( error != GL_NO_ERROR )
	{
		std::cout << "Error initializing OpenGL! " << gluErrorString( error ) << std::endl;
		success = false;
	}
	
	//Initialize clear color
	glClearColor( 0.f, 0.f, 0.f, 1.f );
	
	//Check for error
	error = glGetError();
	if( error != GL_NO_ERROR )
	{
		std::cout << "Error initializing OpenGL! " << gluErrorString( error ) << std::endl;
		success = false;
	}
    
	return success;
}

void render()
{
    //Clear color buffer
    glClear( GL_COLOR_BUFFER_BIT );
    
    //Render quad
    if( gRenderQuad )
    {
        glBegin( GL_QUADS );
            glVertex3f( -0.5f, -0.5f, 1.0f);
            glVertex3f( 0.5f, -0.5f, 1.0f );
            glVertex3f( 0.5f, 0.5f, 0.0f);
            glVertex3f( -0.5f, 0.5f, 0.0f);
        glEnd();
    }
}
