#ifndef _QTTEXTUREWIDGET_H_
#define _QTTEXTUREWIDGET_H_
#include <glew.h>
#include <wglew.h>
#include <QDialog>
#include <QVBoxLayout>
#include <QGLWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include "SNOTexture.h"

class TextureWidget : public QDialog
{
	Q_OBJECT

	private:
		bool m_bInitialized;
		SNOTexture *m_pTexture;

		QVBoxLayout m_Layout;

		QGLWidget *m_pGLWidget;
		QGraphicsScene m_GraphicsScene;
		QGraphicsView m_GraphicsView;

		QGraphicsPixmapItem m_TextureItem;

	public:
		TextureWidget(void);
		~TextureWidget(void);

		bool initialize(void);
		void shutdown(void);

		bool open(SNOTexture *pTexture);

		void makeCurrent(void){}
};

#endif //#ifndef _QTTEXTUREWIDGET_H_
