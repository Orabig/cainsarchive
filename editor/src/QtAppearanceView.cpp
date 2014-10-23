#include "QtAppearanceView.h"
#include <QString>
#include <QDir>
#include <QFile>
#include <glm/gtc/matrix_transform.hpp> 
#include <StormLib.h>
#include "SNOManager.h"
#include "ShaderManager.h"
#include "Logger.h"

AppearanceView::AppearanceView(QWidget *pParent)
:QGLWidget(pParent)
{
	setObjectName(tr("Appearance"));

	m_bOpen = false;

	m_bAlphaDiscard = true;

	m_pCamera = 0;

	m_position = glm::vec3(0.f, 0.f, -15.f);
	m_rotation = glm::vec3(0.f, -90.f, 0.f);

	//Shaders
	m_uShaderGroupIndex = 0;

	m_pFileData = 0;
	m_TextureUnit = 0;

	m_rgbaClearColor.r = 0.0f;
	m_rgbaClearColor.g = 0.0f;
	m_rgbaClearColor.b = 0.0f;
	m_rgbaClearColor.a = 0.0f;
}

void AppearanceView::initialize(void)
{
	initializeShaders();

	m_pCamera = new Camera;
}

void AppearanceView::initializeShaders(void)
{
	//TODO: Better shaders, dynamic, interactive (user GUI choices) of dynamically loaded Diablo 3 shaders
	ShaderManager *pShaderManager = ShaderManager::instance();
	pShaderManager->initialize();

	QString shaderPathName;

	m_uShaderGroupIndex = pShaderManager->newGroup();

	//Textured
	shaderPathName = QDir::currentPath();
	shaderPathName += tr("/textured.vert");
	pShaderManager->addShader(m_uShaderGroupIndex, pShaderManager->newShader(GL_VERTEX_SHADER, shaderPathName));

	shaderPathName = QDir::currentPath();
	shaderPathName += tr("/textured.frag");
	pShaderManager->addShader(m_uShaderGroupIndex, pShaderManager->newShader(GL_FRAGMENT_SHADER, shaderPathName));

	pShaderManager->linkGroup(m_uShaderGroupIndex);
}

bool AppearanceView::open(const uint32 uSNOIndex)
{
	if(m_bOpen)
		close();

	HANDLE hApperanceFile = 0;
	hApperanceFile = SNOManager::instance()->openFile(uSNOIndex);

	if(hApperanceFile == 0)
		return false;

	SFileSetFilePointer(hApperanceFile, 0, 0, FILE_BEGIN);

	uint32 uFileSize = 0;
	uFileSize = (uint32)SFileGetFileSize(hApperanceFile);

	if(uFileSize == 0)
		return false;

	char *pDataBuffer = 0;
	pDataBuffer = new char[uFileSize];
	ZeroMemory(pDataBuffer, uFileSize);

	if(!SFileReadFile(hApperanceFile, pDataBuffer, (DWORD)uFileSize))
	{
		delete [] pDataBuffer;
		return false;
	}

	m_bOpen = open(pDataBuffer, uFileSize);

	return m_bOpen;
}

bool AppearanceView::open(char *pAppearanceData, uint32 uFileSize)
{
	if(pAppearanceData == 0)
		return false;

	if(m_bOpen)
		close();

	m_pFileData = pAppearanceData;
	loadModelGL();

	m_bOpen = true;
	return m_bOpen;
}

