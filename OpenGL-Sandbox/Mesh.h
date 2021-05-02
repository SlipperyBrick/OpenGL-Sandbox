#pragma once
#include <vector>
#include <string>
#include <GL/glew.h>
#include "Entity.h"
#include "Vertex.hpp"

class Mesh : public Entity
{
public:
	Mesh();
	Mesh(std::string name, std::vector<Vertex> vertices, std::vector<unsigned int> indices);
	~Mesh();

	inline std::string GetName() const { return m_name; };
	
	void Create(GLfloat* vertices, unsigned int *indices, unsigned int vertexCount, unsigned int indicesCount);
	void Create(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
	void Create();

	void Render();
	void Clear();

private:
	std::string m_name;
	std::vector<Vertex> m_vertices;
	std::vector<unsigned int> m_indices;
	GLuint VAO, VBO, IBO;
	GLsizei m_indexCount;

};

