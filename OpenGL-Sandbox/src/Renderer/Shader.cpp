#include "Shader.h"

Shader::Shader()
{
	m_id = 0;
}

Shader::~Shader()
{
	this->ClearShader();
}

void Shader::CreateFromString(const char* vertexShader, const char* fragmentShader)
{
	CompileShader(vertexShader, fragmentShader);
}

void Shader::CreateFromFile(const char* vertexPath, const char* geometryPath, const char* fragmentPath)
{
	std::string vertexStr = ReadFile(vertexPath);
	std::string geometryStr = ReadFile(geometryPath);
	std::string fragmentStr = ReadFile(fragmentPath);

	CompileShader(vertexStr.c_str(), geometryStr.c_str(), fragmentStr.c_str());
}

void Shader::CreateFromFile(const char* vertexPath, const char* fragmentPath)
{
	std::string vertexStr = ReadFile(vertexPath);
	std::string fragmentStr = ReadFile(fragmentPath);

	CompileShader(vertexStr.c_str(), fragmentStr.c_str());

}

void Shader::CreateFromFile(const char* computePath)
{
	std::string computeStr = ReadFile(computePath);
	CompileShader(computeStr.c_str());
}

std::string Shader::ReadFile(const char* path)
{
	std::string content;
	std::ifstream fileStream(path, std::ios::in);

	if (!fileStream.is_open()) {

		printf("[ERROR READING FILE]: Failed to open file {%s}\n", path);
		return "";
	}

	std::string line = "";

	while (!fileStream.eof())
	{
		std::getline(fileStream, line);
		content.append(line + "\n");
	}

	fileStream.close();
	return content;

}

void Shader::Bind()
{
	if (m_id != 0)
		glUseProgram(m_id);
	else
		std::cout << "[SHADER ERROR]: Shader did not bind..\n";
}

void Shader::Unbind()
{
	glUseProgram(0);
}

void Shader::ClearShader()
{
	if (m_id != 0) {
		glDeleteProgram(m_id);
		m_id = 0;
	}

}

void Shader::Set1f(const std::string& name, const GLfloat value)
{
	glUniform1f(GetUniformLocation(name), value);
}

void Shader::Set1i(const std::string& name, const GLint value)
{
	glUniform1i(GetUniformLocation(name), value);
}

void Shader::SetVec2f(const std::string& name, const glm::vec2& value)
{
	glUniform2f(GetUniformLocation(name), value.x, value.y);
}

void Shader::SetVec3f(const std::string& name, const glm::vec3& value)
{
	glUniform3f(GetUniformLocation(name), value.x, value.y, value.z);
}

void Shader::SetVec4f(const std::string& name, const glm::vec4& value) {
	glUniform4f(GetUniformLocation(name), value.x, value.y, value.z, value.w);
}

void Shader::SetMat3f(const std::string& name, const glm::mat3& value, const bool transpose)
{
	glUniformMatrix3fv(GetUniformLocation(name), 1, transpose, glm::value_ptr(value));
}

void Shader::SetMat4f(const std::string& name, const glm::mat4& value, const bool transpose)
{
	glUniformMatrix4fv(GetUniformLocation(name), 1, transpose, glm::value_ptr(value));
}

void Shader::Validate()
{
	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glValidateProgram(m_id);
	glGetProgramiv(m_id, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(m_id, sizeof(eLog), NULL, eLog);
		printf("Error validating program: '%s'\n", eLog);
		return;
	}
}

void Shader::QueryWorkgroups() {

	int work_grp_size[3], work_grp_inv;
	// maximum global work group (total work in a dispatch)
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &work_grp_size[0]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &work_grp_size[1]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &work_grp_size[2]);
	//printf("max global (total) work group size x:%i y:%i z:%i\n", work_grp_size[0], work_grp_size[1], work_grp_size[2]);
	// maximum local work group (one shader's slice)
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &work_grp_size[0]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &work_grp_size[1]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &work_grp_size[2]);
	//printf("max local (in one shader) work group sizes x:%i y:%i z:%i\n", work_grp_size[0], work_grp_size[1], work_grp_size[2]);
	// maximum compute shader invocations (x * y * z)
	glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &work_grp_inv);
	//printf("max computer shader invocations %i\n", work_grp_inv);
}

