#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include "imagen.h"

#define NUM_HILOS 4

unsigned char *imagenRGB, *imagenGray, *imagenGrayS;
	
void RGBToGray(unsigned char *imagenRGB, unsigned char *imagenGray, uint32_t width, uint32_t height);
void RGBToGray_v2(unsigned char *imagenRGB, unsigned char *imagenGray, uint32_t width, uint32_t height);
unsigned char *reservarMemoria(uint32_t width, uint32_t height);
void GrayToRGB(unsigned char *imagenRGB, unsigned char *imagenGray, uint32_t width, uint32_t height);
void GrayToRGB_v2(unsigned char *imagenRGB, unsigned char *imagenGray, uint32_t width, uint32_t height);
void birghtnessImage(unsigned char *imagenGray, uint32_t width, uint32_t height);
void umbralGlobal(unsigned char *imagenGray, uint32_t width, uint32_t height);
void umbralDinamico(unsigned char *imagenGray, uint32_t width, uint32_t height);
void filtroPasabajas(unsigned char *imagenGray, unsigned char *imagenGrayS, unsigned char *filtro, int factor, int dimension, uint32_t width, uint32_t height);
unsigned char *calcularKernelGauss(int *factor, int dim, double desviacion);
void *funcion_hilo(void *nh);

int main(int argc, char const *argv[])
{
	bmpInfoHeader info;
	unsigned char *filtro;
	int factor = 0;

	pthread_t tids[NUM_HILOS];
	int nhs[NUM_HILOS];
	int *hilo;
	register int nh;


	printf("Abriendo imagen...\n");		
	imagenRGB = abrirBMP("huella1.bmp", &info);
	displayInfo(&info); 

	imagenGray = reservarMemoria(info.width, info.height);
	imagenGrayS = reservarMemoria(info.width, info.height);

	RGBToGray_v2(imagenRGB, imagenGray, info.width, info.height);
	// birghtnessImage(imagenGray, info.width, info.height);
	// umbralGlobal(imagenGray, info.width, info.height);
	// umbralDinamico(imagenGray, info.width, info.height);
	filtro = calcularKernelGauss(&factor, 5, 1.0);

	// filtroPasabajas(imagenGray, imagenGrayS, filtro, factor, 5, info.width, info.height); Antes de Hilos
	for(nh = 0; nh < NUM_HILOS; nh++)
	{
		nhs[nh] = nh;
		pthread_create(&tids[nh], NULL, funcion_hilo, (void *) &nhs[nh]);	
	}

	for(nh = 0; nh < NUM_HILOS; nh++)
	{
		pthread_join(tids[nh], (void **) &hilo);
		printf("Hilo %d terminado\n", *hilo);
	}
	GrayToRGB(imagenRGB, imagenGrayS, info.width, info.height);	
	guardarBMP("huella1Filtro.bmp", &info, imagenRGB);
	
	free(imagenRGB);	
	free(imagenGray);
	free(imagenGrayS);

	return 0;
}

void *funcion_hilo(void *nh)
{
	int core = *(int *)nh;
	int elemsBloque = N / NUM_HILOS;
	int inicioBloque = core  * elemsBloque;
	int finBloque = inicioBloque + elemsBloque;

	for(int i = inicioBloque; i < finBloque; i++)
	{
		P[i] = A[i] * B[i];
	}

	pthread_exit(nh);
}
unsigned char *calcularKernelGauss(int *factor, int dim, double desviacion)
{
	unsigned char *kernelGauss = (unsigned char *) malloc(dim * dim);
	int rango = dim >> 1;
	register int x, y, indice;
	double coef, valor_normalizacion;

	indice = 0;
	*factor = 0;
	for(y = 0; y < dim; y++)
		for(x = 0; x < dim; x++)
		{
			coef = expf( - (( ((x - rango) * (x - rango)) + ((y - rango) * (y - rango)) ) / (2 * desviacion * desviacion) ));
			if(!indice)
				valor_normalizacion = coef;

			kernelGauss[indice] = (unsigned char) (coef / valor_normalizacion);
			*factor += kernelGauss[indice++];
		}

	return kernelGauss;
}

