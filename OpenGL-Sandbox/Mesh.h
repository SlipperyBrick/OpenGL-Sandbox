#pragma once
#include "Entity.h"
#include <GL/glew.h>
#include <vector>
#include "Vertex.hpp"

class Mesh : public Entity
{
public:
	Mesh();
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
	~Mesh();

	//Static mesh
	void Create(GLfloat* vertices, unsigned int *indices, unsigned int vertexCount, unsigned int indicesCount);

	//Non-Static Mesh
	void CreateAnimatedMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);

	void Render();
	void Clear();

private:

	GLuint VAO, VBO, IBO;
	GLsizei indexCount;

};

