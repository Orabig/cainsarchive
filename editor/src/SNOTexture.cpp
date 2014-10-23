#include "SNOTexture.h"
#include "SNOManager.h"

SNOTexture::SNOTexture(void)
{
	m_bOpen = false;
	m_hSNOFile = 0;
	m_pSNOFileData = 0;
	m_dwSNOFileSize = 0;
	m_pSNOTextureHeader = 0;
	m_pDecodedBuffer = 0;
	m_uDecodedBufferSize = 0;
	m_uTextureIndex = 0;
}

bool SNOTexture::open(const uint32 &uTextureSNOIndex)
{
	if(m_bOpen)
		close();

	//Get the SNO file handle from the manager, read the file size and data.
	m_hSNOFile = SNOManager::instance()->openFile(uTextureSNOIndex);
	if(m_hSNOFile <= 0)
		return false;

	m_dwSNOFileSize = SFileGetFileSize(m_hSNOFile, 0);

	if(m_dwSNOFileSize <= 0)
		return false;

	m_pSNOFileData = new unsigned char[m_dwSNOFileSize];
	ZeroMemory(m_pSNOFileData, m_dwSNOFileSize);

	SFileSetFilePointer(m_hSNOFile, 0, 0, FILE_BEGIN);
	if(!SFileReadFile(m_hSNOFile, m_pSNOFileData, m_dwSNOFileSize))
	{
		m_bOpen = true;
		close();
		return false;
	}

	m_pSNOTextureHeader = (SNOFiles::TextureHeader *)&m_pSNOFileData[sizeof(SNOFiles::SNOHeader)];


	if(!decode())
	{
		m_bOpen = true;
		close();
		return false;
	}

	glLoadTexture();

	m_bOpen = true;
	return m_bOpen;
}

bool SNOTexture::decode(void)
{
	if(m_bOpen)
		return false;

	switch(m_pSNOTextureHeader->uPixelFormat)
	{
		case eTF_D3D_A8R8G8B8:
		case eTF_D3D_A1R5G5B5:
		case eTF_D3D_L8:
		{
			m_pDecodedBuffer = new unsigned char[m_pSNOTextureHeader->levels[0].uSize];
			memcpy_s(m_pDecodedBuffer, m_pSNOTextureHeader->levels[0].uSize, &m_pSNOFileData[sizeof(SNOFiles::SNOHeader) + m_pSNOTextureHeader->levels[0].uOffset], m_pSNOTextureHeader->levels[0].uSize);
			return true;
		}
		break;

		case eTF_DXT1:
		case eTF_DXT1_ALPHA:
		case eTF_DXT3:
		case eTF_DXT5:
		{
			return decodeDXT();
		}
		break;

		default:
			break;
	}

	return false;
}

