#include"shader.hpp"
#include"devTools/logger/logger_core.hpp"
#include<array>
#include <cstddef>
#include <cstdint>
#include<glad.h>
#include<glfw3.h>
#include<fstream>
#include<memory>
#include<string>
#include<unordered_map>

uint32_t Core::Shader::s_currentBind = 0;

static constexpr std::array<GLenum, 6> glTypes = 
{GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, GL_GEOMETRY_SHADER, GL_COMPUTE_SHADER, GL_TESS_CONTROL_SHADER, GL_TESS_EVALUATION_SHADER};

uint32_t Core::Shader::compileShader(shaderType type, const char* src)
{	
	static constexpr std::array<const char*, 6> shaderNames = 
		{"Vertex", "Fragment", "Geometry", "Compute", "Tesselation control", "Tesselation evaluation"};
	const uint32_t id = glCreateShader(glTypes.at(static_cast<size_t>(type)));
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);
	//error handling
	int shaderCompiled = 0;
	glGetShaderiv(id, GL_COMPILE_STATUS, &shaderCompiled);
	if (shaderCompiled != GL_TRUE)
	{
	int logLength = 0;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLength);
		const std::unique_ptr<char[]> message = std::make_unique<char[]>(static_cast<size_t>(logLength));
		glGetShaderInfoLog(id, logLength, &logLength, message.get());
		XN_LOG_ERR("Compilaton of a shader with a type of {q} failed. Error message:\n{0}", shaderNames.at(type), message.get());
		return 0;
	}
	return id;
}

