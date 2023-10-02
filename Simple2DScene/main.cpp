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
#define GL_GLEXT_PROTOTYPES 1
#define LOG(argument) std::cout << argument << std::endl;
#define STB_IMAGE_IMPLEMENTATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>

#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

#include "stb_image.h"

// Useful values:
const float MILLISECONDS_IN_SECOND = 1000.0f;


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
const char V_SHADER_PATH[] = "shaders/vertex_textured.glsl";
const char F_SHADER_PATH[] = "shaders/fragment_textured.glsl";

const char texture1[] = "assets/godot.png",
          texture2[] = "assets/unity.png";

GLuint object_texture_id;
GLuint object2_texture_id;

ShaderProgram g_program;
// ShaderProgram g_program2;
glm::mat4 g_view_matrix; // cam orientation
glm::mat4 g_projection_matrix; //cam characteristics
glm::mat4 g_object1_matrix; //pos matrix of object 1
glm::mat4 g_object2_matrix; //pos matrix of object 2

// Tranforms
glm::vec3 g_object1_position = glm::vec3(0.0f, 0.0f, 0.0f);
float g_object1_x = 0.0f;
float g_object1_y = 0.0f;
float g_object1_rotate = 0.0f;

glm::vec3 g_object2_position = glm::vec3(0.0f, 0.0f, 0.0f);
float g_object2_x = 0.0f;
float g_object2_y = 0.0f;
float g_object2_rotate = 0.0f;

// glm::vec3 g_object_movement = glm::vec3(0.0f, 0.0f, 0.0f);

bool gameIsRunning = true;

const int NUMBER_OF_TEXTURES = 1; // to be generated, that is
const GLint LEVEL_OF_DETAIL = 0; // base image level; Level n is the nth mipmap reduction image
const GLint TEXTURE_BORDER = 0; // this value MUST be zero





GLuint load_texture(const char* filepath) {
    int width, height, number_of_components;
    unsigned char* image = stbi_load(filepath, &width, &height, &number_of_components, STBI_rgb_alpha);

    if (image == NULL) {
        LOG("Unable to load image");
        assert(false);
    }

    GLuint textureID;
    glGenTextures(NUMBER_OF_TEXTURES, &textureID);

    glBindTexture(GL_TEXTURE_2D, textureID);


    glTexImage2D(
        GL_TEXTURE_2D, LEVEL_OF_DETAIL, GL_RGBA,
        width, height,
        TEXTURE_BORDER, GL_RGBA, GL_UNSIGNED_BYTE,
        image
    );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    stbi_image_free(image);
    return textureID;
}


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
    //g_program2.load(V_SHADER_PATH, F_SHADER_PATH);

    object_texture_id = load_texture(texture1);
    object2_texture_id = load_texture(texture2);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    g_view_matrix = glm::mat4(1.0f);
    g_object1_matrix = glm::mat4(1.0f);
    g_object1_matrix = glm::translate(g_object1_matrix, glm::vec3(-4.0f, -2.0f, 0.0f));
    g_object2_matrix = glm::translate(g_object1_matrix, glm::vec3(0.0f, 0.75f, 0.0f));
    g_projection_matrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

    g_program.set_view_matrix(g_view_matrix);
    g_program.set_projection_matrix(g_projection_matrix);

    /*
    g_program2.set_view_matrix(g_view_matrix);
    g_program2.set_projection_matrix(g_projection_matrix);
    */


    //g_program.set_colour(TR, TB, TG, TO);
    glUseProgram(g_program.get_program_id());
    /*
    g_program2.set_colour(1.0f, 1.0f, 1.0f, 1.0f);
    glUseProgram(g_program2.get_program_id());
    */
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

float g_previous_ticks = 0.0f;
int frames = 0;
int let_go_frame = 1500;
bool moving_right = true;

void shutdown();

void update() {


    glClear(GL_COLOR_BUFFER_BIT);

    float ticks = (float)SDL_GetTicks() / MILLISECONDS_IN_SECOND;
    float delta_time = ticks - g_previous_ticks;
    g_previous_ticks = ticks;

    

    
    // Do transformations here

    
    g_object1_x = 3.0f * delta_time;
    g_object1_y = 1.5f * delta_time;
    g_object2_x += 1.0f * delta_time;
    g_object2_y += 1.0f * delta_time;
    g_object1_rotate = 10.0f * delta_time;
    g_object2_rotate += 90.0f * delta_time;
  
    frames++;
    if (frames <= let_go_frame) {
        g_object1_matrix = glm::translate(g_object1_matrix, glm::vec3(g_object1_x, g_object1_y, 0.0f));
        g_object2_matrix = glm::translate(g_object1_matrix, glm::vec3(0.0f, 0.75f, 0.0f));
    }
    else {
        g_object2_matrix = glm::translate(g_object1_matrix, glm::vec3(g_object2_x, g_object2_y, 0.0f));
        g_object2_matrix = glm::rotate(g_object2_matrix, glm::radians(g_object2_rotate), glm::vec3(0.0f, 0.0f, 1.0f));
    }

   
   
}

void draw_object(glm::mat4& object_model_matrix, GLuint& object_texture_id) {
    g_program.set_model_matrix(object_model_matrix);
    glBindTexture(GL_TEXTURE_2D, object_texture_id);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}


void render() {
    glClear(GL_COLOR_BUFFER_BIT);

    g_program.set_model_matrix(g_object1_matrix);


    float vertices[] =
    {
        -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f,
        -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f
    };

    glVertexAttribPointer(g_program.get_position_attribute(), 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(g_program.get_position_attribute());

    float texture_coordinates[] = {
        0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f
    };

    glVertexAttribPointer(g_program.get_tex_coordinate_attribute(), 2, GL_FLOAT, false, 0, texture_coordinates);
    glEnableVertexAttribArray(g_program.get_tex_coordinate_attribute());


    draw_object(g_object1_matrix, object_texture_id);
    

    glDisableVertexAttribArray(g_program.get_position_attribute());
    glDisableVertexAttribArray(g_program.get_tex_coordinate_attribute());

    // object 2
    
    g_program.set_model_matrix(g_object2_matrix);

    glVertexAttribPointer(g_program.get_position_attribute(), 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(g_program.get_position_attribute());

    glVertexAttribPointer(g_program.get_tex_coordinate_attribute(), 2, GL_FLOAT, false, 0, texture_coordinates);
    glEnableVertexAttribArray(g_program.get_tex_coordinate_attribute());


    draw_object(g_object2_matrix, object2_texture_id);


    glDisableVertexAttribArray(g_program.get_position_attribute()); 
    glDisableVertexAttribArray(g_program.get_tex_coordinate_attribute());

    SDL_GL_SwapWindow(displayWindow);

}

void shutdown() {
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    initialize();


    while (gameIsRunning) {

        process_input();

        update();

        render();
    }

    shutdown();
    return 0;
}