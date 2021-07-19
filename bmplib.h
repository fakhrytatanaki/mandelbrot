#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>


/*a simple BMP library*/

#ifndef BMPLIB_H
#define BMPLIB_H


#define BITMAP_INFO_HEADER 1

typedef struct {
  int width;
  int height;
  int channels;
  uint8_t* data;
} ImageArray; //flat image array (without padding)

typedef struct  __attribute__((__packed__)) {
    char idField[2];
    uint32_t fileSize;
    char appData[4];
    uint32_t arrOffset;
} BmpHeader;


typedef struct {
    uint32_t headerSize;
    int width;
    int height;
    uint16_t numColorPlanes;
    uint16_t bitsPerPixel;
    uint32_t compressionMethod;
    uint32_t bmpDataSize;
    uint32_t pixelsPerMeterHorizontal;
    uint32_t pixelsPerMeterVertical;
    uint32_t numColorsPalette;
    uint32_t numImportantColors;
} WindowsBitmapInfoHeader;

typedef union {
  WindowsBitmapInfoHeader windowsBitmapInfoHeader;
} DibHeader;

typedef struct {
    uint8_t paddingSize;
    BmpHeader bmpHeader;
    DibHeader dibHeader;
    int dibHeaderType;
    uint8_t* bmpData;
} BmpImage;


ImageArray imageArray_init(int width,int height,int channels);
void imageArray_delete(ImageArray *imageArray);

BmpImage bmp_init(const ImageArray *imageArray);
void bmp_delete(BmpImage *bmpImage);
void bmp_save_file(const BmpImage *bmpImage,const char* fileName);
void _convert_to_bmp_data(uint8_t* src,uint8_t* dest,int paddingSize,int width,int height,int channels);



static inline void rgb_array_set_pixel(
    ImageArray *imageArray,
    unsigned int row,
    unsigned int col,
    uint8_t red,
    uint8_t green,
    uint8_t blue
    ){
  imageArray->data[row*imageArray->channels*imageArray->width + col*imageArray->channels] = blue;
  imageArray->data[row*imageArray->channels*imageArray->width + col*imageArray->channels + 1] = green;
  imageArray->data[row*imageArray->channels*imageArray->width + col*imageArray->channels + 2] = red;
}

#endif /*BMPLIB_H*/

