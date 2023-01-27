#include "lab_m1/Tema3/Tema3.h"
#include "lab_m1/Tema3/Floor.h"
#include "lab_m1/Tema3/SpotLight.h"

#include <vector>
#include <Core/Engine.h>
#include <string>
#include <iostream>
#include<time.h>

using namespace std;
using namespace m1;

Tema3::Tema3()
{
}

Tema3::~Tema3()
{
}

void Tema3::Init()
{
    InitShader();
    InitFloor();
    InitWalls();
    InitSpots();
    InitDancers();
    InitDiscoBall();

    {
        Mesh* box = new Mesh("box");
        box->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[box->GetMeshID()] = box;
    }

    {
        Mesh* cone = SpotLight::CreateSpotCone("spotCone");
        AddMeshToList(cone);
    }

    {
        mapTextures["random"] = CreateRandomTexture(16, 16);
    }

    // material properties
    {
        materialShininess = 40;
        materialKe = 0.9;
        materialKd = 1.1;
        materialKs = 0.2;
    }

    // spotlight properties
    {
        cutoffAngle = 20.f;
    }
}

void Tema3::InitShader() {

    Shader* shader = new Shader("Tema3Shader");
    shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
    shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
    shader->CreateAndLink();
    shaders[shader->GetName()] = shader;
}

void Tema3::InitFloor() {
    //Initializarea ringului de dans, fiecare celula avand o culoare aleasa random

    Mesh* floorCell = Floor::CreateFloorCell("floor_cell");
    AddMeshToList(floorCell);
    srand(time(0));

    int k = 0;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            float color1 = rand() % 256;
            float color2 = rand() % 256;
            float color3 = rand() % 256;

            floorCells.push_back(FloorCell(i, j, glm::vec3(color1 / 255, color2 / 255, color3 / 255), glm::vec3(i + 0.5, 0, j + 0.5)));
            floorLightPos[k] = glm::vec3(i + 0.5, 0, j + 0.5);
            floorLightColor[k] = glm::vec3(color1 / 255, color2 / 255, color3 / 255);
            k++;
        }
    }
}

void Tema3::InitWalls() {

    for (int i = 0; i < 8; i++) {
        walls.push_back(Wall(i + 0.5, -1 + 0.5));
        walls.push_back(Wall(i + 0.5, 8 + 0.5));
        walls.push_back(Wall(8 + 0.5, i + 0.5));
    }

    walls.push_back(Wall(8 + 0.5, -1 + 0.5));
    walls.push_back(Wall(8 + 0.5, 8 + 0.5));
}

void Tema3::InitSpots() {
    int k = 0;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            spotLightPos[k] = glm::vec3(2 + 4 * i, 4, 2 + 4 * j);
            spotLightDirection[k] = glm::vec3(0, -1, 0);
            k++;
        }
    }

}

void Tema3::InitDancers() {
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            if (i == 0) {
                dancers.push_back(Dancer(1 + i * 6, 1 + j * 6, glm::vec3(0, 0, 0), (i + j) % 2, (j + 1) % 2, 6));
                dancers.push_back(Dancer(2.5 + i * 3, 2.5 + j * 3, glm::vec3(0, 0, 0), (i + j) % 2, (j + 1) % 2, 3));
            }else {
                dancers.push_back(Dancer(1 + i * 6, 1 + j * 6, glm::vec3(0, 0, 0), -(i + j) % 2, -j, 6));
                dancers.push_back(Dancer(2.5 + i * 3, 2.5 + j * 3, glm::vec3(0, 0, 0), -(i + j) % 2, -j, 3));
            }
        }
    }
}

void Tema3::InitDiscoBall() {
    Mesh* sphere = new Mesh("sphere");
    sphere->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
    meshes[sphere->GetMeshID()] = sphere;

    discoBallPos = glm::vec3(4, 4, 4);
    discoBallLight = glm::vec3(4, 4, 4);
}

void Tema3::DrawDancers(float deltaTimeSeconds) {
    //Dansatorii consituie 2 grupuri a cate 4 dansatori care se misca pe un patrat, avand latura de lungime diferita

    glm::mat4 modelMatrix;
    for (int i = 0; i < dancers.size(); i++) {
        dancers[i].x += dancers[i].moveX * deltaTimeSeconds * 2;
        dancers[i].z += dancers[i].moveZ * deltaTimeSeconds * 2;
        dancers[i].dist += deltaTimeSeconds * 2;
        if (dancers[i].dist > dancers[i].maxDist) {
            dancers[i].dist = 0;
            if (dancers[i].moveX == 0 && dancers[i].moveZ == 1) {
                dancers[i].moveX = 1;
                dancers[i].moveZ = 0;
            }else if (dancers[i].moveX == 1 && dancers[i].moveZ == 0) {
                dancers[i].moveX = 0;
                dancers[i].moveZ = -1;
            }else if (dancers[i].moveX == 0 && dancers[i].moveZ == -1) {
                dancers[i].moveX = -1;
                dancers[i].moveZ = 0;
            }else if (dancers[i].moveX == -1 && dancers[i].moveZ == 0) {
                dancers[i].moveX = 0;
                dancers[i].moveZ = 1;
            }
           
        }

        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(dancers[i].x, 0.5, dancers[i].z));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.65, 1, 0.65));
        RenderSimpleMesh(meshes["box"], shaders["Tema3Shader"], modelMatrix, dancers[i].color, 1, mapTextures["random"]);
    }

}

