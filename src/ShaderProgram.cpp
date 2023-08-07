#include "ShaderProgram.h"

ShaderProgram::ShaderProgram()
{
	m_programid = glCreateProgram();
}
ShaderProgram::~ShaderProgram()
{

}

void ShaderProgram::addShader(const std::string& name, const std::string& filePath, const GLenum& type)
{
    std::string source{parse(filePath)};
    const char* char_source{source.c_str()};
    GLuint id{glCreateShader(type)};
    glShaderSource(id, 1, &char_source, NULL);
    glCompileShader(id);
    checkCompileError(name, id);
    ShaderPtr shaderPtr{new Shader(name, std::move(source), id, type)};
	m_shaders.push_back(shaderPtr);
}

void ShaderProgram::combineShadersIntoPair(const std::string& vtx_name, const std::string& frag_name)
{
	auto it_vtx{findShader(vtx_name)};
	auto it_frag(findShader(frag_name));
	if (it_vtx == m_shaders.end() || it_frag == m_shaders.end())
		return;
	m_shaderPairs.push_back(std::pair<ShaderPtr, ShaderPtr>(*it_vtx, *it_frag));
}

void ShaderProgram::setCurrentShaderPair(const std::string& vtx_name, const std::string& frag_name)
{
	auto it{std::find_if(m_shaderPairs.begin(), m_shaderPairs.end(),
	[=](std::pair<ShaderPtr, ShaderPtr>& pair) -> bool
	{
		return pair.first->m_name == vtx_name && pair.second->m_name == frag_name;
	}
	)};

	if (it != m_shaderPairs.end())
		m_currentShaderPair = *it;
	else
		std::cout << "Shader pair <" << vtx_name << ", " << frag_name << "> could not be found.\n";
}

void ShaderProgram::linkAndUse()
{
	glAttachShader(m_programid, m_currentShaderPair.first->m_shaderid);
	glAttachShader(m_programid, m_currentShaderPair.second->m_shaderid);
	glLinkProgram(m_programid);
}

void ShaderProgram::updateUniform1i(const std::string& uniformName, const GLint& uniformData) const
{
    GLint uLocation{ glGetUniformLocation(m_programid, uniformName.data()) };
    glUniform1i(uLocation, uniformData);
}

void ShaderProgram::updateUniform1f(const std::string& uniformName, const GLfloat& uniformData) const
{
    GLint uLocation{ glGetUniformLocation(m_programid, uniformName.data()) };
    glUniform1f(uLocation, uniformData);
}

void ShaderProgram::updateUniform1fv(const std::string& uniformName, const std::vector<GLfloat>& uniformData) const
{
    GLint uLocation{ glGetUniformLocation(m_programid, uniformName.data()) };
    glUniform1fv(uLocation, uniformData.size(), uniformData.data());
}

void ShaderProgram::updateUniform2f(const std::string& uniformName, const glm::vec2& uniformData) const
{
	GLint uLocation{ glGetUniformLocation(m_programid, uniformName.data()) };
    glUniform2f(uLocation, uniformData.x, uniformData.y);
}

void ShaderProgram::updateUniform2fv(const std::string& uniformName, const std::vector<glm::vec2>& uniformData) const
{
    GLint uLocation{ glGetUniformLocation(m_programid, uniformName.data()) };
    glUniform2fv(uLocation, uniformData.size(), reinterpret_cast<const GLfloat*>(uniformData.data()));
}

void ShaderProgram::updateUniform3f(const std::string& uniformName, const glm::vec3& uniformData) const
{
	GLint uLocation{ glGetUniformLocation(m_programid, uniformName.data()) };
	glUniform3f(uLocation, uniformData.x, uniformData.y, uniformData.z);
}

void ShaderProgram::updateUniform3fv(const std::string& uniformName, const std::vector<glm::vec3>& uniformData) const
{
    GLint uLocation{ glGetUniformLocation(m_programid, uniformName.data()) };
    glUniform3fv(uLocation, uniformData.size(), reinterpret_cast<const GLfloat*>(uniformData.data()));
}

void ShaderProgram::updateUniformMatrix4fv(const std::string& uniformName, const glm::mat4& uniformData) const
{
    GLint uLocation{ glGetUniformLocation(m_programid, uniformName.data()) };
    glUniformMatrix4fv(uLocation, 1, GL_FALSE, glm::value_ptr(uniformData));
}

std::string ShaderProgram::parse(const std::string& path)
{
    std::string shader_source;
	std::ifstream fileStream(path.c_str(), std::ios::in);

	if (!fileStream.is_open()) {
		std::cerr << path << " could not be found.\n";
	}

	std::string line{""};
	while (!fileStream.eof()) {
		std::getline(fileStream, line);
		shader_source.append(line + "\n");
	}

	fileStream.close();

	return shader_source;
}

void ShaderProgram::checkCompileError(const std::string& name, const GLuint& id)
{
    GLint isCompiled = 0;
	glGetShaderiv(id, GL_COMPILE_STATUS, &isCompiled);

	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);
		std::string log;
		log.resize(maxLength);
		glGetShaderInfoLog(id, maxLength, &maxLength, &log[0]);
		std::cout << "Shader '" << name << "' " << "could not be compiled:\n";
		std::cout << log << "\n";
	}
}

std::vector<ShaderPtr>::iterator ShaderProgram::findShader(const std::string& name)
{
	auto it{std::find_if(m_shaders.begin(), m_shaders.end(),
	[=](ShaderPtr& shader) -> bool { return shader->m_name == name;})};
	if (it == m_shaders.end())
	{
		std::cout << "Shader '" << name << "' was not found.\n";
	}
	return it;
}