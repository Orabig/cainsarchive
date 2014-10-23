#ifndef _SHADERMANAGER_H_
#define _SHADERMANAGER_H_
#include <glew.h>
#include <glm/glm.hpp>
#include <QString>
#include <QVector>
#include "Shader.h"
#include "ShaderGroup.h"
#include "Types.h"

class ShaderManager
{
	private:
		bool m_bInitialized;

		QVector<Shader *> m_vShaders;
		QVector<ShaderGroup *> m_vShaderGroups;

	private:
		static ShaderManager *m_pInstance;

		ShaderManager(void);
		ShaderManager(const ShaderManager &ref){}
		ShaderManager &operator=(const ShaderManager &ref){}

	public:
		~ShaderManager(void);

		static ShaderManager *instance(void);
		static void deleteInstance(void);

		bool initialize(void);
		void shutdown(void);

		uint32 newShader(GLenum eShaderType, const QString &szShaderPathName);
		bool deleteShader(uint32 uShaderIndex);

		//Shader Group Functions
		uint32 newGroup(void);
		bool deleteGroup(uint32 uGroupIndex);

		bool addShader(uint32 uGroupIndex, uint32 uShaderIndex);
		bool removeShader(uint32 uGroupIndex, uint32 uShaderIndex);

		bool useGroup(uint32 uGroupIndex);
		bool linkGroup(uint32 uGroupIndex);

		GLint addUniform(uint32 uGroupIndex, const QString &szUniformName);
		GLint addAttribute(uint32 uGroupIndex, const QString &szAttributeName);

		//TODO: Temp functions
		ShaderGroup *group(uint32 uGroupIndex);
};

#endif //#ifndef _SHADERMANAGER_H_
