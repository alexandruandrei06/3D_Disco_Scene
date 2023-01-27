#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"

namespace SpotLight
{
    // Functie pentru crearea mesh-ului pentru conul de lumina
    Mesh* CreateSpotCone(const std::string& name);
}