void AppearanceView::loadModelGL(void)
{
	if(m_pFileData == 0)
		return;

	//Load Materials
	AppearanceHeader *pAppearanceHeader = (AppearanceHeader *)m_pFileData;
	MaterialHeader *pMaterialHeaders = (MaterialHeader *)&m_pFileData[pAppearanceHeader->uMaterialsOffset + sizeof(Chunkheader)];
	uint32 uMaterialCount = pAppearanceHeader->uMaterialsSize / sizeof(MaterialHeader);

	AppearanceObject *pNewObject;
	for(uint32 i = 0 ; i < uMaterialCount; i++)
	{
		pNewObject = new AppearanceObject;
		pNewObject->szName = pMaterialHeaders[i].szMaterialName;
		pNewObject->pMaterial = new AppearanceMaterial;
		pNewObject->pMaterial->pMaterialHeader = &pMaterialHeaders[i];
		
		Material *pNewSubMaterial = 0;
		uint32 uMaterialCount = pNewObject->pMaterial->pMaterialHeader->uMaterialsSize / sizeof(Material);
		for(uint32 j = 0; j < uMaterialCount; j++)
		{
			AppearanceSubMaterial *pNewAppearanceSubMaterial = new AppearanceSubMaterial;
			pNewAppearanceSubMaterial->pMaterial = (Material *)&m_pFileData[pNewObject->pMaterial->pMaterialHeader->uMaterialsOffset + sizeof(Chunkheader) + j * sizeof(Material)];
			pNewObject->pMaterial->vSubMaterials.push_back(pNewAppearanceSubMaterial);
			uint32 uTextureCount = pNewAppearanceSubMaterial->pMaterial->uTextureListSize / sizeof(MaterialTexture);
			for(uint32 k = 0; k < uTextureCount; k++)
			{
				MaterialTexture *pNewTexture = (MaterialTexture *)&m_pFileData[pNewAppearanceSubMaterial->pMaterial->uTextureListOffset + sizeof(Chunkheader) + k * sizeof(MaterialTexture)];
				pNewAppearanceSubMaterial->vTextures.push_back(pNewTexture);
			}
			
			if(pNewAppearanceSubMaterial->vTextures.size() > 0)
			{
				pNewAppearanceSubMaterial->baseTexture.open(pNewAppearanceSubMaterial->vTextures[0]->uSNOIndex);
			}
		}
		
		m_mObjects.insert(pNewObject->szName, pNewObject);
		pNewObject = 0;
	}

	SubObject *pSubObjectList00 = (SubObject *)&m_pFileData[pAppearanceHeader->uGeoSet00ChunkOffset + sizeof(Chunkheader)];
	SubObject *pSubObjectList01 = (SubObject *)&m_pFileData[pAppearanceHeader->uGeoSet01ChunkOffset + sizeof(Chunkheader)];
	bool bSubObjectList00Populated = false;
	bool bSubObjectList01Populated = false;

	if(pAppearanceHeader->uGeoSet00ChunkCount)
	{
		bSubObjectList00Populated = true;
	}

	if(pAppearanceHeader->uGeoSet01ChunkCount)
	{
		bSubObjectList01Populated = true;
	}

	while(bSubObjectList00Populated || bSubObjectList01Populated)
	{
		uint32 uSubObjectCount = 0;
		SubObject *pFirstSubObject = 0;

		if(bSubObjectList00Populated)
		{
			uSubObjectCount = pAppearanceHeader->uGeoSet00ChunkCount;
			pFirstSubObject = (SubObject *)(&m_pFileData[pAppearanceHeader->uGeoSet00ChunkOffset + sizeof(Chunkheader)]);
			bSubObjectList00Populated = false;
		}

		if(pFirstSubObject == 0)
		{
			if(bSubObjectList01Populated)
			{
				uSubObjectCount = pAppearanceHeader->uGeoSet01ChunkCount;
				pFirstSubObject = (SubObject *)(&m_pFileData[pAppearanceHeader->uGeoSet01ChunkOffset + sizeof(Chunkheader)]);
				bSubObjectList01Populated = false;
			}
		}

		if(pFirstSubObject == 0)
			break;

		//Find model center point TODO: different center point for each object
		glm::vec3 v3MinimumPosition = glm::vec3(0.f, 0.f, 0.f);
		glm::vec3 v3MaximumPosition = glm::vec3(0.f, 0.f, 0.f);

		for(uint32 i = 0; i < uSubObjectCount; i++)
		{
			SubObject *pCurrentSubObject = pFirstSubObject + i;

			ModelVertex *pVertexBuffer = (ModelVertex *)(&m_pFileData[pCurrentSubObject->uVertexListOffset + sizeof(Chunkheader)]);
			for(unsigned int j = 0; j < pCurrentSubObject->uVertexCount; j++)
			{
				if(pVertexBuffer[j].v3Position.x > v3MaximumPosition[0])
				{
					v3MaximumPosition[0] =  pVertexBuffer[j].v3Position.x;
				}

				if(pVertexBuffer[j].v3Position.x < v3MinimumPosition[0])
				{
					v3MinimumPosition[0] =  pVertexBuffer[j].v3Position.x;
				}

				if(pVertexBuffer[j].v3Position.z > v3MaximumPosition[1])
				{
					v3MaximumPosition[1] =  pVertexBuffer[j].v3Position.z;
				}

				if(pVertexBuffer[j].v3Position.z < v3MinimumPosition[1])
				{
					v3MinimumPosition[1] =  pVertexBuffer[j].v3Position.z;
				}

				if(pVertexBuffer[j].v3Position.y > v3MaximumPosition[2])
				{
					v3MaximumPosition[2] =  pVertexBuffer[j].v3Position.y;
				}

				if(pVertexBuffer[j].v3Position.y < v3MinimumPosition[2])
				{
					v3MinimumPosition[2] =  pVertexBuffer[j].v3Position.y;
				}
			}
		}

		m_MidPoint[0] = (v3MinimumPosition[0] + v3MaximumPosition[0]) / 2;
		m_MidPoint[1] = (v3MinimumPosition[1] + v3MaximumPosition[1]) / 2;
		m_MidPoint[2] = (v3MinimumPosition[2] + v3MaximumPosition[2]) / 2;

		for(uint32 i = 0; i < uSubObjectCount; i++)
		{
			AppearanceGeoSet *pNewGeoSet = new AppearanceGeoSet;
			ZeroMemory(pNewGeoSet, sizeof(AppearanceGeoSet));

			pNewGeoSet->pSubObjectHeader = pFirstSubObject + i;

			AppearanceObject *pCurrentObject = 0;
			pCurrentObject = m_mObjects[pNewGeoSet->pSubObjectHeader->szObjectName];

			if(pCurrentObject == 0)
			{
				delete pNewGeoSet;
				continue;
			}

			glGenBuffers(1, &pNewGeoSet->uIndexBufferID);
			glGenBuffers(1, &pNewGeoSet->uVertexBufferID);
			glGenBuffers(1, &pNewGeoSet->uUVBufferID);
			glGenVertexArrays(1, &pNewGeoSet->uVertexArrayID);

			//Load Indices
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pNewGeoSet->uIndexBufferID);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * pNewGeoSet->pSubObjectHeader->uIndexCount, (unsigned short *)(&m_pFileData[pNewGeoSet->pSubObjectHeader->uIndexListOffset + sizeof(Chunkheader)]), GL_STATIC_DRAW);

			//Load Vertices and Unpack UVs
			Vector4D *pVertexList = new Vector4D[pNewGeoSet->pSubObjectHeader->uVertexCount];
			ModelVertex *pVertexBuffer = (ModelVertex *)(&m_pFileData[pNewGeoSet->pSubObjectHeader->uVertexListOffset + sizeof(Chunkheader)]);

			//Vector4D *pNormalsList = new Vector4D[pGeoSet->pSubObjectHeader->uVertexCount];

			pNewGeoSet->pUVs = new Vector2D[pNewGeoSet->pSubObjectHeader->uVertexCount];

			for(unsigned int j = 0; j < pNewGeoSet->pSubObjectHeader->uVertexCount; j++)
			{
				pVertexList[j].x = pVertexBuffer[j].v3Position.x;
				pVertexList[j].y = pVertexBuffer[j].v3Position.z;
				pVertexList[j].z = pVertexBuffer[j].v3Position.y;
				pVertexList[j].w = 1.0f;

				//Correct Object Center
				pVertexList[j].x -= m_MidPoint[0];
				pVertexList[j].y -= m_MidPoint[1];
				pVertexList[j].z -= m_MidPoint[2];

				//Unpack Normals
				//pNormalsList[j].x = (pVertexBuffer[j].packedv4Normal.x - 127) / 127.0f;
				//pNormalsList[j].y = (pVertexBuffer[j].packedv4Normal.y - 127) / 127.0f;
				//pNormalsList[j].z = (pVertexBuffer[j].packedv4Normal.z - 127) / 127.0f;
				//pNormalsList[j].w = pVertexBuffer[j].packedv4Normal.w;

				//Unpack UV
				pNewGeoSet->pUVs[j].x = (pVertexBuffer[j].packedv2UV.u - 32767) / 512.0f;
				pNewGeoSet->pUVs[j].y = (pVertexBuffer[j].packedv2UV.v - 32767) / 512.0f;
			}

			glBindBuffer(GL_ARRAY_BUFFER, pNewGeoSet->uVertexBufferID);
			glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 4 * pNewGeoSet->pSubObjectHeader->uVertexCount, pVertexList, GL_STATIC_DRAW);

			delete [] pVertexList;

			//Load UVs
			glBindBuffer(GL_ARRAY_BUFFER, pNewGeoSet->uUVBufferID);
			glBufferData(GL_ARRAY_BUFFER, sizeof(Vector2D) * pNewGeoSet->pSubObjectHeader->uVertexCount, pNewGeoSet->pUVs, GL_STATIC_DRAW);

			delete [] pNewGeoSet->pUVs;
			pNewGeoSet->pUVs = 0;

			QString szAttributeName;

			szAttributeName = "in_v4Position";
			m_nPositionAttribute = ShaderManager::instance()->addAttribute(m_uShaderGroupIndex, szAttributeName);

			szAttributeName = "in_v2TextureCoord";
			m_nTextureAttribute = ShaderManager::instance()->addAttribute(m_uShaderGroupIndex, szAttributeName);

			QString szUniformName;

			szUniformName = "TextureMap";
			m_nTextureMap = ShaderManager::instance()->addUniform(m_uShaderGroupIndex, szUniformName);
			glUniform1i(m_nTextureMap, m_TextureUnit);

			szUniformName = "AlphaDiscard";
			m_nAlphaDiscard = ShaderManager::instance()->addUniform(m_uShaderGroupIndex, szUniformName);
			glUniform1i(m_nAlphaDiscard, m_bAlphaDiscard);

			glGenSamplers(1, &m_uTextureSampler);
			glSamplerParameteri(m_uTextureSampler, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glSamplerParameteri(m_uTextureSampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glSamplerParameteri(m_uTextureSampler, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glSamplerParameteri(m_uTextureSampler, GL_TEXTURE_WRAP_T, GL_REPEAT);

			//Initialize VAOs
			//Normal Draw
			glBindVertexArray(pNewGeoSet->uVertexArrayID);
			{
				glBindBuffer(GL_ARRAY_BUFFER, pNewGeoSet->uVertexBufferID);

				glEnableVertexAttribArray(m_nPositionAttribute);
				glVertexAttribPointer(m_nPositionAttribute, 4, GL_FLOAT, GL_FALSE, 0, 0);

				glBindBuffer(GL_ARRAY_BUFFER, pNewGeoSet->uUVBufferID);

				glEnableVertexAttribArray(m_nTextureAttribute);
				glVertexAttribPointer(m_nTextureAttribute, 2, GL_FLOAT, GL_FALSE, 0, 0);

				glActiveTexture(GL_TEXTURE0 + m_TextureUnit);
				glBindSampler(m_TextureUnit, m_uTextureSampler);

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pNewGeoSet->uIndexBufferID);
			}
			glBindVertexArray(0);


			pCurrentObject->vGeoSets.push_back(pNewGeoSet);
		}
	}
}