void Tema3::DrawSpotsCone(float deltaTimeSeconds) {
    //Desenarea si miscarea conurilor de lumina de la spoturi

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glDepthMask(GL_FALSE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glm::mat4 turn;
    for (int i = 0; i < 4; i++) {
        // Rotire spoturi lumini
        if (angleOX[i] != 0) {
            angleOY[i] += deltaTimeSeconds * 2;
        }
        else {
            spotLightPos[i].x -= spotDir[i % 2] * deltaTimeSeconds * 1.5;
            spotLightPos[i].z += spotDir[i % 2] * deltaTimeSeconds * 1.5;
            if (spotLightPos[i].x < 2 && spotLightPos[i].z > 6) {
                spotDir[i % 2] *= -1;
            }else if (spotLightPos[i].z < 2 && spotLightPos[i].x > 6) {
                spotDir[i % 2] *= -1;
            }
        }

        turn = glm::mat4(1);
        turn = glm::rotate(turn, angleOY[i], glm::vec3(0, 1, 0)); //rotire pe OY
        turn = glm::rotate(turn, angleOX[i], glm::vec3(1, 0, 0)); //rotire pe OX
        spotLightDirection[i] = glm::vec3(0, -1, 0);
        spotLightDirection[i] = glm::vec3(turn * glm::vec4(spotLightDirection[i], 0));

        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(spotLightPos[i].x, 5.5, spotLightPos[i].z));
        modelMatrix = glm::rotate(modelMatrix, angleOY[i], glm::vec3(0, 1, 0));
        modelMatrix = glm::rotate(modelMatrix, angleOX[i], glm::vec3(1, 0, 0));
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -5.5 - 0.3, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(tan(RADIANS((cutoffAngle - 4.3))), 1, tan(RADIANS((cutoffAngle - 4.3)))) * glm::vec3(5));
        RenderSimpleMesh(meshes["spotCone"], shaders["Tema3Shader"], modelMatrix, spotLightColor[i], 2, mapTextures["random"]);
    }

    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
    glDisable(GL_CULL_FACE);
}

void Tema3::DrawWalls(float deltaTimeSeconds) {
    for (int i = 0; i < walls.size(); i++) {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(walls[i].x, 4.75 / 2, walls[i].y));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(1, 4.75, 1));
        RenderSimpleMesh(meshes["box"], shaders["Tema3Shader"], modelMatrix, glm::vec3(0, 0, 0), 1, mapTextures["random"]);
    }

    for (int i = 0; i < floorCells.size(); i++) {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(floorCells[i].x + 0.5, 5.25, floorCells[i].y + 0.5));
        RenderSimpleMesh(meshes["box"], shaders["Tema3Shader"], modelMatrix, glm::vec3(0, 0, 0), 0, mapTextures["random"]);
    }
}

void Tema3::DrawFloor(float deltaTimeSeconds) {
    for (int i = 0; i < floorCells.size(); i++) {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(floorCells[i].x, 0, floorCells[i].y));
        RenderSimpleMesh(meshes["floor_cell"], shaders["Tema3Shader"], modelMatrix, floorCells[i].color, 0, mapTextures["random"]);

        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(floorCells[i].x + 0.5, -0.501, floorCells[i].y + 0.5));
        RenderSimpleMesh(meshes["box"], shaders["Tema3Shader"], modelMatrix, glm::vec3(0, 0, 0), 0, mapTextures["random"]);
    }

}

void Tema3::DrawDiscoBall(float deltaTimeSeconds) {
    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, discoBallPos);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(1.5, 1.5, 1.5));
    RenderSimpleMesh(meshes["sphere"], shaders["Tema3Shader"], modelMatrix, glm::vec3(0, 0, 0), 3, mapTextures["random"]);
}

