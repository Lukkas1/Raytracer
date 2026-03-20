#include "rgbimage.h"

#include <cstdint>
#include <fstream>

#include "color.h"
#include "assert.h"

RGBImage::RGBImage(unsigned int Width, unsigned int Height)
{
	m_Height = Height;
	m_Width = Width;

	m_Image = new Color[m_Width * m_Height];
}

RGBImage::~RGBImage()
{
	delete[] m_Image;
}

void RGBImage::setPixelColor(unsigned int x, unsigned int y, const Color& c)
{
	if (x >= m_Width || y >= m_Height) return;

	unsigned int index = y * m_Width + x;
	m_Image[index] = c;
}

const Color& RGBImage::getPixelColor(unsigned int x, unsigned int y) const
{
	if (x >= m_Width || y >= m_Height) return Color();

	unsigned int index = y * m_Width + x;
	return m_Image[index];
}

unsigned int RGBImage::width() const
{
return m_Width;
}
unsigned int RGBImage::height() const
{
	return m_Height;
}

unsigned char RGBImage::convertColorChannel(float v)
{
	if (v < 0.0f ) return 0;
	if (v > 1.0f) return 255;
	return (unsigned char) (v*255);
}

/**
 * soll das Bild als 24Bit-RGB-Bitmap-Datei speichern
 */
bool RGBImage::saveToDisk(const char* Filename)
{
	FILE* Bitmap = fopen(Filename, "wb");

	//BITMAPFILEHEADER
	uint16_t bfType = 0x4d42; //Hex für BM
	uint32_t bfSize = 0;
	uint32_t bfReseserved = 0;
	uint32_t bfOffBits = 54;

	fwrite(&bfType, sizeof(bfType), 1, Bitmap);
	fwrite(&bfSize, sizeof(bfSize), 1, Bitmap);
	fwrite(&bfReseserved, sizeof(bfReseserved), 1, Bitmap);
	fwrite(&bfOffBits, sizeof(bfOffBits), 1, Bitmap);

	//BITMAPINFOHEADER
	uint32_t biSize = 40;
	int32_t biWidth = m_Width;
	int32_t biHeight = m_Height;
	uint16_t biPlanes = 1;
	uint16_t biBitCount = 24;
	uint32_t biCompression = 0;
	uint32_t biSizeImage = 0;
	int32_t biXPelsPerMeter = 0;
	int32_t biYPelsPerMeter = 0;
	uint32_t biClrUsed = 0;
	uint32_t biClrImportant = 0;

	fwrite(&biSize, sizeof(biSize), 1, Bitmap);
	fwrite(&biWidth, sizeof(biWidth), 1, Bitmap);
	fwrite(&biHeight, sizeof(biHeight), 1, Bitmap);
	fwrite(&biPlanes, sizeof(biPlanes), 1, Bitmap);
	fwrite(&biBitCount, sizeof(biBitCount), 1, Bitmap);
	fwrite(&biCompression, sizeof(biCompression), 1, Bitmap);
	fwrite(&biSizeImage, sizeof(biSizeImage), 1, Bitmap);
	fwrite(&biXPelsPerMeter, sizeof(biXPelsPerMeter), 1, Bitmap);
	fwrite(&biYPelsPerMeter, sizeof(biYPelsPerMeter), 1, Bitmap);
	fwrite(&biClrUsed, sizeof(biClrUsed), 1, Bitmap);
	fwrite(&biClrImportant, sizeof(biClrImportant), 1, Bitmap);

	//Bilddaten
	for (int y = m_Height - 1; y >= 0; y--)
	{
		for (int x = 0; x < m_Width; x++)
		{
			Color pixel = m_Image[y * m_Width + x];
			unsigned char b = convertColorChannel(pixel.B);
			unsigned char g = convertColorChannel(pixel.G);
			unsigned char r = convertColorChannel(pixel.R);

			fwrite(&b, sizeof(unsigned char), 1, Bitmap);
			fwrite(&g, sizeof(unsigned char), 1, Bitmap);
			fwrite(&r, sizeof(unsigned char), 1, Bitmap);
		}
	}

	fclose(Bitmap);
	return true;
}