void AppearanceView::addItemToDrawList(AppearanceDrawItem *pNewItem)
{
	if(pNewItem && m_bOpen)
	{
		m_vDrawList.push_back(pNewItem);

		glDraw();
	}
}

void AppearanceView::removeItemFromDrawList(AppearanceDrawItem *pItem)
{
	if(m_bOpen)
	{
		for(int i = 0; i < m_vDrawList.size(); i++)
		{
			if(m_vDrawList[i]->szObjectName == pItem->szObjectName && m_vDrawList[i]->uObjectMaterial == pItem->uObjectMaterial)
			{
				delete m_vDrawList[i];
				m_vDrawList.remove(i);
				glDraw();
			}
		}
	}
}

void AppearanceView::updateDrawList(const QVector<AppearanceDrawItem *> &vNewDrawList)
{
	if(m_bOpen)
	{
		for(int i = 0; i < m_vDrawList.size(); i++)
		{
			if(m_vDrawList[i])
			{
				delete m_vDrawList[i];
			}
		}
		m_vDrawList.clear();

		m_vDrawList = vNewDrawList;
	}
}

QVector<QString> *AppearanceView::objectList(void)
{
	if(!m_bOpen)
		return 0;

	QVector<QString> *pNewObjectList = new QVector<QString>;
	QMapIterator<QString, AppearanceObject *> iObjects(m_mObjects);
	while(iObjects.hasNext())
	{
		iObjects.next();
		pNewObjectList->push_back(iObjects.value()->szName);
	}

	return pNewObjectList;
}

