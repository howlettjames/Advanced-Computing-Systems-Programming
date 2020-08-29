/** @brief Este programa multiplica dos vectores Pi = Ai * Bi
 *  de forma secuencial
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <pthread.h>

#define N 64	//Bloque de datos
#define NUM_HILOS 4

int *A, *B, *P;

int *reservar_memoria(void);
void llenar_arreglo(int *datos);
void imprimir(int *datos);
void producto(void);
void *funcion_hilo(void *nh);

int main()
{
	pthread_t tids[NUM_HILOS];
	int nhs[NUM_HILOS];
	int *hilo;
	register int nh;

	srand(getpid());

	A = reservar_memoria();
	llenar_arreglo(A);
	imprimir(A);

	B = reservar_memoria();
	llenar_arreglo(B);
	imprimir(B);

	P = reservar_memoria();

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
	
	// producto();
	printf("El producto P[i] = A[i] * B[i]\n");
	imprimir(P);

	free(A);
	free(B);
	free(P);

	return 0;
}

void *funcion_hilo(void *nh)
{
	int core = *(int *)nh;
	
	for(int i = core; i < N; i += NUM_HILOS)
	{
		P[i] = A[i] * B[i];
	}

	pthread_exit(nh);
}

/*void *funcion_hilo(void *nh)
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
*/
void producto(void)
{
	register int i;

	for(i = 0; i < N; i++)
		P[i] = A[i] * B[i];
}

void llenar_arreglo(int *datos)
{
	register int i;

	for(i = 0; i < N; i++)
		datos[i] = rand() % 256;
}

int *reservar_memoria(void)
{
	int *mem;

	mem = (int *) malloc( sizeof(int) * N);
	if(!mem)
	{
		perror("Error de asignación de memoria");
		exit(EXIT_FAILURE);
	}

	return mem;
}

void imprimir(int *datos)
{
	register int i = 0;

	for(i = 0; i < N; i++)
	{
		printf("%5d ", datos[i]);
		if(! ( (i + 1) % 16 ) )
			printf("\n");
	}
	printf("\n");
}


void guardaDatosArch(int s[], char *archivo)
{
	FILE *apFile;
	register int i;

	apFile = fopen(archivo,"w");
	if(apFile == NULL)
	{
		perror("Error al abrir el archivo");
		exit(1);
	}

	for(i = 0; i < N; i++)
	{
		//if( !(i % 16) )
		//	fputs(" \n", apFile);

		fprintf(apFile, "%d \n", s[i]);
	}
	fclose(apFile);
}


