#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Core/Timer.hpp"

enum TextureUnits {

	TU_NULL,
	TU_IRRADIANCE, // Skybox 
	TU_PREFILTER,
	TU_BRDF,

	TU_ALBEDO, // Material
	TU_NORMAL,
	TU_ROUGHNESS,
	TU_AO,
	TU_METALLIC,
	TU_DISPLACEMENT,

	TU_DIRECTIONAL_SHADOW_MAP, // Shadows

	TU_POINT_SHADOW_MAP0,
	TU_POINT_SHADOW_MAP1,
	TU_POINT_SHADOW_MAP2,
	TU_POINT_SHADOW_MAP3,

	TU_SPOT_SHADOW_MAP0,
	TU_SPOT_SHADOW_MAP1,
	TU_SPOT_SHADOW_MAP2,
	TU_SPOT_SHADOW_MAP3,

};

class Shader
{
public:

	Shader();
	~Shader();

	void CreateFromString(const char* vertexShader, const char* fragmentShader);
	void CreateFromFile(const char* vertexPath, const char* geometryShader, const char* fragmentPath);
	void CreateFromFile(const char* vertexPath, const char* fragmentPath);
	void CreateFromFile(const char* computePath);

	std::string ReadFile(const char* path);

	void Bind();
	void Unbind();

	GLuint GetShaderID() const { return m_id; };

	void ClearShader();
	
	void Set1i   (const std::string& name, const GLint value);
	void Set1f   (const std::string& name, const GLfloat value);
	void SetVec2f(const std::string& name, const glm::vec2& value);
	void SetVec3f(const std::string& name, const glm::vec3& value);
	void SetVec4f(const std::string& name, const glm::vec4& value);
	void SetMat3f(const std::string& name, const glm::mat3& value, const bool transpose);
	void SetMat4f(const std::string& name, const glm::mat4& value, const bool transpose);

	void Validate();
	void QueryWorkgroups();

private:

	void CompileShader(const char* vertexShader, const char* fragmentShader);
	void CompileShader(const char* vertexShader, const char* geometryShader, const char* fragmentShader);
	void CompileShader(const char* computeShader);
	void AddShader(GLuint program, const char* shaderCode, GLenum type);

	GLint GetUniformLocation(const std::string& name) const;

	unsigned int m_id;
	mutable std::unordered_map<std::string, GLint> m_uniformLocationMap;
};

