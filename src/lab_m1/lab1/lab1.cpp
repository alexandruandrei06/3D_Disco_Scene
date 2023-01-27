#include "lab_m1/lab1/lab1.h"

#include <vector>
#include <math.h>
#include <iostream>



using namespace std;
using namespace m1;

float red, green, blue;
float x, y, k;
float R = 4;
float circle_x, circle_y, angle;
Mesh* sphereMesh;
Mesh* teapotMesh;
Mesh* oildrumMesh;
Mesh* currentMesh;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Lab1::Lab1()
{
    // TODO(student): Never forget to initialize class variables!

}


Lab1::~Lab1()
{
}


void Lab1::Init()
{
    //initializare culori
    red = 0;
    blue = 0;
    green = 0;

    //initializare coordonate
    x = 0;
    y = 0;
    k = 0;

    //initializare coordonate pentru obiectul care se roteste
    circle_x = 0;
    circle_y = 0;
    angle = 0;

    // Load a mesh from file into GPU memory. We only need to do it once,
    // no matter how many times we want to draw this mesh.
    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // TODO(student): Load some more meshes. The value of RESOURCE_PATH::MODELS
    // is actually a path on disk, go there and you will find more meshes.

    {
        sphereMesh = new Mesh("sphere");
        sphereMesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[sphereMesh->GetMeshID()] = sphereMesh;
    }

    {
        teapotMesh = new Mesh("teapot");
        teapotMesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "teapot.obj");
        meshes[teapotMesh->GetMeshID()] = teapotMesh;
    }

    {
        oildrumMesh = new Mesh("oildrum");
        oildrumMesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "props"), "oildrum.obj");
        meshes[oildrumMesh->GetMeshID()] = oildrumMesh;
    }

    currentMesh = sphereMesh;
}


void Lab1::FrameStart()
{
}


void Lab1::Update(float deltaTimeSeconds)
{
    glm::ivec2 resolution = window->props.resolution;

    // Sets the clear color for the color buffer

    // TODO(student): Generalize the arguments of `glClearColor`.
    // You can, for example, declare three variables in the class header,
    // that will store the color components (red, green, blue).
    glClearColor(red, green, blue, 1);

    // Clears the color buffer (using the previously set color) and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);

    // Render the object
    RenderMesh(meshes["box"], glm::vec3(1, 0.5f, 0), glm::vec3(0.5f));

    // Render the object again but with different properties

    RenderMesh(meshes["box"], glm::vec3(circle_x, circle_y, R));

    // TODO(student): We need to render (a.k.a. draw) the mesh that
    // was previously loaded. We do this using `RenderMesh`. Check the
    // signature of this function to see the meaning of its parameters.
    // You can draw the same mesh any number of times.

    RenderMesh(currentMesh, glm::vec3(x, y, k), glm::vec3(0.5f));
}


void Lab1::FrameEnd()
{
    DrawCoordinateSystem();
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Lab1::OnInputUpdate(float deltaTime, int mods)
{
    // Treat continuous update based on input

    // TODO(student): Add some key hold events that will let you move
    // a mesh instance on all three axes. You will also need to
    // generalize the position used by `RenderMesh`.

    if (window->KeyHold(GLFW_KEY_A)) {
        x += deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_D)) {
        x -= deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_W)) {
        y += deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_S)) {
        y -= deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_E)) {
        k += deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_Q)) {
        k -= deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_R)) {
        circle_x = R * cos(angle + deltaTime);
        circle_y = R * sin(angle + deltaTime);
        angle += deltaTime;
    }
}


void Lab1::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_F) {
        if (red == 1) {
            blue = 1;
            green = 0;
            red = 0;
        }
        else if (blue == 1) {
            blue = 0;
            green = 1;
            red = 0;
        }
        else {
            red = 1;
            blue = 0;
            green = 0;
        }

    }

    // TODO(student): Add a key press event that will let you cycle
    // through at least two meshes, rendered at the same position.
    // You will also need to generalize the mesh name used by `RenderMesh`.

    if (key == GLFW_KEY_K) {
        if (currentMesh == sphereMesh) {
            currentMesh = teapotMesh;
        }
        else if (currentMesh == teapotMesh) {
            currentMesh = oildrumMesh;
        }
        else {
            currentMesh = sphereMesh;
        }

    }
}


void Lab1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Lab1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Lab1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Lab1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Lab1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
    // Treat mouse scroll event
}


void Lab1::OnWindowResize(int width, int height)
{
    // Treat window resize event
}
