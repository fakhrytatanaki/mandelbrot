#include "bmplib.h"
#include <stdlib.h>

ImageArray imageArray_init(int width,int height,int channels){

  ImageArray imageArray = {
    .width = width,
    .height = height,
    .channels = channels,
    .data = malloc(width*height*channels)
  };

  memset(imageArray.data,0,width*height*channels);

  return imageArray;

}


void imageArray_delete(ImageArray *imageArray){
  free(imageArray->data);
  imageArray->data=0;
}



void _convert_to_bmp_data(uint8_t* src,uint8_t* dest,int paddingSize,int width,int height,int channels){

  for (int i=0;i < height;i++){
    memcpy(dest + i*(channels*width+paddingSize),src + i*channels*width,channels*width);
  }

}



BmpImage bmp_init(const ImageArray *imageArray){ 
  /*
   NOTE : for now, there are hardcoded values in this initializer, for example,
   compressionMethod is left zero, the only DIB header used is the windowsBitmapInfoHeader 
  */

  const int width = imageArray->width;
  const int height = imageArray->height;
  const int channels = imageArray->channels;
  const int paddingSize = width*channels % 4;
  const uint32_t bmpDataSize = (width*channels + paddingSize)*height;


  uint8_t* bmpData = malloc(bmpDataSize);
  _convert_to_bmp_data(imageArray->data,bmpData,paddingSize,width,height,channels);

  const int totalHeaderSize =  sizeof(BmpHeader) + sizeof(DibHeader);

  const BmpImage img = {

    .paddingSize = paddingSize,

    .bmpHeader = {
      .idField = {'B','M'},
      .fileSize = totalHeaderSize + bmpDataSize,
      .arrOffset = totalHeaderSize
    },

    .dibHeader = {
      .windowsBitmapInfoHeader = {
        .headerSize = sizeof(WindowsBitmapInfoHeader),
        .width = width,
        .height = -height,
        .numColorPlanes = 1,
        .bitsPerPixel = 8*channels,
        .bmpDataSize = bmpDataSize,
        .pixelsPerMeterHorizontal = 2000,
        .pixelsPerMeterVertical = 2000
      }
    },

    .dibHeaderType = BITMAP_INFO_HEADER, 

    .bmpData = bmpData
  };

  return img;
}

void bmp_delete(BmpImage *bmpImage){
  free(bmpImage->bmpData);
  bmpImage->bmpData=0;
}


void bmp_save_file(const BmpImage *bmpImage,const char* fileName){


  FILE *filePointer = fopen(fileName,"wb+"); 
  assert(filePointer!=NULL);

  const uint32_t bmpDataSize = bmpImage->dibHeader.windowsBitmapInfoHeader.bmpDataSize;
  const uint32_t dibHeaderSize = sizeof(DibHeader);
  const uint32_t bmpHeaderSize = sizeof(BmpHeader);

  uint8_t buffer[bmpDataSize + bmpHeaderSize + dibHeaderSize];
  int bufferPtr = 0;

  memcpy(buffer,&bmpImage->bmpHeader,bmpHeaderSize);
  bufferPtr+=bmpHeaderSize;
  memcpy(buffer + bufferPtr,&bmpImage->dibHeader,dibHeaderSize);
  bufferPtr+=dibHeaderSize;
  memcpy(buffer + bufferPtr,bmpImage->bmpData,bmpDataSize);
  bufferPtr+=bmpDataSize;
  fwrite(buffer,bufferPtr,1,filePointer);

  fclose(filePointer);
}


