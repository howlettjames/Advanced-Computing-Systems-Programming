#include <stdio.h>

#define Qn 8
#define M 3

int main(int argc, char const *argv[])
{	
	register int i;
	float cf[M] = { 0.5, 9.53125, 4.140625 };
	int ce[M], y, x[M] = { 23, 7 , 11 };

	for(i = 0; i < M; i++)
	{
		ce[i] = (int) (cf[i] * (1 << Qn));	
		printf("Formato Q8, ce%d: %d\n", i, ce[i]);
	}

	for(i = 0; i < M; i++)
		y += ce[i] * x[i];

	printf("El resultado Q8 es: %d\n", y);

	y = y >> Qn;

	printf("El resultado Q0 es: %d\n", y);

	return 0;
}