#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include "imagen.h"

unsigned char *imagenRGB, *imagenGray;
	
void RGBToGray_v2(unsigned char *imagenRGB, unsigned char *imagenGray, uint32_t width, uint32_t height);
unsigned char *reservarMemoria(uint32_t width, uint32_t height);
void GrayToRGB_v2(unsigned char *imagenRGB, unsigned char *imagenGray, uint32_t width, uint32_t height);

int main(int argc, char const *argv[])
{
	bmpInfoHeader info;

	printf("Abriendo imagen...\n");		
	imagenRGB = abrirBMP("huella1.bmp", &info);
	displayInfo(&info); 

	imagenGray = reservarMemoria(info.width, info.height);
	RGBToGray_v2(imagenRGB, imagenGray, info.width, info.height);
	GrayToRGB_v2(imagenRGB, imagenGrayS, info.width, info.height);	
	guardarBMP("huella1Filtro.bmp", &info, imagenRGB);
	
	free(imagenRGB);	
	free(imagenGray);

	return 0;
}

void GrayToRGB_v2(unsigned char *imagenRGB, unsigned char *imagenGray, uint32_t width, uint32_t height)
{
	int indiceGray = 0;

	int nBytesImagen = width * height * 3;
	for(int i  = 0; i < nBytesImagen; i += 3)
	{
		imagenRGB[i] = imagenGray[indiceGray];
		imagenRGB[i + 1] = imagenGray[indiceGray];
		imagenRGB[i + 2] = imagenGray[indiceGray++];
	}
}

void RGBToGray_v2(unsigned char *imagenRGB, unsigned char *imagenGray, uint32_t width, uint32_t height)
{
	unsigned char nivelGris;
	int indiceGray = 0;
	int nBytesImagen = width * height * 3;

	for(int i = 0; i < nBytesImagen; i += 3)
	{
		nivelGris = (imagenRGB[i] * 30 + imagenRGB[i + 1] * 59 + imagenRGB[i + 2] * 11) / 100;
		imagenGray[indiceGray++] = nivelGris;			
	}	
}

unsigned char *reservarMemoria(uint32_t width, uint32_t height)
{
	unsigned char *imagen;
	imagen = (unsigned char *) malloc(width * height);

	if(imagen == NULL)
	{
		perror("Error: No se pudo asignar memoria");
		exit(EXIT_FAILURE);
	}

	return imagen;
}