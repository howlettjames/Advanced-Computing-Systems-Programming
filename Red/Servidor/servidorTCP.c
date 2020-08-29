#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "imagen.h"

#define PUERTO 			5000	//Número de puerto asignado al servidor
#define COLA_CLIENTES 	5 		//Tamaño de la cola de espera para clientes
#define TAM_BUFFER 		100

unsigned char *imagenRGB, *imagenGray;

unsigned char *reservarMemoria(int nBytes);
void RGBToGray_v2(unsigned char *imagenRGB, unsigned char *imagenGray, uint32_t width, uint32_t height);
void GrayToRGB_v2(unsigned char *imagenRGB, unsigned char *imagenGray, uint32_t width, uint32_t height);
void recibirImagen(int sockfd, unsigned char *imagenGray, int longitud);

int main(int argc, char **argv)
{
  int sockfd, cliente_sockfd;
	struct sockaddr_in direccion_servidor; //Estructura de la familia AF_INET, que almacena direccion
	// char leer_mensaje[TAM_BUFFER];
  bmpInfoHeader info;
 
 /*	
  *	AF_INET - Protocolo de internet IPV4
  *  htons - El ordenamiento de bytes en la red es siempre big-endian, por lo que
  *  en arquitecturas little-endian se deben revertir los bytes
  *  INADDR_ANY - Se elige cualquier interfaz de entrada
  */	
  memset (&direccion_servidor, 0, sizeof (direccion_servidor));	//se limpia la estructura con ceros
	direccion_servidor.sin_family 		= AF_INET;
	direccion_servidor.sin_port 		= htons(PUERTO);
	direccion_servidor.sin_addr.s_addr 	= INADDR_ANY;

  /*
  *	Creacion de las estructuras necesarias para el manejo de un socket
  *  SOCK_STREAM - Protocolo orientado a conexión
  */
	printf("Creando Socket ....\n");
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
		perror("Ocurrio un problema en la creacion del socket");
		exit(1);
	}
  
  /*
  *  bind - Se utiliza para unir un socket con una dirección de red determinada
  */
	printf("Configurando socket ...\n");
	if(bind(sockfd, (struct sockaddr *) &direccion_servidor, sizeof(direccion_servidor)) < 0)
	{
   	perror ("Ocurrio un problema al configurar el socket");
   	exit(1);
	}
  
  /*
  *  listen - Marca al socket indicado por sockfd como un socket pasivo, esto es, como un socket
  *  que será usado para aceptar solicitudes de conexiones de entrada usando "accept"
  *  Habilita una cola asociada la socket para alojar peticiones de conector procedentes
  *  de los procesos clientes
  */
	printf ("Estableciendo la aceptacion de clientes...\n");
	if(listen(sockfd, COLA_CLIENTES) < 0)
	{
		perror("Ocurrio un problema al crear la cola de aceptar peticiones de los clientes");
		exit(1);
	}
  
  /*
  *  accept - Aceptación de una conexión
  *  Selecciona un cliente de la cola de conexiones establecidas
  *  se crea un nuevo descriptor de socket para el manejo
  *  de la nueva conexion. Apartir de este punto se debe
  *  utilizar el nuevo descriptor de socket
  *  accept - ES BLOQUEANTE 
  */
	printf ("En espera de peticiones de conexión ...\n");
	if((cliente_sockfd = accept(sockfd, NULL, NULL)) < 0)
	{
		perror("Ocurrio algun problema al atender a un cliente");
		exit(1);
	}

  /*
  *	Inicia la transferencia de datos entre cliente y servidor
  */
	if(read(cliente_sockfd, &info, sizeof(bmpInfoHeader)) < 0)
	{
		perror ("Ocurrio algun problema al recibir datos del cliente");
		exit(1);
	}

  printf("Se aceptó un cliente, recibiendo informacion de la imagen\n");
  imagenGray = reservarMemoria(info.width * info.height);
  recibirImagen(cliente_sockfd, imagenGray, info.width * info.height);

  imagenRGB = reservarMemoria(info.width * info.height * 3);
  GrayToRGB_v2(imagenRGB, imagenGray, info.width, info.height); 
  guardarBMP("recv.bmp", &info, imagenRGB);  

	if(write(cliente_sockfd, "Bienvenido cliente", 19) < 0)
	{
		perror("Ocurrio un problema en el envio de un mensaje al cliente");
		exit(1);
	}
	printf("Concluimos la ejecución de la aplicacion Servidor \n");

  /*
  *	Cierre de las conexiones
  */
	close (sockfd);
	close (cliente_sockfd);
  free(imagenRGB);
  free(imagenGray);

	return 0;
}

void recibirImagen(int sockfd, unsigned char *imagenGray, int longitud)
{
   int bytesFaltantes = longitud, bytesRcv;

   while(bytesFaltantes > 0)
   {
      bytesRcv = read(sockfd, imagenGray, bytesFaltantes);
      if(bytesRcv < 0)
      {
         perror ("Ocurrio algun problema al recibir datos del cliente");
         exit(1);
      }

      printf("Bytes leidos: %d\n", bytesRcv);
      bytesFaltantes -= bytesRcv;  
      imagenGray = imagenGray + bytesRcv;
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

unsigned char *reservarMemoria(int nBytes)
{
   unsigned char *imagen;
   imagen = (unsigned char *) malloc(nBytes);

   if(imagen == NULL)
   {
      perror("Error: No se pudo asignar memoria");
      exit(EXIT_FAILURE);
   }

   return imagen;
}