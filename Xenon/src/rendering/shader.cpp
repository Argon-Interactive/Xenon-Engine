#include"shader.hpp"
#include"devTools/logger/logger_core.hpp"
#include<array>
#include <cstddef>
#include<glad.h>
#include<glfw3.h>
#include<fstream>
#include<memory>
#include<string>
#include<unordered_map>

uint32_t Core::Shader::s_currentBind = 0;

std::string Core::Shader::getShaderName(uint32_t shaderType) {
		switch (shaderType) {
			case(GL_VERTEX_SHADER):
				return "Vertex"; break;
			case(GL_FRAGMENT_SHADER):
				return "Fragment"; break;
			case(GL_GEOMETRY_SHADER): 
				return "Geometry"; break;
			case(GL_COMPUTE_SHADER): 
				return "Compute"; break;
			case(GL_TESS_CONTROL_SHADER): 
				return "Tesselation control"; break;
			case(GL_TESS_EVALUATION_SHADER): 
				return "Tesselation evaluaion"; break;
			default:
				return "Unknown";
		}
}

uint32_t Core::Shader::compileShader(uint32_t type, const char* src)
{
	const uint32_t id = glCreateShader(type);
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);
	//error handling
	int shaderCompiled = 0;
	glGetShaderiv(id, GL_COMPILE_STATUS, &shaderCompiled);
	if (shaderCompiled != GL_TRUE)
	{
		int logLength = 0;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLength);
		std::unique_ptr<char[]> message = std::make_unique<char[]>(static_cast<size_t>(logLength));
		glGetShaderInfoLog(id, logLength, &logLength, message.get());
		XN_LOG_ERR("Compilaton of a shader with a type of \"{0}\" failed. Error message:\n{0}\n", getShaderName(type), message.get());
		return 0;
	}
	return id;
}

uint32_t Core::Shader::linkShader(uint32_t vertexID, uint32_t fragmentID) {
	
	const uint32_t shaderID = glCreateProgram();
	glAttachShader(shaderID, vertexID);
	glAttachShader(shaderID, fragmentID);
	glLinkProgram(shaderID);
	glValidateProgram(shaderID);
	//error handling
	int programCompiled = 0;
	glGetProgramiv(shaderID, GL_LINK_STATUS, &programCompiled);
	if (programCompiled != GL_TRUE)
	{
		int logLength = 0;
		glGetProgramiv(shaderID, GL_INFO_LOG_LENGTH, &logLength);
		std::unique_ptr<char[]> message = std::make_unique<char[]>(static_cast<size_t>(logLength));
		glGetProgramInfoLog(shaderID, logLength, &logLength, message.get());
		XN_LOG_ERR("Linking of a shader program failed. Error message:\n{0}\n", std::string(message.get()));
		return 0;
	}
	return shaderID;
}

bool Core::Shader::getUniformLoc(const std::string& varName, uint32_t id)
{
	if (!(uniformLocs.contains(varName)))
	{
		const int Id = glGetUniformLocation(id, varName.c_str());
		if (Id == -1) {
			XN_LOG_ERR("Shader uniform with a name of \"{0}\" wasn't found", varName);
			glUseProgram(s_currentBind);
			return true;
		}
		uniformLocs[varName] = Id;
	}
	return false;
}

Core::Shader::Shader(const std::string& FilePath)
	: m_ID(0)
{
	std::ifstream shaderFile;
	shaderFile.open(FilePath);
	if (!shaderFile.is_open()) { XN_LOG_ERR("Couldn't open shader file. Path: {0}", FilePath); return; }
	shaderType currType = none;
	std::string currLine;
	std::array<std::string, none> shaderSrc;
	//reading from a shader file
	while (std::getline(shaderFile, currLine))
	{
		if (currLine.find("#shader") != std::string::npos)
		{
			if (currLine.find(" vertex") != std::string::npos) currType = vertex;
			else if (currLine.find(" fragment") != std::string::npos) currType = fragment;
			else if (currLine.find(" geometry") != std::string::npos) XN_LOG_ERR("geometry shader is not supported right now");
			else XN_LOG_ERR("Unsupported shader type in file \"{0}\"", FilePath);
			continue;
		}
		if (currType == none) continue;
		 shaderSrc.at(currType) += currLine + "\n"; 
	}
	if (shaderSrc.at(vertex).empty() || shaderSrc.at(fragment).empty()) { 
		XN_LOG_ERR("Vertex and fragment must be provided and in file \"{0}\" they are not", FilePath);
		return; 
	}
	//compiling and linking shader program
	const uint32_t vertexID = compileShader(GL_VERTEX_SHADER, shaderSrc[vertex].c_str());
	const uint32_t fragmentID = compileShader(GL_FRAGMENT_SHADER, shaderSrc[fragment].c_str());
	m_ID = linkShader(vertexID, fragmentID);
	glDeleteShader(vertexID);
	glDeleteShader(fragmentID);
}

