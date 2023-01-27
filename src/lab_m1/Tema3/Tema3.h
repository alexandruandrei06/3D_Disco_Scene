#pragma once

#include "components/simple_scene.h"
#include "components/transform.h"


namespace m1
{
    class Tema3 : public gfxc::SimpleScene
    {
    public:
        Tema3();
        ~Tema3();

        void Init() override;

    private:
        // Structura folosita pentru retinerea informatiilor despre elementele ringului de dans
        struct FloorCell
        {
            FloorCell() : x(0), y(0) {}
            FloorCell(float x, float y, glm::vec3 color, glm::vec3 lightPosition)
                : x(x), y(y), color(color), lightPosition(lightPosition) {}
            float x;
            float y;
            glm::vec3 color;
            glm::vec3 lightPosition;
            glm::vec3 lightDirection = glm::vec3(0, 1, 0);
        };

        // Structura folosita pentru retinerea informatiilor despre pereti
        struct Wall
        {
            Wall() : x(0), y(0) {}
            Wall(float x, float y)
                : x(x), y(y){}
            float x;
            float y;
            glm::vec3 color = glm::vec3(0, 0, 0);
        };

        // Structura folosita pentru retinerea informatiilor despre dansatori
        struct Dancer
        {
            Dancer() : x(0), z(0) {}
            Dancer(float x, float z, glm::vec3 color, int moveX, int moveZ, float maxDist)
                : x(x), z(z), color(color), moveX(moveX), moveZ(moveZ), maxDist(maxDist){}
            float x;
            float z;
            glm::vec3 color;
            float dist = 0;
            int moveX;
            int moveZ;
            float maxDist;
        };


        // Functie pentru initializarea shader-ului folosit pentru tema
        void InitShader();

        // Functie pentru initializarea ringului de dans
        void InitFloor();

        // Functie pentru initializarea peretilor
        void InitWalls();

        // Functie pentru initializarea spot-urilor
        void InitSpots();
        
        // Functie pentru initializarea dansatorilor
        void InitDancers();

        // Functie pentru initializarea globului disco
        void InitDiscoBall();

        //Desenarea ringului de dans
        void DrawFloor(float deltaTimeSeconds);

        //Desenarea peretilor
        void DrawWalls(float deltaTimeSeconds);

        //Desenarea si miscarea conurilor de lumina
        void DrawSpotsCone(float deltaTimeSeconds);

        //Desenarea si miscarea dansatorilor
        void DrawDancers(float deltaTimeSeconds);

        //Desenarea si rotirea globului disco
        void DrawDiscoBall(float deltaTimeSeconds);

        //Generarea unei taxturi de width x height, avand culori random
        Texture2D* CreateRandomTexture(unsigned int width, unsigned int height);

        void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color, int isWall, Texture2D* texture);

        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;
        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        unsigned int materialShininess;
        float materialKd;
        float materialKs;
        float materialKe;

        //vectori cu elementele despre celulele ringului de dans, a peretilor si a dansatorilor
        std::vector <FloorCell> floorCells;
        std::vector <Wall> walls;
        std::vector <Dancer> dancers;

        //vectori cu pozitiile si culorile luminilor ringului de dans
        glm::vec3 floorLightPos[64];
        glm::vec3 floorLightColor[64];

        //Pozitia globului disco si a luminii din interiorul acesteia
        glm::vec3 discoBallPos;
        glm::vec3 discoBallLight;

        //Pozitiile, directiile si culorile spoturilor de lumina
        glm::vec3 spotLightPos[4];
        glm::vec3 spotLightColor[4] = {
            glm::vec3(1, 0, 0), 
            glm::vec3(0.231, 0.988, 0.172),
            glm::vec3(0.054, 0.486, 0.847),
            glm::vec3(0.709, 0.066, 0.933)
        };
        glm::vec3 spotLightDirection[4];

        float cutoffAngle;
        float angleOX[4] = { 0.1, 0, 0, 0.1 };
        float angleOY[4] = { 0, 0, 0, 0 };
        int spotDir[2] = { 1, -1 };
        
        std::unordered_map<std::string, Texture2D*> mapTextures;

        //switch-ul tipurilor de iluminare
        int typeOfLight = 4;
    };
}   // namespace m1