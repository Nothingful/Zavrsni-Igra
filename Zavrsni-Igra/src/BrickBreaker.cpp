#include <gl/glew.h>
#include <GLFW/glfw3.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"

#include "gameObjects/Game.h"
#include "ResourceManager.h"

#include <iostream>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"



/* GLFW function declerations */
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

/* Settings */
const unsigned int SCREEN_WIDTH = 1280;
const unsigned int SCREEN_HEIGHT = 720;

Game BrickBreaker(SCREEN_WIDTH, SCREEN_HEIGHT);

int main(int argc, char* argv[])
{
    /* Initialize the library */
    if (!glfwInit())
    {
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* window;
    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Game", NULL, NULL);
    if (!window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

	/* Set key callback function */
    glfwSetKeyCallback(window, key_callback);
	
    /* Window resize callback */
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSwapInterval(1); // ?

    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }
    std::cout << glGetString(GL_VERSION) << std::endl;

	/* OpenGL configuration */
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    /* Creating ImGui context */
    //IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    //ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui_ImplGlfwGL3_Init(window, false);
    ImGui::StyleColorsDark();

    /* Initialize game */
    BrickBreaker.Init();

    /* deltaTime variables */
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    glm::mat4 projection = glm::ortho(0.0f, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT, 0.0f, -1.0f, 1.0f);

    while (!glfwWindowShouldClose(window))
    {
        /* Calculate delta time */
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Start new ImGui frame
        ImGui_ImplGlfwGL3_NewFrame();
    	
        /* User input */
        BrickBreaker.ProcessInput(deltaTime);

        /* Update game state */
        BrickBreaker.Update(deltaTime);

        /* Render here */
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        BrickBreaker.Render();

        {
            ImGui::Begin("Brick Breaker");
            ImGui::Text("Level: %d \n", BrickBreaker.Level + 1);
            ImGui::Text("Resolution: %d x %d", (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }
    	
        ImGui::Render();
        ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
	
    /* Delete all resources as loaded using the resource manager */
    ResourceManager::Clear();
    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    /* When a user presses the escape key, we set the WindowShouldClose property to true, closing the application */
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) 
    {
	    glfwSetWindowShouldClose(window, true);
	}
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS) BrickBreaker.Keys[key] = true;
        else if (action == GLFW_RELEASE) BrickBreaker.Keys[key] = false;
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}