#pragma once

#include <vector>
#include <string>

#include <gl/glew.h>
#include <SOIL2/SOIL2.h>
#include <glm/glm.hpp>

#include "Mesh.h"
#include "Shader.h"

class Skybox
{
public:

	Skybox(std::vector<std::string> facesPaths); 
   ~Skybox();

   void Render(glm::mat4 viewMatrix, glm::mat4 projectionMatrix);

private:

	void CreateCubeMap(std::string imagePath[6]);
	void CreateMesh();

	Mesh* m_mesh;
	Shader* m_shader;
	GLuint m_cubemapID;

};

