//  ☀️
#include "bmpreader.h"
#include <stdio.h>
#include <stdlib.h>

unsigned char *getBMPData(int &Width, int &Height, int &bitPerPixel,
                          char *fileName) {
  unsigned char *data = NULL;
  mBITMAPFILEHEADER bfh;
  mBITMAPINFOHEADER bfih;
  unsigned int size;
  bfih.Size = sizeof(mBITMAPINFOHEADER);

  FILE *f = fopen(fileName, "rb");
  if (f != NULL) {
    // ������ ��������� �����
    fread(&bfh.Type, sizeof(unsigned short), 1, f);
    fread(&bfh.Size, sizeof(unsigned int), 1, f);
    fread(&bfh.Reserved1, sizeof(unsigned short), 1, f);
    fread(&bfh.Reserved2, sizeof(unsigned short), 1, f);
    fread(&bfh.OffBits, sizeof(unsigned int), 1, f);

    // ������ ��������������� ���������
    fread(&size, sizeof(unsigned int), 1, f);
    fread(&bfih.Width, sizeof(int), 1, f);
    fread(&bfih.Height, sizeof(int), 1, f);
    fread(&bfih.Planes, sizeof(unsigned short), 1, f);
    fread(&bfih.BitCount, sizeof(unsigned short), 1, f);
    fread(&bfih.Compression, sizeof(unsigned int), 1, f);
    fread(&bfih.SizeImage, sizeof(unsigned int), 1, f);
    fread(&bfih.XPelsPerMeter, sizeof(int), 1, f);
    fread(&bfih.YPelsPerMeter, sizeof(int), 1, f);
    fread(&bfih.ClrUsed, sizeof(unsigned int), 1, f);
    fread(&bfih.ClrImportant, sizeof(unsigned int), 1, f);

    Width = bfih.Width;
    Height = bfih.Height;
    bitPerPixel = bfih.BitCount;

    data = new unsigned char[bfih.SizeImage];
    fseek(f, bfh.OffBits, SEEK_SET);
    fread(data, bfih.SizeImage, 1, f);
    fclose(f);
  }
  return data;
}
// ����� nBitmap
nBitmap::nBitmap() { data = NULL; }

nBitmap::~nBitmap() {
  if (data != NULL)
    delete data;
}

void nBitmap::load(char *fileName) {
  unsigned int size;
  if (data != NULL)
    delete data;
  bih.Size = sizeof(mBITMAPINFOHEADER);

  FILE *f = fopen(fileName, "rb");
  if (f != NULL) {
    // ������ ��������� �����
    fread(&bfh.Type, 2, 1, f);
    fread(&bfh.Size, 4, 1, f);
    fread(&bfh.Reserved1, 2, 1, f);
    fread(&bfh.Reserved2, 2, 1, f);
    fread(&bfh.OffBits, 4, 1, f);

    // ������ ��������������� ���������
    fread(&size, 4, 1, f);
    fread(&bih.Width, 4, 1, f);
    fread(&bih.Height, 4, 1, f);
    fread(&bih.Planes, sizeof(unsigned short), 1, f);
    fread(&bih.BitCount, sizeof(unsigned short), 1, f);
    fread(&bih.Compression, sizeof(unsigned int), 1, f);
    fread(&bih.SizeImage, sizeof(unsigned int), 1, f);
    fread(&bih.XPelsPerMeter, sizeof(int), 1, f);
    fread(&bih.YPelsPerMeter, sizeof(int), 1, f);
    fread(&bih.ClrUsed, sizeof(unsigned int), 1, f);
    fread(&bih.ClrImportant, sizeof(unsigned int), 1, f);
    data = new unsigned char[bih.SizeImage];
    fseek(f, bfh.OffBits, SEEK_SET);
    fread(data, bih.SizeImage, 1, f);
    fclose(f);
  }
  return;
}

int nBitmap::Width() // image in Pixels
{
  return bih.Width;
}

int nBitmap::Height() // image in Pixels
{
  return bih.Height;
}

int nBitmap::BitPerPixel() // color depth
{
  return bih.BitCount;
}

mBITMAPFILEHEADER *nBitmap::getFileHeader() { return &bfh; }
mBITMAPINFOHEADER *nBitmap::getInfoHeader() { return &bih; }

nBitmap24::nBitmap24() : nBitmap() {}

nBitmap24::~nBitmap24() {}

bool nBitmap24::is24bbp() {
  if (BitPerPixel() == 24)
    return true;
  else
    return false;
}

unsigned char *nBitmap24::getRGBData() {
  unsigned char *res = NULL, x;
  int rgbsize = Width() * Height() * 3;
  res = new unsigned char[rgbsize];
  int i, j, k = 0, l = 0, n = Width() % 4;
  for (i = 0; i < Height(); i++) {
    for (j = 0; j < Width() * 3; j++) {
      res[k] = data[l];
      k++;
      l++;
    }
    for (j = 0; j < n; j++)
      l++;
  }
  // меняем BGR на RGB
  for (i = 0; i < rgbsize; i += 3) {
    x = res[i];
    res[i] = res[i + 2];
    res[i + 2] = x;
  }
  return res;
}

unsigned char *nBitmap24::getRGBAData(unsigned char alfa) {
  int rgbsize = Width() * Height() * 4;
  unsigned char *res = NULL, x;
  res = new unsigned char[rgbsize];
  int i, j, k = 0, l = 0, n = Width() % 4;
  for (i = 0; i < Height(); i++) {
    for (j = 0; j < Width() * 3; j++) {
      res[k] = data[l];
      k++;
      l++;
      if (l % 3 == 2) {
        res[k] = alfa;
        k++;
      }
    }
    for (j = 0; j < n; j++)
      l++;
  }
  // меняем BGR на RGB
  for (i = 0; i < rgbsize; i += 4) {
    x = res[i];
    res[i] = res[i + 2];
    res[i + 2] = x;
  }
  return res;
}

unsigned char *nBitmap24::getRGBADataWithTransparentColor(unsigned char tred,
                                                          unsigned char tgreen,
                                                          unsigned char tblue) {
  int rgbsize = Width() * Height() * 4;
  unsigned char *res = NULL, x;
  res = new unsigned char[rgbsize];
  int i, j, k = 0, l = 0, n = Width() % 4;
  for (i = 0; i < Height(); i++) {
    for (j = 0; j < Width() * 3; j++) {
      res[k] = data[l];
      k++;
      l++;
      if ((l - 1) % 3 == 2) {
        res[k] = 255;
        k++;
      }
    }
    for (j = 0; j < n; j++)
      l++;
  }
  // меняем BGR на RGB
  for (i = 0; i < rgbsize; i += 4) {
    x = res[i];
    res[i] = res[i + 2];
    res[i + 2] = x;
    // проверка на прозрачный цвет
    if (res[i] == tred && res[i + 1] == tgreen && res[i + 2] == tblue)
      res[i + 3] = 0;
    else
      res[i + 3] = 255;
  }
  return res;
}
