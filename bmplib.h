#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define DWORD_SIZE_BYTES 4
#define BMP_HEADER_SIZE_BYTES 54

const uint8_t BMP_MAGIC_NUM[] = {0x42,0x4D};


typedef struct {

  int width;
  int height;
  uint8_t* data;
  uint8_t* pixArr;
  uint32_t paddingSize;
  uint32_t payloadSize;
  uint32_t totalSize;


} bmp_image_t;


bmp_image_t bmp_init(int width,int height){

  uint8_t tmp[8] = {0,0,0,0,0,0,0,0};

  bmp_image_t img;

  img.width = width;
  img.height = height;

  int rowSize_bytes = abs(width)*3;
  img.paddingSize = rowSize_bytes % DWORD_SIZE_BYTES;
  img.payloadSize = (rowSize_bytes + img.paddingSize)*abs(height);
  img.totalSize = BMP_HEADER_SIZE_BYTES + img.payloadSize;


  img.data = (uint8_t*) malloc(img.totalSize);
  img.pixArr =img.data + 0x36;

  memcpy(img.data,&BMP_MAGIC_NUM,sizeof(BMP_MAGIC_NUM));

  memcpy(img.data+0x2,&img.totalSize,sizeof(uint32_t));

  tmp[4]=0x36;

  memcpy(img.data+0x6,&tmp,sizeof(tmp));

  tmp[4]=0;

  tmp[0] = 0x28;

  memcpy(img.data+0xe,&tmp,4);
  tmp[0] = 0;

  memcpy(img.data+0x12,&img.width,sizeof(int));

  memcpy(img.data+0x16,&img.height,sizeof(int));

  tmp[0] = 0x1;
  tmp[2] = 0x18;

  memcpy(img.data+0x1a,&tmp,sizeof(tmp));

  tmp[0] = 0x0;
  tmp[2] = 0x0;

  memcpy(img.data+0x22,&img.payloadSize,sizeof(uint32_t));

  tmp[0]=0x13;
  tmp[1]=0x0b;
  tmp[4]=0x13;
  tmp[5]=0x0b;

  memcpy(img.data+0x26,&tmp,sizeof(tmp));

  tmp[0]=0;
  tmp[1]=0;
  tmp[4]=0;
  tmp[5]=0;


  memcpy(img.data+0x2e,&tmp,sizeof(tmp));




  return img;

  //uint32_t pixArraySize = abs(imgConf->width)*abs(imgConf->height)*3;
}


inline void BMP_paintPixel(bmp_image_t* img,int row,int col,uint8_t blue,uint8_t green, uint8_t red){
  uint8_t* pixArr = img->pixArr;
  int w = img->width;
  //int* h = &img->height;

  int paddingOffset = row*img->paddingSize;

  pixArr[paddingOffset + 3*row*(w) + 3*col    ] = blue;
  pixArr[paddingOffset + 3*row*(w) + 3*col + 1] = green;
  pixArr[paddingOffset + 3*row*(w) + 3*col + 2] = red;

}

void BMP_saveAsFile(bmp_image_t* img,const char* filename){

  FILE* imgFile = fopen(filename,"w+b");
  fwrite(img->data,img->totalSize,1,imgFile);
  fclose(imgFile);
}
