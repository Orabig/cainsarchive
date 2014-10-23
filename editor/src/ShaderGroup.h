#ifndef _SHADERGROUP_H_
#define _SHADERGROUP_H_
#include <glew.h>
#include <glm/glm.hpp>
#include <QVector>
#include "shader.h"

class ShaderGroup
{
	private:
		GLuint m_glProgramIndex;

		GLint m_MVPMatrixIndex;
		glm::mat4 m_MVPMatrix;

		QVector<Shader *> m_vAttachedShaders;

		bool m_linked;
		bool m_initialized;

	public:
		ShaderGroup(void);
		~ShaderGroup(void);

		bool initialize(void);
		void shutdown(void);

		void addShader(Shader *pShader);
		void removeShader(Shader *pShader);

		bool link(void);

		void use(void);
		bool inUse(void);

		bool isInitialized(void) {return m_initialized;}
		GLuint programIndex(void) {return m_glProgramIndex;}

		bool setMVPMatrix(glm::mat4 newMVPMatrix);
		glm::mat4 MVPMatrix(void) {return m_MVPMatrix;}
};

#endif //#ifndef _SHADERGROUP_H_
