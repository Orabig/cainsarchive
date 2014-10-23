#include "ShaderGroup.h"
#include "Logger.h"

ShaderGroup::ShaderGroup(void)
{
	m_glProgramIndex = 0;

	m_MVPMatrixIndex = -1;
	m_MVPMatrix = glm::mat4(1.f);

	m_linked = false;
	m_initialized = false;
}

ShaderGroup::~ShaderGroup(void)
{
	shutdown();
}

void ShaderGroup::addShader(Shader *pShader)
{
	if(!m_initialized)
		return;

	glAttachShader(m_glProgramIndex, pShader->shaderIndex());
	m_vAttachedShaders.push_back(pShader);
}

void ShaderGroup::removeShader(Shader *pShader)
{
	if(!m_initialized)
		return;

	glDetachShader(m_glProgramIndex, pShader->shaderIndex());

	for(int i = 0; i < m_vAttachedShaders.size(); i++)
	{
		if(m_vAttachedShaders[i] == pShader)
		{
			m_vAttachedShaders.remove(i);
		}
	}
}

bool ShaderGroup::link(void)
{
	if(!m_initialized)
		return m_linked;

	GLint glShaderCount = 0;
	glGetProgramiv(m_glProgramIndex, GL_ATTACHED_SHADERS, &glShaderCount);

	if(glShaderCount > 0)
	{
		glLinkProgram(m_glProgramIndex);

		GLint glResult = 0;
		glGetProgramiv(m_glProgramIndex, GL_LINK_STATUS, &glResult);

		if(glResult = GL_TRUE)
		{
			m_MVPMatrixIndex = glGetUniformLocation(m_glProgramIndex, "in_MVPMatrix");

			m_linked = true;
			return m_linked;
		}
		else
		{
			Logger::instance()->logString("Failed to link shader program!\n");
		}
	}

	return m_linked;
}

bool ShaderGroup::setMVPMatrix(glm::mat4 newMVPMatrix)
{
	if(!m_linked)
		return false;

	m_MVPMatrix = newMVPMatrix;
	glUniformMatrix4fv(m_MVPMatrixIndex, 1, GL_FALSE, &m_MVPMatrix[0][0]);

	return true;
}

void ShaderGroup::use(void)
{
	glUseProgram(m_glProgramIndex);
}

bool ShaderGroup::inUse(void)
{
	GLint glResult = 0;
	glGetIntegerv(GL_CURRENT_PROGRAM, &glResult);

	if(m_glProgramIndex == glResult)
		return true;

	return false;
}

bool ShaderGroup::initialize(void)
{
	if(m_initialized)
		return m_initialized;

	m_glProgramIndex = glCreateProgram();

	if(m_glProgramIndex == 0)
	{
		Logger::instance()->logString("Failed to create gl shader program!\n");
		return m_initialized;
	}

	m_initialized = true;

	return m_initialized;
}

void ShaderGroup::shutdown(void)
{
	for(int i = 0; i < m_vAttachedShaders.size(); i++)
	{
		glDetachShader(m_glProgramIndex, m_vAttachedShaders[i]->shaderIndex());
	}
	m_vAttachedShaders.clear();

	glDeleteProgram(m_glProgramIndex);

	m_initialized = false;
}
