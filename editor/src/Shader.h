#ifndef _SHADER_H_
#define _SHADER_H_
#include <glew.h>

class Shader
{
	private:
		bool m_initialized;

		GLuint  m_glShaderIndex;
		GLenum m_shaderType;

	public:
		Shader(void);
		~Shader(void);

		bool initialize(GLenum shaderType, const char *shaderPath);
		void shutdown(void);

		bool isInitialized(void) {return m_initialized;}
		GLuint shaderIndex(void) {return m_glShaderIndex;}
		GLenum type(void) {return m_shaderType;}
};

#endif //#ifndef _SHADER_H_
