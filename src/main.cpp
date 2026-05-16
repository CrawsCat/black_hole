#include <iostream>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "Uitility.h"

int WIDTH = 1600;
int HEIGHT = 900;

GLfloat vertices[] =
{
	-1.0f, -1.0f , 0.0f, 0.0f, 0.0f,
	-1.0f,  1.0f , 0.0f, 0.0f, 1.0f,
	 1.0f,  1.0f , 0.0f, 1.0f, 1.0f,
	 1.0f, -1.0f , 0.0f, 1.0f, 0.0f,
};

GLuint indices[] =
{
	0, 2, 1,
	0, 3, 2
};

// ========== main ==========

int main() {
    // ========== initialize glfw

    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Black Hole", NULL, NULL);

    if (!window) {
        glfwTerminate();
        std::cout << "Warning: Unable to create window!" << std::endl;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Warning: Failed to initialize OpenGL context!" << std::endl;
    }
    glViewport(0, 0, WIDTH, HEIGHT);

    GLuint vao, vbo, ebo;
    glCreateVertexArrays(1, &vao);
    glCreateBuffers(1, &vbo);
    glCreateBuffers(1, &ebo);

    glNamedBufferData(vbo, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glNamedBufferData(ebo, sizeof(indices), indices, GL_STATIC_DRAW);

    glEnableVertexArrayAttrib(vao, 0);
    glVertexArrayAttribBinding(vao, 0, 0);
    glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, 0);

    glEnableVertexArrayAttrib(vao, 1);
    glVertexArrayAttribBinding(vao, 1, 0);
    glVertexArrayAttribFormat(vao, 1, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat));

    glVertexArrayVertexBuffer(vao, 0, vbo, 0, 5 * sizeof(GLfloat));
    glVertexArrayElementBuffer(vao, ebo);

    GLuint screenTexture;
    glCreateTextures(GL_TEXTURE_2D, 1, &screenTexture);
    glTextureParameteri(screenTexture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTextureParameteri(screenTexture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTextureParameteri(screenTexture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(screenTexture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTextureStorage2D(screenTexture, 1, GL_RGBA32F, WIDTH, HEIGHT);
    glBindImageTexture(0, screenTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

    GLuint screenVertexShader = glCreateShader(GL_VERTEX_SHADER);
    std::string vertexCode = readFile("../../src/shaders/screenShader.vert");
    const char* vertexSource = vertexCode.c_str();
    glShaderSource(screenVertexShader, 1, &vertexSource, NULL);
    glCompileShader(screenVertexShader);
    GLuint screenFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    std::string fragmentCode = readFile("../../src/shaders/screenShader.frag");
    const char* fragmentSource = fragmentCode.c_str();
    glShaderSource(screenFragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(screenFragmentShader);

    GLuint screenShaderProgram = glCreateProgram();
    glAttachShader(screenShaderProgram, screenVertexShader);
    glAttachShader(screenShaderProgram, screenFragmentShader);
    glLinkProgram(screenShaderProgram);

    glDeleteShader(screenVertexShader);
    glDeleteShader(screenFragmentShader);

    GLuint computeShader = glCreateShader(GL_COMPUTE_SHADER);    
    std::string computeCode = readFile("../../src/shaders/rayTracer.glsl");
    const char* computeSource = computeCode.c_str();
    glShaderSource(computeShader, 1, &computeSource, NULL);
    glCompileShader(computeShader);

    GLuint computeProgram = glCreateProgram();
    glAttachShader(computeProgram, computeShader);
    glLinkProgram(computeProgram);

    while (!glfwWindowShouldClose(window)) {
        glUseProgram(computeProgram);
        glDispatchCompute(ceil(WIDTH / 8), ceil(HEIGHT / 4), 1);
        glMemoryBarrier(GL_ALL_BARRIER_BITS);

        glUseProgram(screenShaderProgram);
        glBindTextureUnit(0, screenTexture);
        glUniform1i(glGetUniformLocation(screenShaderProgram, "screen"), 0);
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]), GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;    
}