// Este programa muestra el uso del device driver de caracter
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <math.h>

#define MUESTRAS 4096

void genera_seno(int seno[]);
void guarda_datos(int datos[], char *archivo);

int main(int argc, char const *argv[])
{
	int fd, len;
	int seno[MUESTRAS], seno_proc[MUESTRAS];

	fd = open("/dev/ESCOM_device", O_RDWR);
	if(fd == -1)
	{
		perror("Error al abrir el dispositivo");

		exit(EXIT_FAILURE);
	}

	genera_seno(seno);
	guarda_datos(seno, "seno.dat");

	len = write(fd, seno, MUESTRAS * sizeof(int));
	printf("Bytes enviados: %d\n", len);
	sleep(1);

	len = read(fd, seno_proc, MUESTRAS * sizeof(int));
	printf("Bytes recibidos: %d\n", len);

	guarda_datos(seno_proc, "seno_proc.dat");
	
	close(fd);

	return 0;
}

void genera_seno(int seno[])
{
	float f = 1.3, fs = 512, muestra;
	register int n;

	for(int n = 0; n < MUESTRAS; n++)
	{
		muestra = sinf(2 * n * M_PI * f / fs);
		seno[n] = (int) (muestra * 1024);
	}
}

void guarda_datos(int datos[], char *archivo)
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