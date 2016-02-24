#include <math.h>

class renderer
{
	//The window we'll be rendering to
	SDL_Window * gWindow = NULL;

	//OpenGL context
	SDL_GLContext gContext;

	//Render flag
	bool gRenderQuad = true;
	
	//std::map< ngl::model > models;
	//std::map< ngl::texture > textures;
public:
	renderer(int, int);
	~renderer();
	bool init(int, int);
	bool initGL();
	void render(float, double);
	void close();
	
	void DELETE_rflip() {gRenderQuad = !gRenderQuad;}
	bool DELETE_gflip() {return gRenderQuad;}
};

