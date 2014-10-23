#ifndef _SNOTEXTURE_H_
#define _SNOTEXTURE_H_
#include <glew.h>
#include <wglew.h>
#include <QString>
#include <QImage>
#include <StormLib.h>
#include "SNODefinitions.h"
#include "Types.h"

class SNOTexture
{
	public:
		enum eTextureFormats
		{
			eTF_D3D_A8R8G8B8 = 0,
			eTF_Unknown00,
			eTF_D3D_X8R8G8B8,
			eTF_D3D_A4R4G4B4,
			eTF_D3D_A1R5G5B5,
			eTF_D3D_X1R5G5B5,
			eTF_D3D_R5G6B5,
			eTF_D3D_L8,
			eTF_Unknown01,
			eTF_DXT1,
			eTF_DXT1_ALPHA,
			eTF_DXT3,
			eTF_DXT5
		};

	private:
		bool m_bOpen;

		HANDLE m_hSNOFile;
		unsigned char *m_pSNOFileData;
		DWORD m_dwSNOFileSize;

		SNOFiles::TextureHeader *m_pSNOTextureHeader;

		unsigned char *m_pDecodedBuffer;
		uint32 m_uDecodedBufferSize;

		GLuint m_uTextureIndex;

		//Texture decoders
		bool decode(void);
		bool decodeDXT(void);

		bool glLoadTexture(void);

	public:
		SNOTexture(void);
		~SNOTexture(void);

		bool open(const uint32 &uTextureSNOIndex);
		bool save(const QString &filePathName);
		void close(void);

		QImage *getImage(void);
		bool isOpen(void){return m_bOpen;}

		GLuint glTextureIndex(void){return m_uTextureIndex;}
};

#endif //#ifndef _SNOTEXTURE_H_
