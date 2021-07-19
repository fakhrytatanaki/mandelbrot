#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "bmplib.h"
#include "cmplx.h"
#include <math.h>


int imgWidth;
int imgHeight;


int calculateImageSize(ImageArray* img){
  const int padding = img->width*img->channels % 4;
  return 54 + img->height*(img->width*img->channels + padding);
}

Cmplx transform(int pixX,int pixY,double xOffset,double yOffset,double scale,int resX,int resY){
  Cmplx r = {.r=(pixX - resX/2)/scale + xOffset,.i=(resY/2-pixY)/scale + yOffset};
  return r;
}


uint8_t gaussian(int v,int avg,double stdDev){
  return 255*exp(-pow((v-avg)/stdDev,2));
}

uint8_t mandelbrot(Cmplx c,int iterationLimit){

  Cmplx z = {.r=0,.i=0};
  int i=0;

  while (fabs(z.i) < 2 && fabs(z.r) < 2 && i < iterationLimit){
    z = cmplx_add(cmplx_sqr(z),c);
    i++;
  }

  return gaussian(i, 90, 60);

}

void drawMandelbrot(ImageArray* img,double x,double y,double scale){
  uint8_t m;
  printf("\nRendering the Mandelbrot set\n");
  for (int i=0;i < imgWidth;i++){

    if ( i%(imgWidth/10)==0 )
      printf("%d %c done \n",(i*100)/imgWidth,37);

    for (int j=0;j < imgHeight;j++){

      m = mandelbrot(transform(i,j,x,y,scale,imgWidth,imgHeight), 200);
      rgb_array_set_pixel(img,j,i,m,m,m);
    }
  }

  printf("100 %c done",37);

}






int main(){
  char ch='f';
  double scale,x,y;
  char imageDir[64];
  float imgSizeInMB;

  printf("width of the image : ");
  scanf("%d",&imgWidth);
  printf("height of the image : ");
  scanf("%d",&imgHeight);
  printf("scale of the mandelbrot view : ");
  scanf("%lf",&scale);
  printf("x axis of the mandelbrot view : ");
  scanf("%lf",&x);
  printf("y axis of the mandelbrot view : ");
  scanf("%lf",&y);

  printf("new image file : ");
  scanf("%s",imageDir);



  ImageArray img = imageArray_init(imgWidth,imgHeight,3);
  imgSizeInMB = calculateImageSize(&img)/( 1 << 20 );

  while (!(ch=='y' || ch=='Y')){

    printf("estimated image size %.1f MB, would you like to continue? ( (Y)es/(N)o )? : ",imgSizeInMB);
    fflush(stdin);
    ch = getchar();


    if (ch=='n' || ch=='N')
      return 0;

  }

  drawMandelbrot(&img,x, y,scale);


  BmpImage bmpImage = bmp_init(&img);
  bmp_save_file(&bmpImage,imageDir);

  imageArray_delete(&img);
  bmp_delete(&bmpImage);
  
  return 0;

}