uint32 AppearanceView::objectMaterialCount(const QString &szObjectName)
{
	if(!m_bOpen)
		return 0;

	if(szObjectName.isNull())
		return 0;

	AppearanceObject *pCurrentObject = 0;
	pCurrentObject = m_mObjects[szObjectName];

	if(pCurrentObject)
	{
		return (uint32)pCurrentObject->pMaterial->vSubMaterials.size();
	}

	return 0;
}

void AppearanceView::closeGL(void)
{
	//Unload Models
	QMapIterator<QString, AppearanceObject *> iObjects(m_mObjects);
	while(iObjects.hasNext())
	{
		iObjects.next();
		if(iObjects.value())
		{
			for(int i = 0; i < iObjects.value()->vGeoSets.size(); i++)
			{
				AppearanceGeoSet *pCurrentGeoSet = 0;
				pCurrentGeoSet = iObjects.value()->vGeoSets[i];

				if(pCurrentGeoSet)
				{
					glDeleteBuffers(1, &pCurrentGeoSet->uIndexBufferID);
					glDeleteBuffers(1, &pCurrentGeoSet->uVertexBufferID);
					glDeleteBuffers(1, &pCurrentGeoSet->uUVBufferID);
					glDeleteVertexArrays(1, &pCurrentGeoSet->uVertexArrayID);
					delete pCurrentGeoSet;
				}
			}

			if(iObjects.value()->pMaterial)
			{
				for(int i = 0; i < iObjects.value()->pMaterial->vSubMaterials.size(); i++)
				{
					if(iObjects.value()->pMaterial->vSubMaterials[i])
					{
						delete iObjects.value()->pMaterial->vSubMaterials[i];
					}
				}

				iObjects.value()->pMaterial->vSubMaterials.clear();

				delete iObjects.value()->pMaterial;
			}

			delete iObjects.value();
		}
	}
	m_mObjects.clear();

	ShaderManager::instance()->deleteGroup(m_uShaderGroupIndex);
}

