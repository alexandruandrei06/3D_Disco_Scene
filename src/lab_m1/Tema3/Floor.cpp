#include "Floor.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"

Mesh* Floor::CreateFloorCell(const std::string& name)
{
    std::vector<VertexFormat> vertices =
    {
        //Un patrat pentru podea
        VertexFormat(glm::vec3(0, 0, 0)),    //0
        VertexFormat(glm::vec3(0, 0, 1)),    //1
        VertexFormat(glm::vec3(1, 0, 1)),    //2
        VertexFormat(glm::vec3(1, 0, 0)),    //3
    };

    std::vector<unsigned int> indices =
    {
        0, 1, 2,
        0, 2, 3
    };

    Mesh* map_grid = new Mesh(name);

    map_grid->InitFromData(vertices, indices);
    return map_grid;
}
