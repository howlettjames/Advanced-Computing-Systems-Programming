// Este programa muestra el uso del device driver de caracter
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <math.h>

#define MUESTRAS 1024

void genera_seno(short seno[]);
void genera_ventana(short hamming[]);
void guarda_datos(short datos[], char *archivo);
void procesamiento(short seno[], short ventana[], short seno_proc[]);

int main(int argc, char const *argv[])
{
	short seno[MUESTRAS], seno_proc[MUESTRAS], ventana[MUESTRAS];

	genera_seno(seno);
	guarda_datos(seno, "seno.dat");

	genera_ventana(ventana);
	guarda_datos(ventana, "ventana.dat");

	procesamiento(seno, ventana, seno_proc);
	guarda_datos(seno_proc, "seno_proc.dat");

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
	float f = 1000, fs = 45000, muestra;
	register int n;

	for(int n = 0; n < MUESTRAS; n++)
	{
		muestra = sinf(2 * n * M_PI * f / fs);
		seno[n] = (short) (muestra * (1 << 11)); // Q0 ---- ADC de 12bits
	}
}

void guarda_datos(short	datos[], char *archivo)
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
	{
		fprintf(ap_arch, "%d \n", datos[n]);
	}

	fclose(ap_arch);
}