bool SNOTexture::decodeDXT(void)
{
	if(m_bOpen)
		return false;

	//TODO::Fix for mipmaps
	uint32 uBlockCount = (m_pSNOTextureHeader->uWidth * m_pSNOTextureHeader->uHeight) / 16;

	uint32 *pSrc = (uint32 *)&m_pSNOFileData[sizeof(SNOFiles::SNOHeader) + m_pSNOTextureHeader->levels[0].uOffset];

	switch(m_pSNOTextureHeader->uPixelFormat)
	{
		case eTF_DXT1:
		case eTF_DXT1_ALPHA:
		{
			uint32 *pDecodeBuffer = new uint32[uBlockCount * 2];
			ZeroMemory(pDecodeBuffer, uBlockCount * 2 * sizeof(uint32));

			m_pDecodedBuffer = (unsigned char *)pDecodeBuffer;
			m_uDecodedBufferSize = uBlockCount * 2 * sizeof(uint32);

			uint32 *pPixelColorBlocks = pSrc;
			uint32 *pPixelIndexBlock = pSrc + uBlockCount;
			uint32 uColorBlockPosition = 0;
			uint32 uPixelBlockPosition = 0;
			for(uint32 i = 0; i < uBlockCount * 2; i+=2)
			{
				pDecodeBuffer[i] = pPixelColorBlocks[uColorBlockPosition];
				uColorBlockPosition++;

				pDecodeBuffer[i + 1] = pPixelIndexBlock[uPixelBlockPosition];
				uPixelBlockPosition++;
			}

			return true;
		}
		break;

		case eTF_DXT3:
		{
			uint32 uSubBlockCount = uBlockCount / 4;

			uint32 *pDecodeBuffer = new uint32[uBlockCount * 4];
			ZeroMemory(pDecodeBuffer, uBlockCount * 4 * sizeof(uint32));

			m_pDecodedBuffer = (unsigned char *)pDecodeBuffer;
			m_uDecodedBufferSize = uBlockCount * 4 * sizeof(uint32);

			uint32 *pAlphaBlocks = pSrc;
			uint32 *pPixelColorBlocks = pSrc + uSubBlockCount * 2 * sizeof(uint32);
			uint32 *pPixelIndexBlocks = pSrc + uSubBlockCount * 3 * sizeof(uint32);
			uint32 uAlphaBlockPosition = 0;
			uint32 uPixelBlockPoisiton  = 0;
			for(uint32 i = 0; i < uBlockCount * 4; i+=4)
			{
				pDecodeBuffer[i] = pAlphaBlocks[uAlphaBlockPosition];
				uAlphaBlockPosition++;

				pDecodeBuffer[i + 1] = pAlphaBlocks[uAlphaBlockPosition];
				uAlphaBlockPosition++;

				pDecodeBuffer[i + 2] = pPixelColorBlocks[uPixelBlockPoisiton];
				pDecodeBuffer[i + 3] = pPixelIndexBlocks[uPixelBlockPoisiton];
				uPixelBlockPoisiton++;
			}

			return true;
		}
		break;

		case eTF_DXT5:
		{
			uint32 uSubBlockCount = uBlockCount / 4;

			uint32 *pDecodeBuffer = new uint32[uBlockCount * 4];
			ZeroMemory(pDecodeBuffer, uBlockCount * 4 * sizeof(uint32));

			m_pDecodedBuffer = (unsigned char *)pDecodeBuffer;
			m_uDecodedBufferSize = uBlockCount * 4 * sizeof(uint32);

			uint16 *pAlphaBlocks = (uint16 *)pSrc;
			uint16 *pAlphaIndexBlocks = (uint16 *)(pSrc + uSubBlockCount * sizeof(uint32));
			uint32 *pPixelColorBlocks = pSrc + uSubBlockCount * 2 * sizeof(uint32);
			uint32 *pPixelIndexBlocks = pSrc + uSubBlockCount * 3 * sizeof(uint32);
			uint32 uAlphaBlockPosition = 0;
			uint32 uAlphaIndexBlockPosition = 0;
			uint32 uColorBlockPosition = 0;
			uint32 uPixelBlockPoisiton  = 0;
			for(uint32 i = 0; i < uBlockCount * 4; i+=4)
			{
				*((uint16 *)&pDecodeBuffer[i]) = pAlphaBlocks[uAlphaBlockPosition];
				uAlphaBlockPosition++;

				*(((uint16 *)&pDecodeBuffer[i]) + 1) = pAlphaIndexBlocks[uAlphaIndexBlockPosition];
				uAlphaIndexBlockPosition++;

				*((uint16 *)&pDecodeBuffer[i + 1]) = pAlphaIndexBlocks[uAlphaIndexBlockPosition];
				uAlphaIndexBlockPosition++;

				*(((uint16 *)&pDecodeBuffer[i + 1]) + 1) = pAlphaIndexBlocks[uAlphaIndexBlockPosition];
				uAlphaIndexBlockPosition++;

				pDecodeBuffer[i + 2] = pPixelColorBlocks[uColorBlockPosition];
				uColorBlockPosition++;

				pDecodeBuffer[i + 3] = pPixelIndexBlocks[uPixelBlockPoisiton];
				uPixelBlockPoisiton++;
			}

			return true;
		}
		break;

		default:
			break;
	}

	return false;
}

