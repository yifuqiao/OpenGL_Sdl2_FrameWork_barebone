#include <glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
	// allocate a console window
	AllocConsole();
	HANDLE handle_out = GetStdHandle(STD_OUTPUT_HANDLE);
	int hCrt = _open_osfhandle((long)handle_out, _O_TEXT);
	FILE* hf_out = _fdopen(hCrt, "w");
	setvbuf(hf_out, NULL, _IONBF, 1);
	*stdout = *hf_out;

	HANDLE handle_in = GetStdHandle(STD_INPUT_HANDLE);
	hCrt = _open_osfhandle((long)handle_in, _O_TEXT);
	FILE* hf_in = _fdopen(hCrt, "r");
	setvbuf(hf_in, NULL, _IONBF, 128);
	*stdin = *hf_in;

	// sdl stuff starts here
	// initial sdl video module ,parameter is a bit field, 
	SDL_Init(SDL_INIT_VIDEO);
	//Before doing anything else, first tell SDL that you want a forward compatible OpenGL 3.2 context:
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	/*The first argument specifies the title of the window, the next two are the X and Y position and the two after those are the width and height. 
	If the position doesn't matter, you can specify SDL_WINDOWPOS_UNDEFINED or SDL_WINDOWPOS_CENTERED for the second and third argument. 
	The final parameter specifies window properties like:

	SDL_WINDOW_OPENGL - Create a window ready for OpenGL.
	SDL_WINDOW_RESIZABLE - Create a resizable window.
	Optional SDL_WINDOW_FULLSCREEN - Create a fullscreen window.*/
	SDL_Window* window = SDL_CreateWindow("OpenGL", 100, 100, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(window);

	/*Now all that's left is calling glewInit() after the creation of your window and OpenGL context. 
	The glewExperimental line is necessary to force GLEW to use a modern OpenGL method for checking if a function is available.*/
	glewExperimental = GL_TRUE;
	// initial opengl
	glewInit();

	// test functions
	GLuint vertexBuffer;
	glGenBuffers(1, &vertexBuffer);

	printf("%u\n", vertexBuffer);

	//Then comes the most important part of the program, the event loop:
	SDL_Event windowEvent;
	while (true)
	{
		if (SDL_PollEvent(&windowEvent))
		{
			if (windowEvent.type == SDL_QUIT) break;
		}

		SDL_GL_SwapWindow(window);
	}
	SDL_GL_DeleteContext(context);

	SDL_Quit();
	return 0;
}