void filtroPasabajas(unsigned char *imagenGray, unsigned char *imagenGrayS, unsigned char *filtro, int factor, int dimension, uint32_t width, uint32_t height)
{
	register int x, y, xb, yb;
	int convolucion, indice;
	// int hn[9] = {1, 3, 1, 3, 5, 3, 1, 3, 1};
	// int suma_filtro;

	// LIMPIANDO EL ARREGLO 
	for(x = 0; x < width * height; x++)
		imagenGrayS[x] = 0;

	// SACAMOS LA SUMA DE LOS ELEMENTOS DEL FILTRO esto es FACTOR
	// for(x = 0; x < bloque * bloque; x++)
		// suma_filtro += hn[x];

	for(y = 0; y < height - dimension; y++)
		for(x = 0; x < width - dimension; x++)
		{
			// HACEMOS LA CONVOLUCION DEL FILTRO CON LA IMAGEN
			convolucion = 0;
			for (yb = 0; yb < dimension; yb++)
				for (xb = 0; xb < dimension; xb++)
				{
					indice = (y + yb) * width + (x + xb);
					convolucion += filtro[yb * dimension + xb] * imagenGray[indice];
				}

			// GUARDAMOS EL RESULTADO	
			convolucion = convolucion / factor;
			indice = (y + (dimension >> 1)) * width + (x + (dimension >> 1));
			imagenGrayS[indice] = convolucion;
		}	
}

void umbralDinamico(unsigned char *imagenGray, uint32_t width, uint32_t height)
{
	register int x, y, xb, yb;
	int media, bloque = 15, indice;

	for(y = 0; y < height; y += bloque)
		for(x = 0; x < width; x += bloque)
		{
			// CALCULAMOS EL UMBRAL USANDO LA MEDIA COMO UMBRAL
			media = 0;
			for (yb = 0; yb < bloque; yb++)
				for (xb = 0; xb < bloque; xb++)
				{
					indice = (y + yb) * width + (x + xb);
					media += imagenGray[indice];
				}

			media = media / (bloque * bloque);
			// APLICAMOS EL UMBRAL
			for (yb = 0; yb < bloque; yb++)
				for (xb = 0; xb < bloque; xb++)
				{
					indice = (y + yb) * width + (x + xb);
					imagenGray[indice] = imagenGray[indice] >= media ? 255 : 0;
				}
		}
}

void umbralGlobal(unsigned char *imagenGray, uint32_t width, uint32_t height)
{
	register int i;
	int nBytesImagen = width * height;
	unsigned char umbral = 170;

	for(i = 0; i < nBytesImagen; i++)
		imagenGray[i] = imagenGray[i] > umbral ? 255 : 0;
}

void birghtnessImage(unsigned char *imagenGray, uint32_t width, uint32_t height)
{
	int nBytesImagen = width * height;
	short pixel;

	for(int i  = 0; i < nBytesImagen; i++)
	{
		pixel = imagenGray[i] + 70;

		imagenGray[i] = pixel > 255 ? 255 : pixel;
	}
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

void GrayToRGB(unsigned char *imagenRGB, unsigned char *imagenGray, uint32_t width, uint32_t height)
{
	register int x, y;
	int indiceGray, indiceRGB;

	for(y = 0; y < height; y++)
		for(x  = 0; x < width; x++)
		{
			indiceGray = y * width + x;
			indiceRGB = indiceGray * 3;
			imagenRGB[indiceRGB] = imagenGray[indiceGray];
			imagenRGB[indiceRGB + 1] = imagenGray[indiceGray];
			imagenRGB[indiceRGB + 2] = imagenGray[indiceGray];
		}
}

void RGBToGray(unsigned char *imagenRGB, unsigned char *imagenGray, uint32_t width, uint32_t height)
{
	register int x, y;
	unsigned char nivelGris;
	int indiceGray, indiceRGB;

	for(y = 0; y < height; y++)
		for(x  = 0; x < width; x++)
		{
			indiceGray = y * width + x;
			indiceRGB = indiceGray * 3;	
			// nivelGris = (imagenRGB[indiceRGB] + imagenRGB[indiceRGB + 1] + imagenRGB[indiceRGB + 2]) / 3;
			nivelGris = (imagenRGB[indiceRGB] * 30 + imagenRGB[indiceRGB + 1] * 59 + imagenRGB[indiceRGB + 2] * 11) / 100;
			imagenGray[indiceGray] = nivelGris;
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