void Shader::CompileShader(const char* vertexShader, const char* fragmentShader)
{
	m_id = glCreateProgram();

	AddShader(m_id, vertexShader, GL_VERTEX_SHADER);
	AddShader(m_id, fragmentShader, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar errorLog[1024] = { 0 };

	glLinkProgram(m_id);
	glGetProgramiv(m_id, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(m_id, sizeof(errorLog), NULL, errorLog);
		printf("[ERROR]: Linking program failed: '%s'\n", errorLog);
		return;
	}

	glValidateProgram(m_id);
	glGetProgramiv(m_id, GL_VALIDATE_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(m_id, sizeof(errorLog), NULL, errorLog);
		printf("[ERROR]: Validating program failed: '%s'\n", errorLog);
		return;
	}

	if (!m_id) {
		printf("[ERROR]: shader wasn't complied..\n");
		return;
	}
}

void Shader::CompileShader(const char* vertexShader, const char* geometryShader, const char* fragmentShader)
{
	m_id = glCreateProgram();

	AddShader(m_id, vertexShader, GL_VERTEX_SHADER);
	AddShader(m_id, geometryShader, GL_GEOMETRY_SHADER);
	AddShader(m_id, fragmentShader, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar errorLog[1024] = { 0 };

	glLinkProgram(m_id);
	glGetProgramiv(m_id, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(m_id, sizeof(errorLog), NULL, errorLog);
		printf("[ERROR]: Linking program failed: '%s'\n", errorLog);
		return;
	}

	glValidateProgram(m_id);
	glGetProgramiv(m_id, GL_VALIDATE_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(m_id, sizeof(errorLog), NULL, errorLog);
		printf("[ERROR]: Validating program failed: '%s'\n", errorLog);
		return;
	}

	if (!m_id) {
		printf("[ERROR]: shader wasn't complied..\n");
		return;
	}

}

void Shader::CompileShader(const char* computeShader)
{
	
	m_id = glCreateProgram();

	AddShader(m_id, computeShader, GL_COMPUTE_SHADER);

	GLint result = 0;
	GLchar errorLog[1024] = { 0 };

	glLinkProgram(m_id);
	glGetProgramiv(m_id, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(m_id, sizeof(errorLog), NULL, errorLog);
		printf("[ERROR]: Linking program failed: '%s'\n", errorLog);
		return;
	}

	glValidateProgram(m_id);
	glGetProgramiv(m_id, GL_VALIDATE_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(m_id, sizeof(errorLog), NULL, errorLog);
		printf("[ERROR]: Validating program failed: '%s'\n", errorLog);
		return;
	}

	if (!m_id) {
		printf("[ERROR]: shader wasn't complied..\n");
		return;
	}

}

void Shader::AddShader(GLuint program, const char* shaderCode, GLenum type)
{
	//TODO rewirte func
	GLuint shader = glCreateShader(type);
	const char* programCode[1]; //???
	programCode[0] = shaderCode;

	GLint codeLength[1]; //???
	codeLength[0] = strlen(shaderCode);

	glShaderSource(shader, 1, (const GLchar* const*)programCode, codeLength);
	glCompileShader(shader);

	GLint result = 0;
	GLchar errorLog[1024] = { 0 };
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(shader, sizeof(errorLog), NULL, errorLog);
		printf("[ERROR]: Compiliing %d shader: '%s'\n", type, errorLog);
		return;
	}
	glAttachShader(program, shader);
}

GLint Shader::GetUniformLocation(const std::string& name) const 
{
	if (m_uniformLocationMap.find(name) != m_uniformLocationMap.end())
		return m_uniformLocationMap[name];

	GLint location = glGetUniformLocation(m_id, name.c_str());
	m_uniformLocationMap[name] = location;
	return location;
}
