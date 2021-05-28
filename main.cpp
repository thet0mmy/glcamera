#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "util/shader.cpp"
#include "util/controls.cpp"
#include "util/texture.cpp"
#include <fstream>
#include <iostream>
#include <cstring>

glm::mat4 Model;
glm::mat4 View;
glm::mat4 Projection;
glm::mat4 MVP;

bool controls_verbose = false;
// change above if you want detailed position and angle info

static const GLfloat g_vertex_buffer_data[999] = {
    2.0f, 2.0f, 2.0f,
    2.0f, 2.0f, -2.0f,
    -2.0f, 2.0f, -2.0f,
    -2.0f, 2.0f, 2.0f,
    -2.0f, -2.0f, 2.0f,
    2.0f, -2.0f, 2.0f,
};

static GLfloat g_uv_coords[] = {
    1.0f, 1.0f,
    1.0f, 0.0f,
    0.0f, 0.0f,
    1.0f, 1.0f,
    1.0f, 0.0f,
    0.0f, 0.0f
};

int main(int argc, char *argv[])
{
    glewExperimental = true;
    if (!glfwInit())
    {
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window;
    window = glfwCreateWindow(1024, 768, "OpenGL Camera", NULL, NULL);
    if (window == NULL)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glewExperimental = true;
    if (glewInit() != GLEW_OK)
    {
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    GLuint uvtable;
    glGenBuffers(2, &uvtable);
    glBindBuffer(GL_ARRAY_BUFFER, uvtable);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_coords), g_uv_coords, GL_STATIC_DRAW);

    GLuint programID = LoadShaders("shader/TransformTexVertex.glsl", "shader/TransformTexFragment.glsl");
    GLuint MatrixID = glGetUniformLocation(programID, "MVP");
    GLuint GroundTexture = loadBMP_custom("graphics/rock.bmp");

    glClearColor(0.25f, 0.35f, 0.8f, 0.9f);

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    do
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(programID);    

        computeMatricesFromInputs(window, controls_verbose);
        glm::mat4 Projection = getProjectionMatrix();
        glm::mat4 View = getViewMatrix();
        glm::mat4 Model = glm::mat4(1.0f);
        glm::mat4 MVP = Projection * View * Model;
        
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

        glUniform3f(glGetUniformLocation(programID, "Color"), 0.0, 1.0, 1.0);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, uvtable);
        glVertexAttribPointer(
            1,
            2,
            GL_FLOAT,
            GL_FALSE,
            0,
            (void*)0
        );

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
        0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
        );
        glDrawArrays(GL_TRIANGLES, 0, 4*2);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glfwSwapBuffers(window);
        glfwPollEvents();
    } while (glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
       glfwWindowShouldClose(window) == 0);
    return 0;
}