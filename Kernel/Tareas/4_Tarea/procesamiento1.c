// Este programa muestra el uso del device driver de caracter
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <math.h>

#define MUESTRAS 4096

void genera_seno(short seno[]);
void genera_ventana(short hamming[]);
void guarda_datos_short(short datos[], char *archivo);
void guarda_datos_int(int datos[], char *archivo);
void guarda_datos_arreglo(short datos[], char *archivo);
void procesamiento(short seno[], short ventana[], short seno_proc[]);

int main(int argc, char const *argv[])
{
	int fd, len;
	short seno[MUESTRAS], ventana[MUESTRAS];
	int seno_proc[MUESTRAS];

	fd = open("/dev/ESCOM_device", O_RDWR);
	if(fd == -1)
	{
		perror("Error al abrir el dispositivo");
		exit(EXIT_FAILURE);
	}

	genera_seno(seno);
	guarda_datos_short(seno, "seno.dat");

	genera_ventana(ventana);
	guarda_datos_short(ventana, "ventana.dat");
	guarda_datos_arreglo(ventana, "ventana.h");

	len = write(fd, seno, MUESTRAS * sizeof(short));
	printf("Bytes enviados: %d\n", len);

	len = read(fd, seno_proc, MUESTRAS * sizeof(int));
	printf("Bytes recibidos: %d\n", len);

	guarda_datos_int(seno_proc, "seno_proc.dat");

	close(fd);

	return 0;
}

void procesamiento(short seno[], short ventana[], short seno_proc[])
{
	register int n;
	int proc;

	for(int n = 0; n < MUESTRAS; n++)
	{
		proc = seno[n] * ventana[n];
			// = Q0 * Q15 = Q15
			// (16 BITS) * (16 BITS) = 32 BITS;
		seno_proc[n] = (short) (proc >> 15); // Q0
	}
}

void genera_ventana(short hamming[])
{
	float a0 = 0.53836, a1 = 0.46164, v;
	register int n;

	for(int n = 0; n < MUESTRAS; n++)
	{
		v = a0 - a1 * cosf(2 * n * M_PI / (MUESTRAS - 1));
		hamming[n] = (short) (v * (1 << 15)); // Q15
	}
}

void genera_seno(short seno[])
{
	float f = 1.3, fs = 512, muestra;
	register int n;

	for(int n = 0; n < MUESTRAS; n++)
	{
		muestra = sinf(2 * n * M_PI * f / fs);
		seno[n] = (short) (muestra * (1 << 10)); // Q0 ---- ADC de 12bits
	}
}

void guarda_datos_arreglo(short	datos[], char *archivo)
{
	FILE *ap_arch;
	register int n;

	ap_arch = fopen(archivo, "w");
	if(!ap_arch)
	{
		perror("Error al abrir el archivo");
		exit(EXIT_FAILURE);
	}

	fprintf(ap_arch, "short ventana[%d] = { ", MUESTRAS);
	for(n = 0; n < MUESTRAS; n++)
	{
		if(n == MUESTRAS - 1)
			fprintf(ap_arch, "%d ", datos[n]);
		else
			fprintf(ap_arch, "%d, ", datos[n]);
	}
		
	fprintf(ap_arch, "};");
	fclose(ap_arch);
}

void guarda_datos_short(short datos[], char *archivo)
{
	FILE *ap_arch;
	register int n;

	ap_arch = fopen(archivo, "w");
	if(!ap_arch)
	{
		perror("Error al abrir el archivo");
		exit(EXIT_FAILURE);
	}

	for(n = 0; n < MUESTRAS; n++)
		fprintf(ap_arch, "%d \n", datos[n]);

	fclose(ap_arch);
}

void guarda_datos_int(int datos[], char *archivo)
{
	FILE *ap_arch;
	register int n;

	ap_arch = fopen(archivo, "w");
	if(!ap_arch)
	{
		perror("Error al abrir el archivo");
		exit(EXIT_FAILURE);
	}

	for(n = 0; n < MUESTRAS; n++)
		fprintf(ap_arch, "%d \n", datos[n]);

	fclose(ap_arch);
}