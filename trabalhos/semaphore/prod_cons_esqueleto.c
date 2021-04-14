#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <unistd.h>
#include "libimg.h"

//se quiserem converter a imagem ppm (apenas para visualização)
// sudo apt install netpbm

//uma vez instalado:
// ppmtojpeg image_result.ppm > image_result.jpeg

#define BUF_LEN 3

typedef unsigned char uchar;

//regiao critica
//-------------------
uchar buf[BUF_LEN];
int in;
int out;
int count;
int width;
int height;
//-------------------

void *producer(void *tid)
{ 
	long t = (long) tid;
  uchar nextProduced;
	printf("producer thread %ld\n",t);

  FILE *fp = fopen("image_input.ppm","rb");
  if(fp == NULL)
  {
    fprintf(stderr,"error, cannot open file image.pnm\n");
    pthread_exit(NULL);
  }

  ppm_image image = get_ppm(fp);
  if(image == NULL)
  {
    fprintf(stderr,"error, cannot load image image.pnm\n");
    pthread_exit(NULL);
  }
  fclose(fp);
  to_greyscale(image);

  width = image->width;
  height = image->height;

  for(int i = 0 ; i < image->width; i++)
  {
    for(int j = 0 ; j < image->height; j++)
    {
      while(count == BUF_LEN);

      nextProduced = GET_PIXEL(image,i,j)[0];
      buf[in] = nextProduced;      
      in = (in + 1) % BUF_LEN;  
      count++;
    }
  }
  pthread_exit(NULL);
} 

void *consumer(void *tid) 
{ 
	long t = (long) tid;
  int consumed;
  //não pode usar Sleep
	printf("consumer thread %ld\n",t);
  ppm_image image_res = alloc_img(width,height);
  printf("resulting image: %dx%d\n",image_res->width,image_res->height);

  for(int i = 0 ; i < image_res->width ; i++)
  {
    for(int j = 0 ; j < image_res->height ; j++)
    {
      while(count == 0);
      consumed = buf[out];
      put_pixel_unsafe(image_res, i,j, consumed,consumed,consumed);
      out = (out + 1) % BUF_LEN;  
      count--;
    }
  }

  FILE *fp = fopen("image_result.ppm","wb");
  if(fp == NULL)
  {
    fprintf(stderr,"error, cannot open file image_result.ppm\n");
    pthread_exit(NULL);
  }
  output_ppm(fp,image_res);
  fclose(fp);

  pthread_exit(NULL);
} 


int main(int argc, char **argv) 
{ 
	int i, j; 
	pthread_t threadProd;
	pthread_t threadCons;
	int status;

  in = 0;
  out = 0;
  count = 0;
  width = 1;
  height = 1;

  for(int i = 0 ; i < BUF_LEN ; i++)
    buf[i] = 0;

  srand(time(NULL));

	status = pthread_create(&threadProd, NULL, producer, (void*)0);
	if(status != 0)
	{
		printf("Cannot create thread\n");
		exit(1);
	}
	
	status = pthread_create(&threadCons, NULL, consumer, (void*)1);
	if(status != 0)
	{
		printf("Cannot create thread\n");
		exit(1);
	}
	
	pthread_join(threadProd,NULL);
	pthread_join(threadCons,NULL);
	
  return 0; 
} 