void AppearanceView::close(void)
{
	if(!m_bOpen)
		return;

	closeGL();

	if(m_pFileData)
	{
		delete [] m_pFileData;
		m_pFileData = 0;
	}

	m_bOpen = false;
}

void AppearanceView::keyPressEvent(QKeyEvent *pEvent)
{
	if(pEvent->key() == Qt::Key_Up)
	{
		m_pCamera->setPosition(m_pCamera->xPosition(), m_pCamera->yPosition() + 0.5f, m_pCamera->zPosition());
		glDraw();
		return;
	}

	if(pEvent->key() == Qt::Key_Down)
	{
		m_pCamera->setPosition(m_pCamera->xPosition(), m_pCamera->yPosition() - 0.5f, m_pCamera->zPosition());
		glDraw();
		return;
	}

	if(pEvent->key() == Qt::Key_Right)
	{
		m_pCamera->setPosition(m_pCamera->xPosition() + 0.5f, m_pCamera->yPosition(), m_pCamera->zPosition());
		glDraw();
		return;
	}

	if(pEvent->key() == Qt::Key_Left)
	{
		m_pCamera->setPosition(m_pCamera->xPosition() - 0.5f, m_pCamera->yPosition(), m_pCamera->zPosition());
		glDraw();
		return;
	}
}

