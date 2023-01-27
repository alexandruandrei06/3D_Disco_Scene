#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"

namespace Floor
{   
    //Functie pentru crearea mesh-ului petru podea
    Mesh* CreateFloorCell(const std::string& name);
}