Core::Shader::Shader(const std::string& VertexFilePath, const std::string& FragmentFilePath)
	: m_ID(0)
{
	if (VertexFilePath.empty() || FragmentFilePath.empty()) { 
		XN_LOG_ERR("Vertex and fragment must be provided");
		return; 
	}
	const uint32_t vertexID = compileShader(GL_VERTEX_SHADER, VertexFilePath.c_str());
	const uint32_t fragmentID = compileShader(GL_FRAGMENT_SHADER, FragmentFilePath.c_str());
	m_ID = linkShader(vertexID, fragmentID);
	glDeleteShader(vertexID);
	glDeleteShader(fragmentID);
}
//!floats==================================================================================================================================
void Core::Shader::setUniform1f(const std::string& varName, float v0)
{
	glUseProgram(m_ID);
	if (getUniformLoc(varName, m_ID)) return;
	glUniform1f(uniformLocs[varName], v0);
	glUseProgram(s_currentBind);
}

void Core::Shader::setUniform2f(const std::string& varName, float v0, float v1)
{
	glUseProgram(m_ID);
	if (getUniformLoc(varName, m_ID)) return;
	glUniform2f(uniformLocs[varName], v0, v1);
	glUseProgram(s_currentBind);
}

void Core::Shader::setUniform3f(const std::string& varName, float v0, float v1, float v2)
{
	glUseProgram(m_ID);
	if (getUniformLoc(varName, m_ID)) return;
	glUniform3f(uniformLocs[varName], v0, v1, v2);
	glUseProgram(s_currentBind);
}

void Core::Shader::setUniform4f(const std::string& varName, float v0, float v1, float v2, float v3)
{
	glUseProgram(m_ID);
	if (getUniformLoc(varName, m_ID)) return;
	glUniform4f(uniformLocs[varName], v0, v1, v2, v3);
	glUseProgram(s_currentBind);
}

void Core::Shader::setUniform2f(const std::string& varName, glm::vec2 v0)
{
	glUseProgram(m_ID);
	if (getUniformLoc(varName, m_ID)) return;
	glUniform2f(uniformLocs[varName], v0.x, v0.y);
	glUseProgram(s_currentBind);
}

void Core::Shader::setUniform3f(const std::string& varName, glm::vec3 v0)
{
	glUseProgram(m_ID);
	if (getUniformLoc(varName, m_ID)) return;
	glUniform3f(uniformLocs[varName], v0.x, v0.y, v0.z);
	glUseProgram(s_currentBind);
}

void Core::Shader::setUniform4f(const std::string& varName, glm::vec4 v0)
{
	glUseProgram(m_ID);
	if (getUniformLoc(varName, m_ID)) return;
	glUniform4f(uniformLocs[varName], v0.x, v0.y, v0.z, v0.w);
	glUseProgram(s_currentBind);
}
//!ints==================================================================================================================================
void Core::Shader::setUniform1i(const std::string& varName, int v0)
{
	glUseProgram(m_ID);
	if (getUniformLoc(varName, m_ID)) return;
	glUniform1i(uniformLocs[varName], v0);
	glUseProgram(s_currentBind);
}

void Core::Shader::setUniform2i(const std::string& varName, int v0, int v1)
{
	glUseProgram(m_ID);
	if (getUniformLoc(varName, m_ID)) return;
	glUniform2i(uniformLocs[varName], v0, v1);
	glUseProgram(s_currentBind);
}

void Core::Shader::setUniform3i(const std::string& varName, int v0, int v1, int v2)
{
	glUseProgram(m_ID);
	if (getUniformLoc(varName, m_ID)) return;
	glUniform3i(uniformLocs[varName], v0, v1, v2);
	glUseProgram(s_currentBind);
}

void Core::Shader::setUniform4i(const std::string& varName, int v0, int v1, int v2, int v3)
{
	glUseProgram(m_ID);
	if (getUniformLoc(varName, m_ID)) return;
	glUniform4i(uniformLocs[varName], v0, v1, v2, v3);
	glUseProgram(s_currentBind);
}

void Core::Shader::setUniform2i(const std::string& varName, glm::ivec2 v0)
{
	glUseProgram(m_ID);
	if (getUniformLoc(varName, m_ID)) return;
	glUniform2i(uniformLocs[varName], v0.x, v0.y);
	glUseProgram(s_currentBind);
}

void Core::Shader::setUniform3i(const std::string& varName, glm::ivec3 v0)
{
	glUseProgram(m_ID);
	if (getUniformLoc(varName, m_ID)) return;
	glUniform3i(uniformLocs[varName], v0.x, v0.y, v0.z);
	glUseProgram(s_currentBind);
}

void Core::Shader::setUniform4i(const std::string& varName, glm::ivec4 v0)
{
	glUseProgram(m_ID);
	if (getUniformLoc(varName, m_ID)) return;
	glUniform4i(uniformLocs[varName], v0.x, v0.y, v0.z, v0.w);
	glUseProgram(s_currentBind);
}
//!uints==================================================================================================================================
void Core::Shader::setUniform1u(const std::string& varName, uint32_t v0)
{
	glUseProgram(m_ID);
	if (getUniformLoc(varName, m_ID)) return;
	glUniform1ui(uniformLocs[varName], v0);
	glUseProgram(s_currentBind);
}

