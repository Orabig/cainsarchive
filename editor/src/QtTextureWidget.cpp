#include "QtTextureWidget.h"

TextureWidget::TextureWidget(void)
{
	setObjectName(tr("Texture"));
	m_bInitialized = false;
	m_pTexture = 0;
}

bool TextureWidget::initialize(void)
{
	if(m_bInitialized)
		return m_bInitialized;

	m_pGLWidget = new QGLWidget;
	m_pGLWidget->makeCurrent();
	GLenum glewResult = glewInit();

	m_bInitialized = true;
	return m_bInitialized;
}

bool TextureWidget::open(SNOTexture *pTexture)
{
	if(!m_bInitialized)
		return false;

	if(pTexture == 0)
		return false;

	m_pTexture = pTexture;

	QImage *pTextureImage = 0;
	pTextureImage = m_pTexture->getImage();

	m_GraphicsScene.setSceneRect(0, 0, pTextureImage->width(), pTextureImage->height());

	m_GraphicsView.setScene(&m_GraphicsScene);
	m_GraphicsView.setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
	m_GraphicsView.setBackgroundBrush(QBrush(Qt::black));

	QPixmap tempMap;
	tempMap.convertFromImage(*pTextureImage);
	m_TextureItem.setPixmap(tempMap);

	m_GraphicsScene.addItem(&m_TextureItem);

	m_Layout.addWidget(&m_GraphicsView);
	setLayout(&m_Layout);

	delete pTextureImage;

	return true;
}

void TextureWidget::shutdown(void)
{
	if(!m_bInitialized)
		return;

	m_bInitialized = false;
}

TextureWidget::~TextureWidget(void)
{
	shutdown();
}
