#include "SpotLight.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"

Mesh* SpotLight::CreateSpotCone(const std::string& name) {

	std::vector<VertexFormat> vertices;
	std::vector<unsigned int> indices;
	
	vertices.push_back(VertexFormat(glm::vec3(0, 1, 0)));

	for (int i = 0; i < 360; i += 2) {
		vertices.push_back(VertexFormat(glm::vec3(sin(RADIANS(i)), 0, cos(RADIANS(i)))));
	}

	for (int i = 1; i < 180; i++) {
		indices.push_back(0);
		indices.push_back(i);
		indices.push_back(i + 1);
	}

	indices.push_back(0);
	indices.push_back(180);
	indices.push_back(1);

	Mesh* spotCone = new Mesh(name);

	spotCone->InitFromData(vertices, indices);
	return spotCone;
}
