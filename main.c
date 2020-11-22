#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "bmplib.h"
#include "cmplx.h"
#include <math.h>


int imgWidth;
int imgHeight;


Cmplx transform(int pixX,int pixY,double xOffset,double yOffset,double scale,int resX,int resY){
    Cmplx r = {.r=(pixX - resX/2)/scale + xOffset,.i=(resY/2-pixY)/scale + yOffset};
    return r;
}


uint8_t mapShape(int v,int peak,double breadth){
    return 255*exp(-pow((v-peak)/breadth,2));

}

uint8_t mandelbrot(Cmplx c,int iterationLimit){
    Cmplx z = {.r=0,.i=0};
    int i=0;

    while (fabs(z.i) < 2 && fabs(z.r) < 2 && i < iterationLimit){
        z = cmplx_add(cmplx_sqr(z),c);
        i++;
    }

    return mapShape(i, 90, 60);

}

void drawMandelbrot(bmp_image_t* img,double x,double y,double scale){
    uint8_t m;
    printf("\nRendering the Mandelbrot set\n");
    for (int i=0;i < imgWidth;i++){

      if ( i%(imgWidth/10)==0 )
        printf("%d %c done \n",(i*100)/imgWidth,37);

        for (int j=0;j < imgHeight;j++){

            m = mandelbrot(transform(i,j,x,y,scale,imgWidth,imgHeight), 200);
            BMP_paintPixel(img,j,i,m,m,m);
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



  bmp_image_t img = bmp_init(imgWidth,-imgHeight);
  imgSizeInMB = img.totalSize/( 1 << 20 );

  while (!(ch=='y' || ch=='Y')){

    printf("estimated image size %.1f MB, would you like to continue? ( (Y)es/(N)o )? : ",imgSizeInMB);
    fflush(stdin);
    ch = getchar();


    if (ch=='n' || ch=='N')
      return 0;

  }

  drawMandelbrot(&img,x, y,scale);



  BMP_saveAsFile(&img,imageDir);

  free(img.data);
  return 0;

}
