// promburbuja.c - Hilo de kernel que ejecuta la operación de promedio y ordenamiento burbuja sobre un arreglo, que es pasado por línea de comandos

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
MODULE_VERSION("1.0");
MODULE_INFO(driver, "PULSO CARDIACO");

static int opcion = 0;
static int arreglo[MAX] = {3, 2, 1, 7, 5, 6, 7, 8};
static int num_elem = MAX;

module_param(opcion, int, 0660);
module_param_array(arreglo, int, &num_elem, 0660);

MODULE_PARM_DESC(opcion, "Numero de opcion");
MODULE_PARM_DESC(arreglo, "Arreglo para trabajar");

struct task_struct *khilo; 
  
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
    // printk("\n");
} 

int hilo_kernel(void *data)
{
	int id = *(int *) data;
	int suma = 0, i;
	int promedio = 0;

	printk(KERN_INFO "Hilo en ejecución con id: %d...\n", id);
	while(!kthread_should_stop()) 	// Termina el hilo cuando kthread_stop es llamada
	{
		if(opcion == 1) // Promedio
		{
			printk(KERN_INFO "Obteniendo el promedio...\n");	

			suma = 0;
			for(i = 0; i < MAX; i++)
				suma += arreglo[i];

			promedio = suma >> 3;
			printk(KERN_INFO "Promedio: %d\n", promedio);	
			opcion = 0;
		}
		else if(opcion == 2) // Ordenamiento burbuja
		{
			printk(KERN_INFO "Haciendo el ordenamiento...\n");	
			// printk(KERN_INFO "Num elem: %d\n", num_elem);	
			
			bubbleSort(arreglo, num_elem);
			printk(KERN_INFO "Arreglo ordenado: \n");	
			printArray(arreglo, num_elem);
			// printk(KERN_INFO "Elem: %d\n", arreglo[7]);	
			
			opcion = 0;
		}
		msleep(100);
		schedule();				// Bloquea el hilo por un intervalo preestablecido de tiempo
	}

	return id;
}

static int __init kernel_init(void)
{
	static int id_thread = 1;

	printk(KERN_INFO "\n--------------------------------------------------------\n");
	khilo = kthread_run(hilo_kernel, (void *) &id_thread, "kmi_hilo");

	if(IS_ERR(khilo))
	{
		printk(KERN_ERR "Error en la creacion del hilo...\n");
		return PTR_ERR(khilo);
	}

	printk(KERN_INFO "Hilo de Kernel creado: %s con PID %d\n", khilo->comm, khilo->pid);

	return 0;
}

static void __exit kernel_exit(void)
{
	int ret_hilo_kernel = kthread_stop(khilo);

	if(ret_hilo_kernel == -EINTR)
		printk(KERN_INFO "Error en la terminacion del hilo.\n");
	else
		printk(KERN_INFO "Hilo de Kernel con id: %d detenido.\n", ret_hilo_kernel);
}

module_init(kernel_init);
module_exit(kernel_exit);
