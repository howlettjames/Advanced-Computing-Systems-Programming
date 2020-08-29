// ddc-2.c - Asignación dinámica de número mayor y menor de un device driver de caracter

#include <linux/module.h>    	// Necesaria para todos los módulos
#include <linux/kernel.h>		// Necesaria para KERN_INFO
#include <linux/init.h>			// Necesaria para las macros
#include <linux/moduleparam.h>  // Necesaria para el manejo de parámetros
#include <linux/fs.h>			// Necesaria para las funciones de registro de números de dev
#include <linux/kdev_t.h>		// Necesaria para las macros de registro de números

MODULE_LICENSE("GPL");								// Tipo de licencia
MODULE_AUTHOR("JAIME BASTIDA");						// Autor del módulo
MODULE_DESCRIPTION("Asignacion de numeros en DDC");		// Descripción de la funcionalidad del módulo
MODULE_VERSION("1.0");								// Versión del módulo
MODULE_INFO(driver, "Character Device Driver");      // Información personalizada del usuario

dev_t dispositivo = 0;

static int __init funcion_inicio(void)
{
	int ret;

	printk(KERN_INFO "----------------------------------------");
	printk(KERN_INFO "Iniciando el modulo para asignar numeros dinamicamente en DDC.\n");
	ret = alloc_chrdev_region(&dispositivo, 0, 1, "ESCOM_DDC");	

	if(ret < 0)
	{
		printk(KERN_ERR "Error al asignar el numero mayor y menor.\n");
		return ret;
	}

	printk(KERN_INFO "Numeros asignados correctamente, Mayor: %d, Menor: %d.\n", MAJOR(dispositivo), MINOR(dispositivo));

	return 0;
}

static void __exit funcion_exit(void)
{
	printk(KERN_INFO "----------------------------------------");
	printk(KERN_INFO "Terminando la ejecución del modulo para asignar numeros estaticos en DDC.\n");

	unregister_chrdev_region(dispositivo, 1);	

}

module_init(funcion_inicio);
module_exit(funcion_exit);
