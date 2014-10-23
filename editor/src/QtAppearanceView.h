#ifndef _QTAPPEARANCEVIEW_H_
#define _QTAPPEARANCEVIEW_H_
#include <glew.h>
#include <wglew.h>
#include <QVector>
#include <QMap>
#include <QGLWidget>
#include <QGLFormat>
#include <QMouseEvent>
#include <glm/glm.hpp>
#include "Types.h"
#include "Camera.h"
#include "Shader.h"
#include "ShaderGroup.h"
#include "AppearanceDefinitions.h"
#include "SNOTexture.h"

struct AppearanceGeoSet
{
	SubObject *pSubObjectHeader;
	GLuint uVertexArrayID;
	GLuint uVertexBufferID;
	GLuint uNormalBufferID;
	GLuint uUVBufferID;
	GLuint uIndexBufferID;

	//Model Data Cache
	uint32 uVertexCount;
	Vector3D *pVertices;
	Vector3D *pNormals;
	Vector2D *pUVs;

	uint32 uIndexCount;
	unsigned short *pIndices;
};

struct AppearanceSubMaterial
{
	Material *pMaterial;
	QVector<MaterialTexture *> vTextures;
	SNOTexture baseTexture;
};

struct AppearanceMaterial
{
	MaterialHeader *pMaterialHeader;
	QVector<AppearanceSubMaterial *> vSubMaterials;
};

struct AppearanceObject
{
	QString szName;
	QVector<AppearanceGeoSet *> vGeoSets;
	AppearanceMaterial *pMaterial;
};

struct AppearanceDrawItem
{
	uint32 uObjectMaterial;
	QString szObjectName;
};

#define APPEARANCE_MODEL_COUNT 2 //change this if it ever becomes dynamic

class AppearanceView : public QGLWidget
{
	Q_OBJECT

	private:
		bool m_bOpen;

		bool m_bAlphaDiscard;

		glm::mat4 m_projectionMatrix;
		glm::mat4 m_orthoMatrix;
		Camera *m_pCamera;

		glm::vec3 m_position;
		glm::vec3 m_rotation;
		glm::vec3 m_MidPoint;

		fRGBAColor m_rgbaClearColor;

		//File Data
		char *m_pFileData;
		QMap<QString, AppearanceObject *> m_mObjects;
		QVector<AppearanceDrawItem *> m_vDrawList;

		QCursor m_PreviousCursor;

		void loadModelGL(void);
		void closeGL(void);

	private: //Shaders
		uint32 m_uShaderGroupIndex;

		GLint m_nPositionAttribute;
		GLint m_nTextureAttribute;

		GLint m_nAlphaDiscard;
		GLint m_nTextureMap;
		GLint m_TextureUnit;
		GLuint m_uTextureSampler;

		void initializeShaders(void);

	protected:
		void initializeGL(void);
		void paintGL(void);
		void drawPolylines(void);
		void resizeGL(int nWidth, int nHeight);

		void centerCursor(void);

		void keyPressEvent(QKeyEvent *pEvent);
		void mousePressEvent(QMouseEvent *pEvent);
		void mouseReleaseEvent(QMouseEvent *pEvent);
		void mouseMoveEvent(QMouseEvent *pEvent);
		void wheelEvent(QWheelEvent * pEvent);

		void initialize(void);

		GLuint m_uWatermarkTextureIndex;
		GLuint m_uWatermarkVertexArrayID;
		GLuint m_uWatermarkIndexArrayID;
		GLuint m_uWatermarkVertexBufferID;
		GLuint m_uWatermarkUVBufferID;

	public:
		AppearanceView(QWidget *pParent = 0);
		~AppearanceView(void);

		bool open(const uint32 uSNOIndex);
		bool open(char *pAppearanceData, uint32 uFileSize);

		void close(void);

		void addItemToDrawList(AppearanceDrawItem *pNewItem);
		void removeItemFromDrawList(AppearanceDrawItem *pItem);

		void updateDrawList(const QVector<AppearanceDrawItem *> &vNewDrawList);
		const QVector<AppearanceDrawItem *> &drawList(void){return m_vDrawList;}

		QVector<QString> *objectList(void);
		uint32 objectMaterialCount(const QString &szObjectName);

		bool isOpen(void){return m_bOpen;}
		bool alphaDiscard(void){return m_bAlphaDiscard;}
		void setAlphaDiscard(bool bDiscard);
		const fRGBAColor &clearColor(void){return m_rgbaClearColor;}
		void setClearColor(const fRGBAColor &color);
};

#endif //#ifndef _QTAPPEARANCEVIEW_H_
