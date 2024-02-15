#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

int main(void){
    
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "OpenGL Tutorial", NULL, NULL);
    if (!window){
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK) {
        std::cout << "ERROR!" << std::endl;
    }
    
    std::cout << glGetString(GL_VERSION) << std::endl;
    // start scope 
        // removes the error when closing the program and the index buffer tries to clear the stack allocated objects
        // after glfwTerminate()
    { 
        float positions[] = {
            -0.5f, -0.5f,
             0.5f, -0.5f,
             0.5f,  0.5f,
             -0.5f, 0.5f,
        };

        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };
        
        // vertexArray
        VertexArray va;

        // buffer
        VertexBuffer vb(positions, 4 * 2 * sizeof(float));  // already bound

        // vertexBufferLayout
        VertexBufferLayout layout;
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

        // indices
        IndexBuffer ib(indices, 6);
   
        // shaders
        Shader shader("res/shaders/Shader.shader");
        shader.Bind();
        // change color using global var u_Color

        shader.SetUniform4f("u_Color", 0.1f, 0.3f, 1.0f, 1.0f);

        // clear gl states
        va.Unbind();
        vb.Unbind();
        ib.Unbind();
        shader.Unbind();

        // color variables
        float r = 0.0f;
        float increment = 0.05f;
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window)){
            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);

            shader.Bind();
            shader.SetUniform4f("u_Color", r, 0.3f, 1.0f, 1.0f);

            va.Bind();
            ib.Bind();

            GLCALL( glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr) );

            // color
            if (r > 1.0f) {
                increment = -0.05f;
            }else if (r < 0.0f){
                increment = 0.05f;
            }

            r += increment;

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }

    } // end scope
    glfwTerminate();

    return 0;
}