bool SNOTexture::glLoadTexture(void)
{
	if(m_bOpen)
		return false;

	glGenTextures(1, &m_uTextureIndex);
	glBindTexture(GL_TEXTURE_2D, m_uTextureIndex);

	switch(m_pSNOTextureHeader->uPixelFormat)
	{
		case eTF_D3D_A8R8G8B8:
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_pSNOTextureHeader->uWidth, m_pSNOTextureHeader->uHeight, 0, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, m_pDecodedBuffer);
		}
		break;

		case eTF_D3D_A1R5G5B5:
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB5_A1, m_pSNOTextureHeader->uWidth, m_pSNOTextureHeader->uHeight, 0, GL_BGRA, GL_UNSIGNED_SHORT_1_5_5_5_REV, m_pDecodedBuffer);
		}
		break;

		case eTF_D3D_L8:
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE8, m_pSNOTextureHeader->uWidth, m_pSNOTextureHeader->uHeight, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, m_pDecodedBuffer);
		}
		break;

		case eTF_DXT1:
		{
			glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGB_S3TC_DXT1_EXT, m_pSNOTextureHeader->uWidth, m_pSNOTextureHeader->uHeight, 0, m_uDecodedBufferSize, m_pDecodedBuffer);
		}
		break;

		case eTF_DXT1_ALPHA:
		{
			glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGBA_S3TC_DXT1_EXT, m_pSNOTextureHeader->uWidth, m_pSNOTextureHeader->uHeight, 0, m_uDecodedBufferSize, m_pDecodedBuffer);
		}
		break;

		case eTF_DXT3:
		{
			glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGBA_S3TC_DXT3_EXT, m_pSNOTextureHeader->uWidth, m_pSNOTextureHeader->uHeight, 0, m_uDecodedBufferSize, m_pDecodedBuffer);
		}
		break;

		case eTF_DXT5:
		{
			glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGBA_S3TC_DXT5_EXT, m_pSNOTextureHeader->uWidth, m_pSNOTextureHeader->uHeight, 0, m_uDecodedBufferSize, m_pDecodedBuffer);
		}
		break;

		default:
			break;
	}

	//TODO: load/generate mipmaps?
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);

	return true;
}

bool SNOTexture::save(const QString &filePathName)
{
	if(!m_bOpen)
		return false;

	if(filePathName.isNull())
		return false;

	//TODO: Alpha
	uint32 uImageSize = (m_pSNOTextureHeader->uWidth * m_pSNOTextureHeader->uHeight) * sizeof(uRGBAColor);
	unsigned char *pPixels = new unsigned char[(m_pSNOTextureHeader->uWidth * m_pSNOTextureHeader->uHeight) * 3];

	glBindTexture(GL_TEXTURE_2D, m_uTextureIndex);
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, pPixels);

	QImage newImage(pPixels, m_pSNOTextureHeader->uWidth, m_pSNOTextureHeader->uHeight, QImage::Format_RGB888);

	newImage.save(filePathName);

	return true;
}

QImage *SNOTexture::getImage(void)
{
	if(!m_bOpen)
		return 0;

	//TODO: Alpha
	uint32 uImageSize = (m_pSNOTextureHeader->uWidth * m_pSNOTextureHeader->uHeight) * sizeof(uRGBAColor);
	unsigned char *pPixels = new unsigned char[(m_pSNOTextureHeader->uWidth * m_pSNOTextureHeader->uHeight) * 3];

	glBindTexture(GL_TEXTURE_2D, m_uTextureIndex);
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, pPixels);

	QImage *pNewImage = 0;
	pNewImage = new QImage(pPixels, m_pSNOTextureHeader->uWidth, m_pSNOTextureHeader->uHeight, QImage::Format_RGB888);

	return pNewImage;
}

void SNOTexture::close(void)
{
	if(!m_bOpen)
		return;

	glDeleteTextures(1, &m_uTextureIndex);

	//Note: We do not need to close the handle.. probably..
	m_hSNOFile = 0;

	if(m_pSNOFileData)
	{
		delete [] m_pSNOFileData;
		m_pSNOFileData = 0;
	}
	m_dwSNOFileSize = 0;

	if(m_pDecodedBuffer)
	{
		delete [] m_pDecodedBuffer;
		m_pDecodedBuffer = 0;
	}
	m_uDecodedBufferSize = 0;

	m_pSNOTextureHeader = 0;

	m_bOpen = false;
}

SNOTexture::~SNOTexture(void)
{
	close();
}
