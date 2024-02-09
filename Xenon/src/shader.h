#pragma once
#include<all.hpp>
#include<glad.h>
#include<glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<unordered_map>


namespace xen
{
	class Shader
	{
	public:
		Shader(const Shader&) = delete;
		Shader(const std::string& FilePath);
		Shader(const std::string& VertexFilePath, const std::string& FragmentFilePath, const std::string& GeometryFilePath = "");
		~Shader();
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

		template<size_t S>
		void setUniform1fv(const std::string& varName, const std::array<float, S>& arr)
		{
			glUseProgram(m_ID);
			if (getUniformLoc(varName, m_ID)) return;
			glUniform1fv(uniformLocs[varName], S, &arr[0]);
			glUseProgram(sm_currBind);
		}
		template<size_t S>
		void setUniform1iv(const std::string& varName, const std::array<int, S>& arr)
		{
			glUseProgram(m_ID);
			if (getUniformLoc(varName, m_ID)) return;
			glUniform1iv(uniformLocs[varName], S, &arr[0]);
			glUseProgram(sm_currBind);
		}
		template<size_t S>
		void setUniform1uv(const std::string& varName, const std::array<uint32_t, S>& arr)
		{
			glUseProgram(m_ID);
			if (getUniformLoc(varName, m_ID)) return;
			glUniform1uiv(uniformLocs[varName], S, &arr[0]);
			glUseProgram(sm_currBind);
		}
				
		void setUniformMatrix3(const std::string& varName, glm::mat3 v0);
		void setUniformMatrix4(const std::string& varName, glm::mat4 v0);
	private:
		unsigned int m_ID;
		static unsigned int sm_currBind;
		std::unordered_map<std::string, int> uniformLocs;
		enum shaderType
		{ vertex, fragment, geometry, none };
		bool getUniformLoc(const std::string& varName, uint32_t id);
		static unsigned int CompileShader(unsigned int type, const std::string& src);
	};
}