void Core::Shader::setUniform2u(const std::string& varName, uint32_t v0, uint32_t v1)
{
	glUseProgram(m_ID);
	if (getUniformLoc(varName, m_ID)) return;
	glUniform2ui(uniformLocs[varName], v0, v1);
	glUseProgram(s_currentBind);
}

void Core::Shader::setUniform3u(const std::string& varName, uint32_t v0, uint32_t v1, uint32_t v2)
{
	glUseProgram(m_ID);
	if (getUniformLoc(varName, m_ID)) return;
	glUniform3ui(uniformLocs[varName], v0, v1, v2);
	glUseProgram(s_currentBind);
}

void Core::Shader::setUniform4u(const std::string& varName, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3)
{
	glUseProgram(m_ID);
	if (getUniformLoc(varName, m_ID)) return;
	glUniform4ui(uniformLocs[varName], v0, v1, v2, v3);
	glUseProgram(s_currentBind);
}

void Core::Shader::setUniform2u(const std::string& varName, glm::uvec2 v0)
{
	glUseProgram(m_ID);
	if (getUniformLoc(varName, m_ID)) return;
	glUniform2ui(uniformLocs[varName], v0.x, v0.y);
	glUseProgram(s_currentBind);
}

void Core::Shader::setUniform3u(const std::string& varName, glm::uvec3 v0)
{
	glUseProgram(m_ID);
	if (getUniformLoc(varName, m_ID)) return;
	glUniform3ui(uniformLocs[varName], v0.x, v0.y, v0.z);
	glUseProgram(s_currentBind);
}

void Core::Shader::setUniform4u(const std::string& varName, glm::uvec4 v0)
{
	glUseProgram(m_ID);
	if (getUniformLoc(varName, m_ID)) return;
	glUniform4ui(uniformLocs[varName], v0.x, v0.y, v0.z, v0.w);
	glUseProgram(s_currentBind);
}
//!vecs==================================================================================================================================
void Core::Shader::setUniform1fv(const std::string& varName, float* ptr, size_t size)
{
	glUseProgram(m_ID);
	if (getUniformLoc(varName, m_ID)) return;
	glUniform1fv(uniformLocs[varName], static_cast<GLsizei>(size), ptr);
	glUseProgram(s_currentBind);
}

void Core::Shader::setUniform1iv(const std::string& varName, int* ptr, size_t size)
{
	glUseProgram(m_ID);
	if (getUniformLoc(varName, m_ID)) return;
	glUniform1iv(uniformLocs[varName], static_cast<GLsizei>(size), ptr);
	glUseProgram(s_currentBind);
}

void Core::Shader::setUniform1uv(const std::string& varName, uint32_t * ptr, size_t size)
{
	glUseProgram(m_ID);
	if (getUniformLoc(varName, m_ID)) return;
	glUniform1uiv(uniformLocs[varName], static_cast<GLsizei>(size), ptr);
	glUseProgram(s_currentBind);
}
void Core::Shader::setUniform1fv(const std::string& varName, const std::vector<float>& arr)
{
	glUseProgram(m_ID);
	if (getUniformLoc(varName, m_ID)) return;
	glUniform1fv(uniformLocs[varName], static_cast<GLsizei>(arr.size()), arr.data());
	glUseProgram(s_currentBind);
}
void Core::Shader::setUniform1iv(const std::string& varName, const std::vector<int>& arr)
{
	glUseProgram(m_ID);
	if (getUniformLoc(varName, m_ID)) return;
	glUniform1iv(uniformLocs[varName], static_cast<GLsizei>(arr.size()), arr.data());
	glUseProgram(s_currentBind);
}
void Core::Shader::setUniform1uv(const std::string& varName, const std::vector<uint32_t>& arr)
{
	glUseProgram(m_ID);
	if (getUniformLoc(varName, m_ID)) return;
	glUniform1uiv(uniformLocs[varName], static_cast<GLsizei>(arr.size()), arr.data());
	glUseProgram(s_currentBind);
}
//!matrixes==================================================================================================================================
void Core::Shader::setUniformMatrix3(const std::string& varName, glm::mat3 v0)
{
	glUseProgram(m_ID);
	if (getUniformLoc(varName, m_ID)) return;
	glUniformMatrix3fv(uniformLocs[varName], 1, GL_FALSE, glm::value_ptr(v0));
	glUseProgram(s_currentBind);
}

void Core::Shader::setUniformMatrix4(const std::string& varName, glm::mat4 v0)
{
	glUseProgram(m_ID);
	if (getUniformLoc(varName, m_ID)) return;
	glUniformMatrix4fv(uniformLocs[varName], 1, GL_FALSE, glm::value_ptr(v0));
	glUseProgram(s_currentBind);
}

Core::Shader::~Shader()
{ if(m_ID != 0) glDeleteProgram(m_ID); }

void Core::Shader::bind() const
{
	glUseProgram(m_ID);
	s_currentBind = m_ID;
}

void Core::Shader::unbind()
{
	glUseProgram(0);
	s_currentBind = 0;
}

void Core::Shader::enableBlending()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
