// STL
#include <iostream>
#include <math.h>
#include <stdlib.h>

// include opengl extension and application library
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

// include Engine
#include <Camera.hpp>
#include <Mesh.hpp>
#include <Material.hpp>
#include <RenderObject.hpp>

#include <DefaultMaterial.hpp>
#include <PickingMaterial.hpp>
#include <LineMaterial.hpp>
#include <Geometry.hpp>

#include <picking.hpp>
#include <PickableObject.hpp>

#define _USE_MATH_DEFINES

GLFWwindow* g_window;
float g_window_width = 1024.f;
float g_window_height = 768.f;
int g_framebuffer_width = 1024;
int g_framebuffer_height = 768;

// TODO: Fill up GLFW mouse button callback function
static void MouseButtonCallback(GLFWwindow* a_window, int a_button, int a_action, int a_mods)
{
    //example code for get x position and y position of mouse click
    if (a_button == GLFW_MOUSE_BUTTON_LEFT && a_action == GLFW_PRESS)
    {
        double xpos, ypos;
        glfwGetCursorPos(a_window, &xpos, &ypos);
        xpos = xpos / ((double)g_window_width) * ((double)g_framebuffer_width);
        ypos = ypos / ((double)g_window_height) * ((double)g_framebuffer_height);
        int target = pick((int)xpos, (int)ypos, g_framebuffer_width, g_framebuffer_height);
        std::cout << "Picked object index: " << target << std::endl;
    }
}

// TODO: Fill up GLFW cursor position callback function
static void CursorPosCallback(GLFWwindow* a_window, double a_xpos, double a_ypos)
{

}

static void KeyboardCallback(GLFWwindow* a_window, int a_key, int a_scancode, int a_action, int a_mods)
{
    if (a_action == GLFW_PRESS)
    {
        switch (a_key)
        {
        case GLFW_KEY_H:
            std::cout << "CS380 Lab 2" << std::endl;
            std::cout << "keymaps:" << std::endl;
            std::cout << "h\t\t Help command" << std::endl;
            break;
        default:
            break;
        }
    }
}


int main(int argc, char** argv)
{
    // Initialize GLFW library
    if (!glfwInit())
    {
        return -1;
    }

    // Create window and OpenGL context
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    /* Create a windowed mode window and its OpenGL context */
    g_window = glfwCreateWindow(g_window_width, g_window_height, "Lab 2 - 3D Rendering and Frames", NULL, NULL);
    if (!g_window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(g_window);

    // Initialize GLEW library
    GLenum glew_error = glewInit();
    if (glew_error != GLEW_OK)
    {
        std::cout << "GLEW Error: " << glewGetErrorString(glew_error) << std::endl;
        exit(1);
    }

    // Enable depth test
    glEnable(GL_DEPTH_TEST);

    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK); // Cull back-facing triangles -> draw only front-facing triangles

    glfwSetInputMode(g_window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetMouseButtonCallback(g_window, MouseButtonCallback);
    glfwSetCursorPosCallback(g_window, CursorPosCallback);
    glfwSetKeyCallback(g_window, KeyboardCallback);

    // Initialize framebuffer object and picking textures
    pickingInitialize(g_framebuffer_width, g_framebuffer_width);

    Engine::Camera* main_camera = new Engine::Camera();
    main_camera->SetPosition(glm::vec3(0.0f, 0.0f, 7.5f));

    // TODO: Create mesh and material (in main.cpp) Slide No. 10 (Define cube mesh, material), No. 18 (Define pickable object)
    
    // TODO: Add parent (cube1) to cube2 (Slide No. 14)

    Engine::Mesh* cube_mesh = new Engine::Mesh();
    Geometry geometry = Geometry();
    geometry.GenerateCube(cube_mesh);

    DefaultMaterial* material = new DefaultMaterial();
    material->CreateMaterial();

//    Engine::RenderObject cube1 = Engine::RenderObject(cube_mesh, material);
//    cube1.SetPosition(glm::vec3(0.0f, 2.0f, 0.0f));
//
//    Engine::RenderObject cube2 = Engine::RenderObject(cube_mesh, material);
//    cube2.SetPosition(glm::vec3(0.0f, -2.0f, 0.0f));

    PickingMaterial* picking_material = new PickingMaterial();
    picking_material->CreateMaterial();

    PickableObject cube1 = PickableObject(cube_mesh, material);
    cube1.SetPickingMat(picking_material);
    cube1.SetPosition(glm::vec3(0.0f, 2.0f, 0.0f));
    cube1.SetIndex(1);

    PickableObject cube2 = PickableObject(cube_mesh,material);
    cube2.SetPickingMat(picking_material);
    cube2.SetPosition(glm::vec3(0.0f, 2.0f, 0.0f));
    cube2.SetIndex(2);
    cube2.AddParent(&cube1);

    float prev_time = 0;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(g_window) && glfwGetKey(g_window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
    {
        float total_time = (float)glfwGetTime();
        float elapsed_time = total_time - prev_time;
        prev_time = total_time;

        // First Pass: Object Selection (Slide No. 20)
        // this is for picking the object using mouse interaction
        // binding framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, picking_fbo);
        // Background: RGB = 0x000000 => objectID: 0
        glClearColor((GLclampf) 0.0f, (GLclampf) 0.0f, (GLclampf) 0.0f, (GLclampf) 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // render your objects that you want to select using mouse interaction here

        // Second Pass: Object Rendering (Slide No. 11)
        // Drawing object again
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor((GLclampf)(128. / 255.), (GLclampf)(200. / 255.), (GLclampf)(255. / 255.), (GLclampf)0.);

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Todo: Render object with main camera in the loop (Slide No. 11)
        cube1.RenderPicking(main_camera);
        cube2.RenderPicking(main_camera);


        /// Picking

        glBindFramebuffer(GL_FRAMEBUFFER, picking_fbo);

        glClearColor((GLclampf)0.0f, (GLclampf)0.0f, (GLclampf) 0.0f, (GLclampf) 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // update color of cube 1
        material->UpdateColor(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
        cube1.Render(main_camera);

        // update color for cube2
        material->UpdateColor(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
        cube2.Render(main_camera);

        glfwSwapBuffers(g_window);

        glfwPollEvents();
        
        /* Swap front and back buffers */
        glfwSwapBuffers(g_window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    // Delete resources
    delete main_camera;
    delete cube_mesh;
    delete material;
    delete picking_material;

    glfwTerminate();
    return 0;
}