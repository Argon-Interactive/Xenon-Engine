#ifndef _XENON_REDNERING_SHADER_
#define _XENON_REDNERING_SHADER_

#include<glm/glm.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<unordered_map>
#include<vector>
#include<string>


namespace Core
{
	class Shader
	{
	public:
		explicit Shader(const std::string& filePath);
		Shader(const std::string& vertexSrc, const std::string& fragmentSrc, const std::string& geometrySrc = "", 
		 const std::string& computeSrc = "", const std::string& tessControlSrc = "", const std::string& tessEvalSrc = "");
		~Shader();
		// It is deleted for now to not couse stupid bugs but it probaly should be implemented at some point
		Shader(const Shader& oth) = delete;
		Shader& operator=(const Shader& oth) = delete;
		Shader(Shader&& oth) = delete;
		Shader& operator=(Shader&& oth) = delete;

		void bind() const;
		static void unbind();
		static void enableBlending();

		void setUniform1f(const std::string& varName, float v0);
		void setUniform2f(const std::string& varName, float v0, float v1);
		void setUniform3f(const std::string& varName, float v0, float v1, float v2);
		void setUniform4f(const std::string& varName, float v0, float v1, float v2, float v3);
		void setUniform2f(const std::string& varName, glm::vec2 v0);
		void setUniform3f(const std::string& varName, glm::vec3 v0);
		void setUniform4f(const std::string& varName, glm::vec4 v0);

		void setUniform1i(const std::string& varName, int v0);
		void setUniform2i(const std::string& varName, int v0, int v1);
		void setUniform3i(const std::string& varName, int v0, int v1, int v2);
		void setUniform4i(const std::string& varName, int v0, int v1, int v2, int v3);
		void setUniform2i(const std::string& varName, glm::ivec2 v0);
		void setUniform3i(const std::string& varName, glm::ivec3 v0);
		void setUniform4i(const std::string& varName, glm::ivec4 v0);

		void setUniform1u(const std::string& varName, uint32_t v0);
		void setUniform2u(const std::string& varName, uint32_t v0, uint32_t v1);
		void setUniform3u(const std::string& varName, uint32_t v0, uint32_t v1, uint32_t v2);
		void setUniform4u(const std::string& varName, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3);
		void setUniform2u(const std::string& varName, glm::uvec2 v0);
		void setUniform3u(const std::string& varName, glm::uvec3 v0);
		void setUniform4u(const std::string& varName, glm::uvec4 v0);
		
		void setUniform1fv(const std::string& varName, float* ptr, size_t size);
		void setUniform1iv(const std::string& varName, int* ptr, size_t size);
		void setUniform1uv(const std::string& varName, uint32_t* ptr, size_t size);

		void setUniform1fv(const std::string& varName, const std::vector<float>& arr);
		void setUniform1iv(const std::string& varName, const std::vector<int>& arr);
		void setUniform1uv(const std::string& varName, const std::vector<uint32_t>& arr);

		void setUniformMatrix3(const std::string& varName, glm::mat3 v0);
		void setUniformMatrix4(const std::string& varName, glm::mat4 v0);
	private:
		unsigned int m_ID;
		static unsigned int s_currentBind;
		std::unordered_map<std::string, int> uniformLocs;
		enum shaderType // "none" should alweys be last, else it may couse bugs
		{ vertex, fragment, geometry, compute, tessControl, tessEval, none };
		bool getUniformLoc(const std::string& varName, uint32_t id);
		static uint32_t compileShader(shaderType type, const char* src);
		static uint32_t linkShader(uint32_t vertexID, uint32_t fragmentID, uint32_t geometryID, uint32_t computeID, uint32_t tessControlID, uint32_t tessEvalID);
	};
}

#endif // !_XENON_REDNERING_SHADER_