void AppearanceView::centerCursor(void)
{
	QPoint newPos = mapToGlobal(pos());
	newPos.setX(newPos.x() + width() / 2);
	newPos.setY(newPos.y() + height() / 2);
	QCursor::setPos(newPos);
}

void AppearanceView::mousePressEvent(QMouseEvent *pEvent)
{
	if(pEvent->button() == Qt::LeftButton)
	{
		centerCursor();
		m_PreviousCursor = cursor();
		setCursor(Qt::BlankCursor);
		setFocus();
	}
}

void AppearanceView::mouseReleaseEvent(QMouseEvent *pEvent)
{
	if(pEvent->button() == Qt::LeftButton)
	{
		setCursor(m_PreviousCursor);
	}
}

void AppearanceView::mouseMoveEvent(QMouseEvent *pEvent)
{
	if(pEvent->buttons() & Qt::LeftButton)
	{
		QPoint cursorDelta = mapToGlobal(pos());
		cursorDelta.setX(cursorDelta.x() + width() / 2);
		cursorDelta.setY(cursorDelta.y() + height() / 2);
		cursorDelta.setX(cursorDelta.x() - QCursor::pos().x());
		cursorDelta.setY(cursorDelta.y() - QCursor::pos().y());

		centerCursor();

		if(cursorDelta.x() != 0)
		{
			if(cursorDelta.x() > 0)
			{
				cursorDelta.setX(-1);
			}
			else
			{
				cursorDelta.setX(1);
			}
		}

		if(cursorDelta.y() != 0)
		{
			if(cursorDelta.y() > 0)
			{
				cursorDelta.setY(-1);
			}
			else
			{
				cursorDelta.setY(1);
			}
		}


		m_rotation[1] = m_rotation[1] + cursorDelta.x() * 1.5f;
		m_rotation[0] = m_rotation[0] + cursorDelta.y() * 1.5f;

		//if(m_pCamera)
		//{
		//	m_pCamera->setRotationY(m_pCamera->yRotation() + cursorDelta.x() * 1.5f);
		//	m_pCamera->setRotationX(m_pCamera->xRotation() + cursorDelta.y() * 1.5f);
		//}

		glDraw();
	}
}

void AppearanceView::wheelEvent(QWheelEvent * pEvent)
{
	m_position[2] += pEvent->delta() / 100.f;
	glDraw();
}

void AppearanceView::initializeGL(void)
{
	GLenum glewResult = glewInit();
	//if(glewResult != GLEW_OK)
	//{
	//}

	glClearColor(m_rgbaClearColor.r, m_rgbaClearColor.g, m_rgbaClearColor.b, m_rgbaClearColor.a);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK); 

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	initialize();
}

