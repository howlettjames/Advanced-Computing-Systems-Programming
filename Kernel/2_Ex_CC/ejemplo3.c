// ejemplo3.c - Demostración de la documentación de un módulo de kernel
// 				Estas macros se prefieren sobre las funciones init_module() y cleanup_module().

#include <linux/module.h>    	// Necesaria para todos los módulos
#include <linux/kernel.h>		// Necesaria para KERN_INFO
#include <linux/init.h>			// Necesaria para las macros

MODULE_LICENSE("GPL");								// Tipo de licencia
MODULE_AUTHOR("JAMESWILSON");						// Autor del módulo
MODULE_DESCRIPTION("Modulo de kernel simple");		// Descripción de la funcionalidad del módulo

static int __init funcion_inicio(void)
{
	printk(KERN_INFO "Hola Mundo en el modulo 3.\n");

	return 0;
}

static void __exit funcion_exit(void)
{
	printk(KERN_INFO "Terminando la ejecución del módulo 3.\n");
}

module_init(funcion_inicio);
module_exit(funcion_exit);
