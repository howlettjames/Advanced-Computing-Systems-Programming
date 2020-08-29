// Este programa muestra el uso del device driver de caracter
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define N 1024

char buffer[N];
char buffer_read[N];

int main(int argc, char const *argv[])
{
	register int i;
	int fd;
	unsigned char dato;

	fd = open("/dev/ESCOM_device", O_RDWR);

	if (fd == -1)
	{
		perror("Error al abrir el dispositivo");

		exit(EXIT_FAILURE);
	}

	printf("Dame el mensaje: ");
	scanf("%s", buffer);
	write(fd, buffer, strlen(buffer) + 1);
	sleep(5);
	read(fd, buffer_read, N);
	printf("Mensaje recibido del DDC: %s\n", buffer_read);
	sleep(5);
	
	close(fd);

	return 0;
}