void AppearanceView::paintGL(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 model = glm::mat4(1.f);//glm::translate(glm::mat4(1.f), m_pCamera->position());

	model *= glm::translate(glm::mat4(1.0f), m_position);
	model *= glm::rotate(glm::mat4(1.0f), m_rotation[0], glm::vec3(1.f, 0.f, 0.f));
	model *= glm::rotate(glm::mat4(1.0f), m_rotation[1], glm::vec3(0.f, 1.f, 0.f));
	model *= glm::rotate(glm::mat4(1.0f), m_rotation[2], glm::vec3(0.f, 0.f, 1.f));

	if(m_bAlphaDiscard)
	{
		glEnable(GL_BLEND);
	}
	else
	{
		glDisable(GL_BLEND);
	}

	if(m_vDrawList.size() > 0)
	{
		for(int i  = 0; i < m_vDrawList.size(); i++)
		{
			AppearanceObject *pCurrentObject = 0;
			pCurrentObject = m_mObjects[m_vDrawList[i]->szObjectName];

			if(pCurrentObject)
			{
				glBindTexture(GL_TEXTURE_2D, pCurrentObject->pMaterial->vSubMaterials[m_vDrawList[i]->uObjectMaterial]->baseTexture.glTextureIndex());
				for(int i = 0; i < pCurrentObject->vGeoSets.size(); i++)
				{
					ShaderManager::instance()->useGroup(m_uShaderGroupIndex);
					ShaderManager::instance()->group(m_uShaderGroupIndex)->setMVPMatrix(m_projectionMatrix * m_pCamera->viewMatrix() * model);

					glBindVertexArray(pCurrentObject->vGeoSets[i]->uVertexArrayID);
					glDrawElements(GL_TRIANGLES, pCurrentObject->vGeoSets[i]->pSubObjectHeader->uIndexCount, GL_UNSIGNED_SHORT, 0);
					glBindVertexArray(0);
				}
			}
		}
	}
	
	//drawPolylines();
}