Texture2D* Tema3::CreateRandomTexture(unsigned int width, unsigned int height)
{
    GLuint textureID = 0;
    unsigned int channels = 3;
    unsigned int size = width * height * channels;
    unsigned char* data = new unsigned char[size];

    // Generate random texture data
    for (size_t i = 0; i < size; ++i) {
        data[i] = rand() % (UINT8_MAX + 1);
    }

    // Generate and bind the new texture ID
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    if (GLEW_EXT_texture_filter_anisotropic) {
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4);
    }
    // Set the texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    CheckOpenGLError();

    // Use glTexImage2D to set the texture data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    // Generate texture mip-maps
    glGenerateMipmap(GL_TEXTURE_2D);
    CheckOpenGLError();

    // Save the texture into a wrapper Texture2D class for using easier later during rendering phase
    Texture2D* texture = new Texture2D();
    texture->Init(textureID, width, height, channels);

    SAFE_FREE_ARRAY(data);
    return texture;
}

void Tema3::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

void Tema3::Update(float deltaTimeSeconds)
{
    DrawFloor(deltaTimeSeconds);
    DrawWalls(deltaTimeSeconds);
    DrawDancers(deltaTimeSeconds);
    DrawDiscoBall(deltaTimeSeconds);
    DrawSpotsCone(deltaTimeSeconds);
}

void Tema3::FrameEnd()
{
    
}

void Tema3::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color, int object, Texture2D* texture)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // Set shader uniforms for light properties
    GLuint floorLight_Pos = glGetUniformLocation(shader->program, "floorLight_Pos");
    glUniform3fv(floorLight_Pos, 64, glm::value_ptr(floorLightPos[0]));

    GLuint floorLight_Color = glGetUniformLocation(shader->program, "floorLight_Color");
    glUniform3fv(floorLight_Color, 64, glm::value_ptr(floorLightColor[0]));

    GLuint spotLight_Pos = glGetUniformLocation(shader->program, "spotLight_Pos");
    glUniform3fv(spotLight_Pos, 4, glm::value_ptr(spotLightPos[0]));

    GLuint spotLight_Color = glGetUniformLocation(shader->program, "spotLight_Color");
    glUniform3fv(spotLight_Color, 4, glm::value_ptr(spotLightColor[0]));

    GLuint spotLight_Direction = glGetUniformLocation(shader->program, "spotLight_Direction");
    glUniform3fv(spotLight_Direction, 4, glm::value_ptr(spotLightDirection[0]));

    GLint cut_off_angle = glGetUniformLocation(shader->program, "cut_off_angle");
    glUniform1f(cut_off_angle, cutoffAngle);

    //Time 
    GLint timeT = glGetUniformLocation(shader->program, "timeT");
    glUniform1f(timeT, (GLfloat)Engine::GetElapsedTime());

    //Type of light
    GLint type = glGetUniformLocation(shader->program, "type_of_light");
    glUniform1i(type, typeOfLight);

    //Disco ball position and disco ball light position
    int discoBall_position = glGetUniformLocation(shader->program, "discoBall_position");
    glUniform3f(discoBall_position, discoBallPos.x, discoBallPos.y, discoBallPos.z);

    int discoBallLigth_position = glGetUniformLocation(shader->program, "discoBallLigth_position");
    glUniform3f(discoBallLigth_position, discoBallLight.x, discoBallLight.y, discoBallLight.z);

    // Set eye position (camera position) uniform
    glm::vec3 eyePosition = GetSceneCamera()->m_transform->GetWorldPosition();
    int eye_position = glGetUniformLocation(shader->program, "eye_position");
    glUniform3f(eye_position, eyePosition.x, eyePosition.y, eyePosition.z);

    // Set material property uniforms (shininess, kd, ks, object color) 
    int material_shininess = glGetUniformLocation(shader->program, "material_shininess");
    glUniform1i(material_shininess, materialShininess);

    int material_kd = glGetUniformLocation(shader->program, "material_kd");
    glUniform1f(material_kd, materialKd);

    int material_ks = glGetUniformLocation(shader->program, "material_ks");
    glUniform1f(material_ks, materialKs);

    int material_ke = glGetUniformLocation(shader->program, "material_ke");
    glUniform1f(material_ke, materialKe);

    int object_color = glGetUniformLocation(shader->program, "object_color");
    glUniform3f(object_color, color.r, color.g, color.b);


    // Set texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());
    glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);

    // Set object_type
    GLint object_type = glGetUniformLocation(shader->program, "object_type");
    glUniform1i(object_type, object);

    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}

void Tema3::OnInputUpdate(float deltaTime, int mods)
{
   
}

void Tema3::OnKeyPress(int key, int mods)
{
    // Switch intre tipurile de iliminare la apasarea tastei space
    if (key == GLFW_KEY_SPACE)
    {
        typeOfLight = (typeOfLight + 1) % 5;
    }
}

void Tema3::OnKeyRelease(int key, int mods)
{
    // Add key release event
}

void Tema3::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}

void Tema3::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}

void Tema3::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}

void Tema3::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema3::OnWindowResize(int width, int height)
{
}