// promburbuja_cb.c - Programa que ejecuta la operación de promedio y ordenamiento burbuja sobre un arreglo, con funciones call back

#include <linux/module.h>    	// Necesaria para todos los módulos
#include <linux/kernel.h>		// Necesaria para KERN_INFO
#include <linux/init.h>			// Necesaria para las macros
#include <linux/kthread.h>      // Necesaria para los hilos
#include <linux/delay.h>        // Necesaria para las rutinas de tiempo
#include <linux/sched.h>		// Necesaria para la función schedule()
#include <linux/moduleparam.h>		// Necesaria para los parámetros

#define MAX 8

MODULE_LICENSE("GPL");								// Tipo de licencia
MODULE_AUTHOR("JAMESWILSON");						// Autor del módulo
MODULE_DESCRIPTION("Modulo de kernel que hace promedio y burbuja sobre un arreglo");		// Descripción de la funcionalidad del módulo
MODULE_VERSION("2.0");
MODULE_INFO(driver, "ROBOT");

// A function to implement bubble sort 
void bubbleSort(int arr[], int n) 
{ 
   int i, j, temp; 
   for (i = 0; i < n-1; i++)       
  
       // Last i elements are already in place    
       for (j = 0; j < n-i-1; j++)  
           if (arr[j] > arr[j+1])
           {
           		temp = arr[j];
           		arr[j] = arr[j + 1];
           		arr[j + 1] = temp;
           } 
} 
  
/* Function to print an array */
void printArray(int arr[], int size) 
{ 
    int i; 
    for (i=0; i < size; i++) 
        printk(KERN_INFO "arreglo[%d] = %d ", i, arr[i]); 
    printk("\n");
} 

static int opcion = 0;
static int promedio = 0;
static int arreglo[MAX] = {3, 2, 1, 7, 5, 6, 7, 8};
static int num_elem = MAX;

module_param_array(arreglo, int, &num_elem, 0660);
MODULE_PARM_DESC(arreglo, "Arreglo para trabajar");
  
int set_param(const char *val, const struct kernel_param *kp)
{
	int parametro = *(int *) kp->arg;
	int ret, suma = 0, i;

	printk(KERN_INFO "Funcion callback de escritura ejecutada...\n");
	printk(KERN_INFO "Parametro valor por el cual actualizar: %s\n", val);
	printk(KERN_INFO "Parametro kp->arg: %d\n", parametro);
	printk(KERN_INFO "Hola Dani\n");
	
	
	// Convierte de cadena a entero, 0 en éxito
	ret = param_set_int(val, kp);

	if(ret == 0)
	{
		printk(KERN_INFO "Conversion de cadena a entero exitosa\n");
		if(opcion == 1)	// PROMEDIO
		{
			printk(KERN_INFO "Obteniendo el promedio...\n");	

			suma = 0;
			for(i = 0; i < MAX; i++)
				suma += arreglo[i];

			promedio = suma >> 3;
			// printk(KERN_INFO "Promedio: %d\n", promedio);	
		}
		else if(opcion == 2) // ORDENAMIENTO
		{
			printk(KERN_INFO "Haciendo el ordenamiento...\n");	
			// printk(KERN_INFO "Num elem: %d\n", num_elem);	
			
			bubbleSort(arreglo, num_elem);
			printk(KERN_INFO "Arreglo ordenado: \n");	
			printArray(arreglo, num_elem);
			// printk(KERN_INFO "Elem: %d\n", arreglo[7]);	
		}
		else
		{
			return -EINVAL;
		}
	}

	return ret;
}

int get_param(char *buffer, const struct kernel_param *kp)
{
	int ret = 0;

	printk(KERN_INFO "Funcion callback de lectura ejecutada...\n");

	ret = sprintf(buffer, "%d\n", promedio);

	if(ret > 0)
	{
		// printk(KERN_INFO "Buffer despues de conversion: %s, ret: %d\n", buffer, ret);
		printk(KERN_INFO "Promedio: %d\n", promedio);
		return ++ret;
	}

	return -EPERM;
}

const struct kernel_param_ops mis_param_ops =
{
	.set = set_param,
	.get = get_param,
};

module_param_cb(opcion, &mis_param_ops, &opcion, 0660);
MODULE_PARM_DESC(opcion, "Opcion a ejecutar");

static int __init kernel_init(void)
{
	printk("--------------------------------------------");
	printk(KERN_INFO "Iniciando el modulo con funciones callback.\n");
	printk(KERN_INFO "Promedio: %d\n", promedio);
	
	return 0;
}

static void __exit kernel_exit(void)
{
	printk("--------------------------------------------");
	printk(KERN_INFO "Terminando la ejecución del modulo con funciones callback.\n");
}

module_init(kernel_init);
module_exit(kernel_exit);
