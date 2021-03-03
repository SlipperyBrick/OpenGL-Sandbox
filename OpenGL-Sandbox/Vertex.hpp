#pragma once

#define MAX_BONE_INFLUENCE 4
#define MAX_BONE_WEIGHTS 4;

struct Vertex
{
	glm::vec3 m_position;
	glm::vec2 m_texcoords;
	glm::vec3 m_normal;
	int m_boneIDs[MAX_BONE_INFLUENCE];
	float m_weights[MAX_BONE_INFLUENCE];
};