uint32_t Core::Shader::linkShader(uint32_t vertexID, uint32_t fragmentID, uint32_t geometryID, uint32_t computeID, uint32_t tessControlID, uint32_t tessEvalID) 
{	
	const uint32_t shaderID = glCreateProgram();

	glAttachShader(shaderID, vertexID);
	glAttachShader(shaderID, fragmentID);
	if(geometryID != 0) glAttachShader(shaderID, geometryID);
	if(computeID != 0) glAttachShader(shaderID, computeID);
	if(tessControlID != 0) glAttachShader(shaderID, tessControlID);
	if(tessEvalID != 0) glAttachShader(shaderID, tessEvalID);

	glLinkProgram(shaderID);
	glValidateProgram(shaderID);
	//error handling
	int programCompiled = 0;
	glGetProgramiv(shaderID, GL_LINK_STATUS, &programCompiled);
	if (programCompiled != GL_TRUE)
	{
		int logLength = 0;
		glGetProgramiv(shaderID, GL_INFO_LOG_LENGTH, &logLength);
		const std::unique_ptr<char[]> message = std::make_unique<char[]>(static_cast<size_t>(logLength));
		glGetProgramInfoLog(shaderID, logLength, &logLength, message.get());
		XN_LOG_ERR("Linking of a shader program failed. Error message:\n{0}", message.get());
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
	while (std::getline(shaderFile, currLine)) {
		if (currLine.find("#shader") != std::string::npos)
		{
			if (currLine.find(" vertex") != std::string::npos) currType = vertex;
			else if (currLine.find(" fragment") != std::string::npos) currType = fragment;
			else if (currLine.find(" geometry") != std::string::npos) currType = geometry;
			else if (currLine.find(" compute") != std::string::npos) currType = compute;
			else if (currLine.find(" tess-control") != std::string::npos) currType = tessControl;
			else if (currLine.find(" tess-eval") != std::string::npos) currType = tessEval;
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
	const uint32_t vertexID = compileShader(vertex, shaderSrc[vertex].c_str());
	const uint32_t fragmentID = compileShader(fragment, shaderSrc[fragment].c_str());
	const uint32_t geometryID = (shaderSrc[geometry].empty()) ? 0 : compileShader(fragment, shaderSrc[geometry].c_str());
	const uint32_t computeID = (shaderSrc[geometry].empty()) ? 0 : compileShader(fragment, shaderSrc[compute].c_str());
	const uint32_t tessControlID = (shaderSrc[geometry].empty()) ? 0 : compileShader(fragment, shaderSrc[tessControl].c_str());
	const uint32_t tessEvalID = (shaderSrc[geometry].empty()) ? 0 : compileShader(fragment, shaderSrc[tessEval].c_str());
	
	m_ID = linkShader(vertexID, fragmentID, geometryID, computeID, tessControlID, tessEvalID);
	glDeleteShader(vertexID);
	glDeleteShader(fragmentID);
	if(geometryID != 0) glDeleteShader(geometryID);
	if(computeID != 0) glDeleteShader(computeID);
	if(tessControlID != 0) glDeleteShader(tessControlID);
	if(tessEval != 0) glDeleteShader(tessEvalID);
}

Core::Shader::Shader(const std::string& vertexSrc, const std::string& fragmentSrc, const std::string& geometrySrc, 
					 const std::string& computeSrc, const std::string& tessControlSrc, const std::string& tessEvalSrc)
	: m_ID(0)
{
	if (vertexSrc.empty() || fragmentSrc.empty()) { 
		XN_LOG_ERR("Vertex and fragment shaders must be provided");
		return; 
	}
	const uint32_t vertexID = compileShader(vertex, vertexSrc.c_str());
	const uint32_t fragmentID = compileShader(fragment, fragmentSrc.c_str());
	const uint32_t geometryID = (!geometrySrc.empty()) ? 0 : compileShader(fragment, geometrySrc.c_str());
	const uint32_t computeID = (!computeSrc.empty()) ? 0 : compileShader(fragment, computeSrc.c_str());
	const uint32_t tessControlID = (!tessControlSrc.empty()) ? 0 : compileShader(fragment, tessControlSrc.c_str());
	const uint32_t tessEvalID = (!tessEvalSrc.empty()) ? 0 : compileShader(fragment, tessEvalSrc.c_str());
	m_ID = linkShader(vertexID, fragmentID, geometryID, computeID, tessControlID, tessEvalID);
	glDeleteShader(vertexID);
	glDeleteShader(fragmentID);
	if(geometryID != 0) glDeleteShader(geometryID);
	if(computeID != 0) glDeleteShader(computeID);
	if(tessControlID != 0) glDeleteShader(tessControlID);
	if(tessEval != 0) glDeleteShader(tessEvalID);
}
//!floats==================================================================================================================================
void Core::Shader::setUniform1f(const std::string& varName, float v0)
{
	if(m_ID == 0) return;
	glUseProgram(m_ID);
	if (getUniformLoc(varName, m_ID)) return;
	glUniform1f(uniformLocs[varName], v0);
	glUseProgram(s_currentBind);
}

void Core::Shader::setUniform2f(const std::string& varName, float v0, float v1)
{
	if(m_ID == 0) return;
	glUseProgram(m_ID);
	if (getUniformLoc(varName, m_ID)) return;
	glUniform2f(uniformLocs[varName], v0, v1);
	glUseProgram(s_currentBind);
}

void Core::Shader::setUniform3f(const std::string& varName, float v0, float v1, float v2)
{
	if(m_ID == 0) return;
	glUseProgram(m_ID);
	if (getUniformLoc(varName, m_ID)) return;
	glUniform3f(uniformLocs[varName], v0, v1, v2);
	glUseProgram(s_currentBind);
}

void Core::Shader::setUniform4f(const std::string& varName, float v0, float v1, float v2, float v3)
{
	if(m_ID == 0) return;
	glUseProgram(m_ID);
	if (getUniformLoc(varName, m_ID)) return;
	glUniform4f(uniformLocs[varName], v0, v1, v2, v3);
	glUseProgram(s_currentBind);
}

void Core::Shader::setUniform2f(const std::string& varName, glm::vec2 v0)
{
	if(m_ID == 0) return;
	glUseProgram(m_ID);
	if (getUniformLoc(varName, m_ID)) return;
	glUniform2f(uniformLocs[varName], v0.x, v0.y);
	glUseProgram(s_currentBind);
}

void Core::Shader::setUniform3f(const std::string& varName, glm::vec3 v0)
{
	if(m_ID == 0) return;
	glUseProgram(m_ID);
	if (getUniformLoc(varName, m_ID)) return;
	glUniform3f(uniformLocs[varName], v0.x, v0.y, v0.z);
	glUseProgram(s_currentBind);
}

void Core::Shader::setUniform4f(const std::string& varName, glm::vec4 v0)
{
	if(m_ID == 0) return;
	glUseProgram(m_ID);
	if (getUniformLoc(varName, m_ID)) return;
	glUniform4f(uniformLocs[varName], v0.x, v0.y, v0.z, v0.w);
	glUseProgram(s_currentBind);
}
//!ints==================================================================================================================================
void Core::Shader::setUniform1i(const std::string& varName, int v0)
{
	if(m_ID == 0) return;
	glUseProgram(m_ID);
	if (getUniformLoc(varName, m_ID)) return;
	glUniform1i(uniformLocs[varName], v0);
	glUseProgram(s_currentBind);
}

void Core::Shader::setUniform2i(const std::string& varName, int v0, int v1)
{
	if(m_ID == 0) return;
	glUseProgram(m_ID);
	if (getUniformLoc(varName, m_ID)) return;
	glUniform2i(uniformLocs[varName], v0, v1);
	glUseProgram(s_currentBind);
}

void Core::Shader::setUniform3i(const std::string& varName, int v0, int v1, int v2)
{
	if(m_ID == 0) return;
	glUseProgram(m_ID);
	if (getUniformLoc(varName, m_ID)) return;
	glUniform3i(uniformLocs[varName], v0, v1, v2);
	glUseProgram(s_currentBind);
}

void Core::Shader::setUniform4i(const std::string& varName, int v0, int v1, int v2, int v3)
{
	if(m_ID == 0) return;
	glUseProgram(m_ID);
	if (getUniformLoc(varName, m_ID)) return;
	glUniform4i(uniformLocs[varName], v0, v1, v2, v3);
	glUseProgram(s_currentBind);
}

void Core::Shader::setUniform2i(const std::string& varName, glm::ivec2 v0)
{
	if(m_ID == 0) return;
	glUseProgram(m_ID);
	if (getUniformLoc(varName, m_ID)) return;
	glUniform2i(uniformLocs[varName], v0.x, v0.y);
	glUseProgram(s_currentBind);
}

void Core::Shader::setUniform3i(const std::string& varName, glm::ivec3 v0)
{
	if(m_ID == 0) return;
	glUseProgram(m_ID);
	if (getUniformLoc(varName, m_ID)) return;
	glUniform3i(uniformLocs[varName], v0.x, v0.y, v0.z);
	glUseProgram(s_currentBind);
}

void Core::Shader::setUniform4i(const std::string& varName, glm::ivec4 v0)
{
	if(m_ID == 0) return;
	glUseProgram(m_ID);
	if (getUniformLoc(varName, m_ID)) return;
	glUniform4i(uniformLocs[varName], v0.x, v0.y, v0.z, v0.w);
	glUseProgram(s_currentBind);
}
//!uints==================================================================================================================================
void Core::Shader::setUniform1u(const std::string& varName, uint32_t v0)
{
	if(m_ID == 0) return;
	glUseProgram(m_ID);
	if (getUniformLoc(varName, m_ID)) return;
	glUniform1ui(uniformLocs[varName], v0);
	glUseProgram(s_currentBind);
}

void Core::Shader::setUniform2u(const std::string& varName, uint32_t v0, uint32_t v1)
{
	if(m_ID == 0) return;
	glUseProgram(m_ID);
	if (getUniformLoc(varName, m_ID)) return;
	glUniform2ui(uniformLocs[varName], v0, v1);
	glUseProgram(s_currentBind);
}

void Core::Shader::setUniform3u(const std::string& varName, uint32_t v0, uint32_t v1, uint32_t v2)
{
	if(m_ID == 0) return;
	glUseProgram(m_ID);
	if (getUniformLoc(varName, m_ID)) return;
	glUniform3ui(uniformLocs[varName], v0, v1, v2);
	glUseProgram(s_currentBind);
}

void Core::Shader::setUniform4u(const std::string& varName, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3)
{
	if(m_ID == 0) return;
	glUseProgram(m_ID);
	if (getUniformLoc(varName, m_ID)) return;
	glUniform4ui(uniformLocs[varName], v0, v1, v2, v3);
	glUseProgram(s_currentBind);
}

void Core::Shader::setUniform2u(const std::string& varName, glm::uvec2 v0)
{
	if(m_ID == 0) return;
	glUseProgram(m_ID);
	if (getUniformLoc(varName, m_ID)) return;
	glUniform2ui(uniformLocs[varName], v0.x, v0.y);
	glUseProgram(s_currentBind);
}

void Core::Shader::setUniform3u(const std::string& varName, glm::uvec3 v0)
{
	if(m_ID == 0) return;
	glUseProgram(m_ID);
	if (getUniformLoc(varName, m_ID)) return;
	glUniform3ui(uniformLocs[varName], v0.x, v0.y, v0.z);
	glUseProgram(s_currentBind);
}

void Core::Shader::setUniform4u(const std::string& varName, glm::uvec4 v0)
{
	if(m_ID == 0) return;
	glUseProgram(m_ID);
	if (getUniformLoc(varName, m_ID)) return;
	glUniform4ui(uniformLocs[varName], v0.x, v0.y, v0.z, v0.w);
	glUseProgram(s_currentBind);
}
//!vecs==================================================================================================================================
void Core::Shader::setUniform1fv(const std::string& varName, float* ptr, size_t size)
{
	if(m_ID == 0) return;
	glUseProgram(m_ID);
	if (getUniformLoc(varName, m_ID)) return;
	glUniform1fv(uniformLocs[varName], static_cast<GLsizei>(size), ptr);
	glUseProgram(s_currentBind);
}

void Core::Shader::setUniform1iv(const std::string& varName, int* ptr, size_t size)
{
	if(m_ID == 0) return;
	glUseProgram(m_ID);
	if (getUniformLoc(varName, m_ID)) return;
	glUniform1iv(uniformLocs[varName], static_cast<GLsizei>(size), ptr);
	glUseProgram(s_currentBind);
}

void Core::Shader::setUniform1uv(const std::string& varName, uint32_t * ptr, size_t size)
{
	if(m_ID == 0) return;
	glUseProgram(m_ID);
	if (getUniformLoc(varName, m_ID)) return;
	glUniform1uiv(uniformLocs[varName], static_cast<GLsizei>(size), ptr);
	glUseProgram(s_currentBind);
}
void Core::Shader::setUniform1fv(const std::string& varName, const std::vector<float>& arr)
{
	if(m_ID == 0) return;
	glUseProgram(m_ID);
	if (getUniformLoc(varName, m_ID)) return;
	glUniform1fv(uniformLocs[varName], static_cast<GLsizei>(arr.size()), arr.data());
	glUseProgram(s_currentBind);
}
void Core::Shader::setUniform1iv(const std::string& varName, const std::vector<int>& arr)
{
	if(m_ID == 0) return;
	glUseProgram(m_ID);
	if (getUniformLoc(varName, m_ID)) return;
	glUniform1iv(uniformLocs[varName], static_cast<GLsizei>(arr.size()), arr.data());
	glUseProgram(s_currentBind);
}
void Core::Shader::setUniform1uv(const std::string& varName, const std::vector<uint32_t>& arr)
{
	if(m_ID == 0) return;
	glUseProgram(m_ID);
	if (getUniformLoc(varName, m_ID)) return;
	glUniform1uiv(uniformLocs[varName], static_cast<GLsizei>(arr.size()), arr.data());
	glUseProgram(s_currentBind);
}
//!matrixes==================================================================================================================================
void Core::Shader::setUniformMatrix3(const std::string& varName, glm::mat3 v0)
{
	if(m_ID == 0) return;
	glUseProgram(m_ID);
	if (getUniformLoc(varName, m_ID)) return;
	glUniformMatrix3fv(uniformLocs[varName], 1, GL_FALSE, glm::value_ptr(v0));
	glUseProgram(s_currentBind);
}

void Core::Shader::setUniformMatrix4(const std::string& varName, glm::mat4 v0)
{
	if(m_ID == 0) return;
	glUseProgram(m_ID);
	if (getUniformLoc(varName, m_ID)) return;
	glUniformMatrix4fv(uniformLocs[varName], 1, GL_FALSE, glm::value_ptr(v0));
	glUseProgram(s_currentBind);
}

Core::Shader::~Shader()
{ if(m_ID != 0) glDeleteProgram(m_ID); }

void Core::Shader::bind() const
{
	if(m_ID == 0) return;
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
