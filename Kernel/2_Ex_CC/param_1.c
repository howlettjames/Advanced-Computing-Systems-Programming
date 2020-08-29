// param1.c -   Demostración del uso de parámetros.

#include <linux/module.h>    	// Necesaria para todos los módulos
#include <linux/kernel.h>		// Necesaria para KERN_INFO
#include <linux/init.h>			// Necesaria para las macros
#include <linux/moduleparam.h>  // Necesaria para el manejo de parámetros

#define MAX 4

MODULE_LICENSE("GPL");								// Tipo de licencia
MODULE_AUTHOR("JAMESWILSON");						// Autor del módulo
MODULE_DESCRIPTION("Uso de parametros");		// Descripción de la funcionalidad del módulo
MODULE_VERSION("1.0");
MODULE_INFO(driver, "PULSO CARDIACO");

static int irq = 7;
static char *comando = "derecha";
static int datos[MAX] = {1, 2, 3, 4};
static int num_elem = MAX;

module_param(irq, int, 0660);
module_param(comando, charp, 0660);
module_param_array(datos, int, &num_elem, 0660);

MODULE_PARM_DESC(irq, "Numero de interrupcion");
MODULE_PARM_DESC(comando, "Comando del modulo");
MODULE_PARM_DESC(datos, "Muestras de pulso cardiaco");

static int __init funcion_inicio(void)
{
	register int i;

	printk(KERN_INFO "Iniciando el modulo con parametros 1.\n");
	printk(KERN_INFO "Interrupcion: %d, Comando: %s.\n", irq, comando);
	for(i = 0; i < num_elem; i++)
	{
		printk(KERN_INFO "Datos[%d] = %d.\n", i, datos[i]);	
	}	

	return 0;
}

static void __exit funcion_exit(void)
{
	register int i;

	printk(KERN_INFO "Terminando la ejecución del módulo parametros con parametros 1.\n");
	printk(KERN_INFO "Interrupcion: %d, Comando: %s.\n", irq, comando);
	for(i = 0; i < num_elem; i++)
	{
		printk(KERN_INFO "Datos[%d] = %d.\n", i, datos[i]);	
	}
}

module_init(funcion_inicio);
module_exit(funcion_exit);
