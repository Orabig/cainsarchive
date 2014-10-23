#include "ShaderManager.h"

ShaderManager *ShaderManager::m_pInstance = 0;

ShaderManager::ShaderManager(void)
{
	m_bInitialized = false;
}

bool ShaderManager::initialize(void)
{
	if(m_bInitialized)
		return true;

	m_bInitialized = true;
	return m_bInitialized;
}

uint32 ShaderManager::newShader(GLenum eShaderType, const QString &szShaderPathName)
{
	if(!m_bInitialized)
		return 0;

	Shader *pNewShader = new Shader;
	pNewShader->initialize(eShaderType, szShaderPathName.toAscii().data());

	for(int i = 0; i < m_vShaders.size(); i++)
	{
		if(m_vShaders[i] == 0)
		{
			m_vShaders[i] = pNewShader;
			return i;
		}
	}

	m_vShaders.push_back(pNewShader);

	return (uint32)(m_vShaders.size() - 1);
}

bool ShaderManager::deleteShader(uint32 uShaderIndex)
{
	if(!m_bInitialized)
		return false;

	if(uShaderIndex >= (uint32)m_vShaders.size())
		return false;

	if(m_vShaders[uShaderIndex])
	{
		delete m_vShaders[uShaderIndex];
		m_vShaders[uShaderIndex] = 0;

		return true;
	}

	return false;
}

//Shader Group Functions
uint32 ShaderManager::newGroup(void)
{
	if(!m_bInitialized)
		return 0;

	ShaderGroup *pNewGroup = new ShaderGroup;
	pNewGroup->initialize();

	for(int i = 0; i < m_vShaderGroups.size(); i++)
	{
		if(m_vShaderGroups[i] == 0)
		{
			m_vShaderGroups[i] = pNewGroup;
			return i;
		}
	}

	m_vShaderGroups.push_back(pNewGroup);

	return (uint32)(m_vShaderGroups.size() -1);
}

bool ShaderManager::deleteGroup(uint32 uGroupIndex)
{
	if(!m_bInitialized)
		return false;

	if(uGroupIndex >= (uint32)m_vShaderGroups.size())
		return false;

	if(m_vShaderGroups[uGroupIndex])
	{
		delete m_vShaderGroups[uGroupIndex];
		m_vShaderGroups[uGroupIndex] = 0;

		return true;
	}

	return false;
}

bool ShaderManager::addShader(uint32 uGroupIndex, uint32 uShaderIndex)
{
	if(!m_bInitialized)
		return false;

	if(uGroupIndex >= (uint32)m_vShaderGroups.size())
		return false;

	if(uShaderIndex >= (uint32)m_vShaders.size())
		return false;

	if(m_vShaderGroups[uGroupIndex])
	{
		if(m_vShaders[uShaderIndex])
		{
			m_vShaderGroups[uGroupIndex]->addShader(m_vShaders[uShaderIndex]);
			return true;
		}
	}

	return false;
}

bool ShaderManager::removeShader(uint32 uGroupIndex, uint32 uShaderIndex)
{
	if(!m_bInitialized)
		return false;

	if(uGroupIndex >= (uint32)m_vShaderGroups.size())
		return false;

	if(uShaderIndex >= (uint32)m_vShaders.size())
		return false;

	if(m_vShaderGroups[uGroupIndex])
	{
		if(m_vShaders[uShaderIndex])
		{
			m_vShaderGroups[uGroupIndex]->removeShader(m_vShaders[uShaderIndex]);
			return true;
		}
	}

	return false;
}

bool ShaderManager::useGroup(uint32 uGroupIndex)
{
	if(!m_bInitialized)
		return false;

	if(uGroupIndex >= (uint32)m_vShaderGroups.size())
		return false;

	if(m_vShaderGroups[uGroupIndex])
	{
		m_vShaderGroups[uGroupIndex]->use();
		return true;
	}

	return false;
}

bool ShaderManager::linkGroup(uint32 uGroupIndex)
{
	if(!m_bInitialized)
		return false;

	if(uGroupIndex >= (uint32)m_vShaderGroups.size())
		return false;

	if(m_vShaderGroups[uGroupIndex])
	{
		return m_vShaderGroups[uGroupIndex]->link();
	}

	return false;
}

GLint ShaderManager::addUniform(uint32 uGroupIndex, const QString &szUniformName)
{
	if(szUniformName.isNull())
		return -1;

	if(!m_bInitialized)
		return -1;

	if(uGroupIndex >= (uint32)m_vShaderGroups.size())
		return -1;

	if(m_vShaderGroups[uGroupIndex])
	{
		return glGetUniformLocation(m_vShaderGroups[uGroupIndex]->programIndex(), szUniformName.toAscii().data());
	}

	return -1;
}

GLint ShaderManager::addAttribute(uint32 uGroupIndex, const QString &szAttributeName)
{
	if(szAttributeName.isNull())
		return -1;

	if(!m_bInitialized)
		return -1;

	if(uGroupIndex >= (uint32)m_vShaderGroups.size())
		return -1;

	if(m_vShaderGroups[uGroupIndex])
	{
		return glGetAttribLocation(m_vShaderGroups[uGroupIndex]->programIndex(), szAttributeName.toAscii().data());
	}

	return -1;
}

ShaderGroup *ShaderManager::group(uint32 uGroupIndex)
{
	if(!m_bInitialized)
		return 0;

	if(uGroupIndex >= (uint32)m_vShaderGroups.size())
		return 0;

	if(m_vShaderGroups[uGroupIndex])
	{
		return m_vShaderGroups[uGroupIndex];
	}

	return 0;
}

void ShaderManager::shutdown(void)
{
	if(!m_bInitialized)
		return;

	for(int i = 0; i < m_vShaderGroups.size(); i++)
	{
		if(m_vShaderGroups[i])
		{
			delete m_vShaderGroups[i];
			m_vShaderGroups[i] = 0;
		}
	}
	m_vShaderGroups.clear();

	for(int i = 0; i < m_vShaders.size(); i++)
	{
		if(m_vShaders[i])
		{
			delete m_vShaders[i];
			m_vShaders[i] = 0;
		}
	}
	m_vShaders.clear();

	m_bInitialized = false;
}

ShaderManager::~ShaderManager(void)
{
	shutdown();
}

ShaderManager *ShaderManager::instance(void)
{
	if(m_pInstance)
		return m_pInstance;

	m_pInstance = new ShaderManager;

	return m_pInstance;
}

void ShaderManager::deleteInstance(void)
{
	if(m_pInstance)
		delete m_pInstance;

	m_pInstance = 0;
}