void AppearanceView::drawPolylines(void)
{
	//if(m_vModels.size() <= 0)
	//	return;

	//glm::mat4 model = glm::translate(glm::mat4(1.f), m_pCamera->position());

	//m_pShaderGroupWireframe->use();
	//m_pShaderGroupWireframe->setMVPMatrix(m_projectionMatrix * m_pCamera->viewMatrix() * model);

	//glUniform2f(glGetUniformLocation(m_pShaderGroupWireframe->programIndex(),"in_ScaleW"), width() / 2.0f, height() / 2.0f);
	//glUniform3f(glGetUniformLocation(m_pShaderGroupWireframe->programIndex(),"in_Wireframe_Color"), 1.0f, 1.0f, 1.0f);
	//glUniform3f(glGetUniformLocation(m_pShaderGroupWireframe->programIndex(),"in_Fill_Color"), 0.0f, 0.0f, 1.0f);

	//GLuint uPosition01 = glGetAttribLocation(m_pShaderGroupWireframe->programIndex(), "in_Position01");
	//GLuint uPosition02 = glGetAttribLocation(m_pShaderGroupWireframe->programIndex(), "in_Position02");

	////Draw Triangles
	//ModelVertex *pCurrentvertex = 0;
	//for(int i = 0; i < m_vModels.size(); i++)
	//{
	//	for(int j = 0; j < m_vModels[i]->vGeoSets.size(); j ++)
	//	{
	//		ModelVertex *pModelVertexList = (ModelVertex *)(&m_pFileData[m_vModels[i]->vGeoSets[j]->pSubObjectHeader->uVertexListOffset + sizeof(Chunkheader)]);
	//		unsigned short *pModelIndexList = (unsigned short *)(&m_pFileData[m_vModels[i]->vGeoSets[j]->pSubObjectHeader->uIndexListOffset + sizeof(Chunkheader)]);
	//		glBegin(GL_TRIANGLES);
	//		for(unsigned int k = 0; k < m_vModels[i]->vGeoSets[j]->pSubObjectHeader->uIndexCount; k += 3)
	//		{
	//			//Vertex 0
	//			pCurrentvertex = &pModelVertexList[pModelIndexList[k + 1]];
	//			glVertexAttrib4f(uPosition01, pCurrentvertex->v3Position.x, pCurrentvertex->v3Position.z, pCurrentvertex->v3Position.y, 1.0f);

	//			pCurrentvertex = &pModelVertexList[pModelIndexList[k + 2]];
	//			glVertexAttrib4f(uPosition02, pCurrentvertex->v3Position.x, pCurrentvertex->v3Position.z, pCurrentvertex->v3Position.y, 1.0f);

	//			pCurrentvertex = &pModelVertexList[pModelIndexList[k]];
	//			glVertex4f(pCurrentvertex->v3Position.x, pCurrentvertex->v3Position.z, pCurrentvertex->v3Position.y, 0.0f); 

	//			//Vertex 1
	//			pCurrentvertex = &pModelVertexList[pModelIndexList[k + 2]];
	//			glVertexAttrib4f(uPosition01, pCurrentvertex->v3Position.x, pCurrentvertex->v3Position.z, pCurrentvertex->v3Position.y, 1.0f);

	//			pCurrentvertex = &pModelVertexList[pModelIndexList[k]];
	//			glVertexAttrib4f(uPosition02, pCurrentvertex->v3Position.x, pCurrentvertex->v3Position.z, pCurrentvertex->v3Position.y, 1.0f);

	//			pCurrentvertex = &pModelVertexList[pModelIndexList[k + 1]];
	//			glVertex4f(pCurrentvertex->v3Position.x, pCurrentvertex->v3Position.z, pCurrentvertex->v3Position.y, 1.0f); 

	//			//Vertex 2
	//			pCurrentvertex = &pModelVertexList[pModelIndexList[k]];
	//			glVertexAttrib4f(uPosition01, pCurrentvertex->v3Position.x, pCurrentvertex->v3Position.z, pCurrentvertex->v3Position.y, 1.0f);

	//			pCurrentvertex = &pModelVertexList[pModelIndexList[k + 1]];
	//			glVertexAttrib4f(uPosition02, pCurrentvertex->v3Position.x, pCurrentvertex->v3Position.z, pCurrentvertex->v3Position.y, 1.0f);

	//			pCurrentvertex = &pModelVertexList[pModelIndexList[k + 2]];
	//			glVertex4f(pCurrentvertex->v3Position.x, pCurrentvertex->v3Position.z, pCurrentvertex->v3Position.y, 2.0f); 
	//		}
	//		glEnd();
	//	}
	//}
}

void AppearanceView::resizeGL(int nWidth, int nHeight)
{
	glViewport(0, 0, (GLint)nWidth, (GLint)nHeight);

	m_projectionMatrix = glm::perspective(45.0f, (((float)nWidth) / ((float)nHeight)), 0.1f, 1000.f);
	m_orthoMatrix = glm::ortho(0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f);
}

void AppearanceView::setAlphaDiscard(bool bDiscard)
{
	m_bAlphaDiscard = bDiscard;

	if(m_bOpen)
	{
		glUniform1i(m_nAlphaDiscard, m_bAlphaDiscard);

		glDraw();
	}
}

void AppearanceView::setClearColor(const fRGBAColor &color)
{
	m_rgbaClearColor.r = color.r;
	m_rgbaClearColor.g = color.g;
	m_rgbaClearColor.b = color.b;
	m_rgbaClearColor.a = color.a;

	if(m_bOpen)
	{
		glClearColor(m_rgbaClearColor.r, m_rgbaClearColor.g, m_rgbaClearColor.b, m_rgbaClearColor.a);

		glDraw();
	}
}

AppearanceView::~AppearanceView(void)
{
	close();
}
