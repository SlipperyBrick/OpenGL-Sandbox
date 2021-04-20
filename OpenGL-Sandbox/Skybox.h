#pragma once

#include <vector>
#include <string>

#include <gl/glew.h>
#include <SOIL2/SOIL2.h>
#include <glm/glm.hpp>

#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"


class Skybox
{
public:

	Skybox();
	Skybox(Texture* cubemap);

   ~Skybox();

   void Render(Camera* camera);
   void SetCubemap(Texture* cubemap);

private:

	void CreateMesh();

	Mesh* m_mesh;
	Shader* m_shader;
	Texture* m_cubemap;
	GLuint m_cubemapID;
	unsigned int envCubemap;
};

