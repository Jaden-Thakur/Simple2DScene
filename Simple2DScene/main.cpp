/**
* Author: Jaden Thakur
* Assignment: Simple 2D Scene
* Date Due: 2023-9-30, 11:59
* I pledge that I have completed this assignment without
* collaborationg with anyone else, in conformance with the
* NYU school of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>

#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

// Display Window
SDL_Window* displayWindow;
const int WINDOW_HEIGHT = 600,
WINDOW_WIDTH = 1000;

// Background Color
const float BG_RED = 0.953f,
BG_BLUE = 0.827f,
BG_GREEN = 0.741f,
BG_ALPHA = 1.0f;

// Viewport
const int VIEWPORT_X = 0,
VIEWPORT_Y = 0,
VIEWPORT_WIDTH = WINDOW_WIDTH,
VIEWPORT_HEIGHT = WINDOW_HEIGHT;

// Triangle 1
const float TR = 1.0,
TB = 0.4,
TG = 0.4,
TO = 1.0;


// Textures
const char V_SHADER_PATH[] = "shaders/vertex.glsl";
const char F_SHADER_PATH[] = "shaders/fragment.glsl";

//const char texture1[],
//           texture2[];

ShaderProgram g_program;
ShaderProgram g_program2;
glm::mat4 g_view_matrix; // cam orientation
glm::mat4 g_projection_matrix; //cam characteristics
glm::mat4 g_triangle1_matrix; //pos matrix of triangle 1
glm::mat4 g_triangle2_matrix; //pos matrix of triangle 2


bool gameIsRunning = true;


void initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Simple 2D Scene", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
    glewInit();
#endif

    glViewport(VIEWPORT_X, VIEWPORT_Y, VIEWPORT_WIDTH, VIEWPORT_HEIGHT); // initialize cam
    g_program.load(V_SHADER_PATH, F_SHADER_PATH); // loading shaders
    g_program2.load(V_SHADER_PATH, F_SHADER_PATH);


    g_view_matrix = glm::mat4(1.0f);
    g_triangle1_matrix = glm::mat4(1.0f);
    g_triangle2_matrix = glm::mat4(1.0f);
    g_projection_matrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

    g_program.set_view_matrix(g_view_matrix);
    g_program.set_projection_matrix(g_projection_matrix);
    g_program2.set_view_matrix(g_view_matrix);
    g_program2.set_projection_matrix(g_projection_matrix);

    g_program.set_colour(TR, TB, TG, TO);
    glUseProgram(g_program.get_program_id());
    g_program2.set_colour(1.0f, 1.0f, 1.0f, 1.0f);
    glUseProgram(g_program2.get_program_id());

    //Background color
    glClearColor(BG_RED, BG_BLUE, BG_GREEN, BG_ALPHA);
}


void process_input() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            gameIsRunning = false;
        }
    }
}

void update() {


    glClear(GL_COLOR_BUFFER_BIT);

    // Draw Shapes Here


    SDL_GL_SwapWindow(displayWindow);
}

void render() {
    glClear(GL_COLOR_BUFFER_BIT);

    g_program.set_model_matrix(g_triangle1_matrix);
    g_program2.set_model_matrix(g_triangle2_matrix);

    float vertices[] =
    {
        1.0f, -0.5f, 0.0f, 0.5f, -0.5f, -0.5f,
        3.0f, -0.5f, 3.0f, 0.5f, 2.5f, -0.5f
    };

    glVertexAttribPointer(g_program.get_position_attribute(), 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(g_program.get_position_attribute());
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableVertexAttribArray(g_program.get_position_attribute());


    glVertexAttribPointer(g_program2.get_position_attribute(), 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(g_program2.get_position_attribute());
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableVertexAttribArray(g_program2.get_position_attribute());


    SDL_GL_SwapWindow(displayWindow);

}

void shutdown() {
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    initialize();


    while (gameIsRunning) {

        process_input();

        // update();

        render();
    }

    shutdown();
    return 0;
}