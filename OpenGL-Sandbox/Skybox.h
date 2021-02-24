#pragma once

#include <vector>
#include <string>

#include <gl/glew.h>
#include <SOIL2/SOIL2.h>
#include <glm/glm.hpp>

#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"

class Skybox
{
public:

	Skybox();
	Skybox(Texture* cubemap);
	Skybox(std::vector<std::string> facesPaths); 
   ~Skybox();

   void Render(glm::mat4 viewMatrix, glm::mat4 projectionMatrix);
   void SetCubemap(Texture* cubemap);

private:

	void CreateCubemap(std::string imagePath[6]);
	void CreateMesh();

	Mesh* m_mesh;
	Shader* m_shader;
	Texture* m_cubemap;
	GLuint m_cubemapID;
	unsigned int envCubemap;
};

