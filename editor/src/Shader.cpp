#include "shader.h"
#include <QFile>
#include <QString>
#include <vector>
#include <string.h>
#include <iterator>
#include "Logger.h"

Shader::Shader(void)
{
	m_glShaderIndex = 0;
	m_shaderType = 0;

	m_initialized = false;
}

Shader::~Shader(void)
{
	shutdown();
}

bool Shader::initialize(GLenum shaderType, const char *shaderPath)
{
	if(m_initialized)
		return m_initialized;

	QFile shaderFile;
	shaderFile.setFileName(shaderPath);
	shaderFile.open(QIODevice::ReadOnly | QIODevice::Text);

	if(!shaderFile.isOpen())
	{
		return m_initialized;
	}

	unsigned int uFileSize = (unsigned int)shaderFile.size();

	std::vector<std::string> shaderLines;
	std::string line;
	while(!shaderFile.atEnd())
	{
		line = shaderFile.readLine().data();
		line += '\n';
		shaderLines.push_back(line);
	}

	shaderFile.close();

	unsigned int lineCount = shaderLines.size();
	GLchar **shaderData = 0;
	shaderData = new GLchar *[lineCount];
	GLint *shaderLineLengths = new GLint[lineCount];
	for(unsigned int i = 0; i < lineCount; i++)
	{
		shaderLineLengths[i] = shaderLines[i].size();
		shaderData[i] = 0;
		shaderData[i] = new GLchar[shaderLineLengths[i]];
		memset(shaderData[i], 0, shaderLineLengths[i]);
		memcpy(shaderData[i], shaderLines[i].c_str(), shaderLineLengths[i]);
	}

	m_shaderType = shaderType;
	m_glShaderIndex = glCreateShader(m_shaderType);

	glShaderSource(m_glShaderIndex, lineCount, (const GLchar **)shaderData, shaderLineLengths);
	glCompileShader(m_glShaderIndex);
	
	GLint glResult = 0;
	glGetShaderiv(m_glShaderIndex, GL_COMPILE_STATUS, &glResult);

	if(shaderData != 0)
	{
		for(unsigned int i = 0; i < lineCount; i ++)
		{
			if(shaderData[i] != 0)
			{
				delete [] shaderData[i];
				shaderData[i] = 0;
			}
		}

		delete [] shaderData;
		shaderData = 0;
	}

	if(shaderLineLengths != 0)
	{
		delete [] shaderLineLengths;
		shaderLineLengths = 0;
	}

	if(glResult == GL_TRUE)
	{
		m_initialized = true;
	}
	else
	{
		QString logError = "Failed to initialize shader: ";
		logError += shaderPath;
		Logger::instance()->logString(logError + "\n");

		GLint logLength;
		glGetShaderiv(m_glShaderIndex, GL_INFO_LOG_LENGTH, &logLength);

		char *logMessage = new char[logLength];
		glGetShaderInfoLog(m_glShaderIndex, logLength, 0, logMessage);

		logError = logMessage;
		Logger::instance()->logString(logError + "\n");
	}

	return m_initialized;
}

void Shader::shutdown(void)
{
	glDeleteShader(m_glShaderIndex);

	m_initialized = false;
}
