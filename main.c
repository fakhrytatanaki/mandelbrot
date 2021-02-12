#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "bmplib.h"
#include "cmplx.h"
#include <math.h>
#include <unistd.h>  
#include <pthread.h> 

int imgWidth;
int imgHeight;


Cmplx transform(int pixX,int pixY,double xOffset,double yOffset,double scale,int resX,int resY){
    Cmplx r = {.r=(pixX - resX/2.0)/scale + xOffset,.i=(resY/2.0-pixY)/scale + yOffset};
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

//void drawMandelbrot(int t,int n,bmp_image_t* img,double x,double y,double scale){
//
//    uint8_t m;
//
//    for (int i=t*imgWidth/n;i < (t+1)*imgWidth/n;i++){
//
//        for (int j=0;j < imgHeight;j++){
//            m = mandelbrot(transform(i,j,x,y,scale,imgWidth,imgHeight), 200);
//            BMP_paintPixel(img,j,i,m,m,m);
//        }
//    }
//
//
//}

typedef struct {
    int t;
    int n;
    bmp_image_t* img;
    double x;
    double y;
    double scale;

} drawMandelbrotArgs;

pthread_mutex_t printMutex;

void* drawMandelbrot(void* vargp){

    drawMandelbrotArgs* args= (drawMandelbrotArgs*) vargp;

    uint8_t m;

    for (int i=args->t*imgWidth/args->n;i < (args->t+1)*imgWidth/args->n;i++){

        for (int j=0;j < imgHeight;j++){
            m = mandelbrot(transform(i,j,args->x,args->y,args->scale,imgWidth,imgHeight), 200);

            BMP_paintPixel(args->img,j,i,m,m,m);
        }
    }
    pthread_mutex_lock(&printMutex);
    printf("thread %d done\n",args->t);
    pthread_mutex_unlock(&printMutex);

    return NULL;

}





int main(){
    pthread_mutex_init(&printMutex,NULL);

    int numOfThreads = 12;

    pthread_t threads[numOfThreads];
    drawMandelbrotArgs args[numOfThreads];

    
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

    for (int i=0;i < numOfThreads;i++){

        args[i].img = &img;
        args[i].n = numOfThreads;
        args[i].scale = scale;
        args[i].x = x;
        args[i].y = y;
        args[i].t = i;

        pthread_create(&threads[i], NULL, drawMandelbrot, args + i);

    }

//  drawMandelbrot(&img,x, y,scale);

    for (int i=0;i < numOfThreads;i++)
        pthread_join(threads[i],NULL);

    pthread_mutex_destroy(&printMutex);
     

BMP_saveAsFile(&img,imageDir);

    free(img.data);
    pthread_exit(NULL);
    return 0;

}
