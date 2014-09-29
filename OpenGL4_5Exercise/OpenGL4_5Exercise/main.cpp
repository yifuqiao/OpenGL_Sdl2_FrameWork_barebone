#include <glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <io.h>
#include <fcntl.h>
#include "FileReader.h"
#include "Headers.h"



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

	// Create Vertex Array Object
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	/*GLuint elements[] = {
		0, 1, 2
	};*/
	GLuint elements[] = {
		0, 1, 2,
		2, 3, 0
	};

	// order the existing vertices and reuse them
	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		sizeof(elements), elements, GL_STATIC_DRAW);

	// Create a Vertex Buffer Object and copy the vertex data to it
	GLuint vbo;
	glGenBuffers(1, &vbo);

	/*GLfloat vertices[] = {
		0.0f, 0.5f,
		0.5f, -0.5f,
		-0.5f, -0.5f
	};*/

	/*GLfloat vertices[] = {
		0.0f, 0.5f, 1.0f, 0.0f, 0.0f, // Vertex 1: Red
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // Vertex 2: Green
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f  // Vertex 3: Blue
	};*/

	float vertices[] = {
		-0.5f, 0.5f, 1.0f, 0.0f, 0.0f, // Top-left
		0.5f, 0.5f, 0.0f, 1.0f, 0.0f, // Top-right
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // Bottom-right
		-0.5f, -0.5f, 1.0f, 1.0f, 1.0f  // Bottom-left
	};

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Create and compile the vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const GLchar* vertexSource = FileReader::Read("Shaders/vertexshader.vertex");
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	// Create and compile the fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const GLchar* fragmentSource = FileReader::Read("Shaders/fragmentShader.fragment");
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	// Link the vertex and fragment shader into a shader program
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glBindFragDataLocation(shaderProgram, 0, "outColor");
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	// Specify the layout of the vertex data
	/*GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
	*/
	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE,
		5 * sizeof(GLfloat), 0);

	GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE,
		5 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));

//	GLint uniColor = glGetUniformLocation(shaderProgram, "triangleColor");
//	glUniform3f(uniColor, 1.0f, 0.0f, 0.0f);

	//Then comes the most important part of the program, the event loop:
	SDL_Event windowEvent;
	while (true)
	{
		if (SDL_PollEvent(&windowEvent))
		{
			if (windowEvent.type == SDL_QUIT) break;
		}
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// to do : need to get a batter accurate time implementation
		float time = (float)clock() / (float)CLOCKS_PER_SEC;
		
		//glUniform3f(uniColor, (sin(time * 4.0f) + 1.0f) / 2.0f, 0.0f, 0.0f);


		// Draw a triangle from the 3 vertices
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
		SDL_GL_SwapWindow(window);
	}
	SDL_GL_DeleteContext(context);


	glDeleteProgram(shaderProgram);
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);

	glDeleteBuffers(1, &vbo);

	glDeleteVertexArrays(1, &vao);

	SDL_Quit();
	return 0;
}