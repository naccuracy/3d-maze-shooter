//  ☀️
#ifndef BMPREADER_H
#define BMPREADER_H

typedef struct tgBITMAPFILEHEADER {
  unsigned short Type;
  unsigned int Size;
  unsigned short Reserved1;
  unsigned short Reserved2;
  unsigned int OffBits;
} mBITMAPFILEHEADER, *mPBITMAPFILEHEADER;

typedef struct tgBITMAPINFOHEADER {
  unsigned int Size;
  int Width;
  int Height;
  unsigned short Planes;
  unsigned short BitCount;
  unsigned int Compression;
  unsigned int SizeImage;
  int XPelsPerMeter;
  int YPelsPerMeter;
  unsigned int ClrUsed;
  unsigned int ClrImportant;
} mBITMAPINFOHEADER, *mPBITMAPINFOHEADER;

unsigned char *getBMPData(int &Width, int &Height, int &bytePerPixel,
                          char *fileName);

class nBitmap // ����� ��� ������ � BMP �������
{
private:
  mBITMAPFILEHEADER bfh;
  mBITMAPINFOHEADER bih;

public:
  nBitmap();
  ~nBitmap();

  unsigned char *data;
  int Width();       // image in Pixels
  int Height();      // image in Pixels
  int BitPerPixel(); // color depth
  void load(char *fileName);
  mBITMAPFILEHEADER *getFileHeader();
  mBITMAPINFOHEADER *getInfoHeader();
};

class nBitmap24 : public nBitmap {
public:
  nBitmap24();
  ~nBitmap24();
  bool is24bbp();
  unsigned char *getRGBData();
  unsigned char *getRGBAData(unsigned char alfa);
  unsigned char *getRGBADataWithTransparentColor(unsigned char tred,
                                                 unsigned char tgreen,
                                                 unsigned char tblue);
};
#endif
