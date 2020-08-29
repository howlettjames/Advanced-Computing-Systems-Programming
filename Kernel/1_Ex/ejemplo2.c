// ejemplo2.c - Demostración de las macros module_init() y module_exit().
// 				Estas macros se prefieren sobre las funciones init_module() y cleanup_module().

#include <linux/module.h>    	// Necesaria para todos los módulos
#include <linux/kernel.h>		// Necesaria para KERN_INFO
#include <linux/init.h>			// Necesaria para las macros

static int __init funcion_inicio(void)
{
	printk(KERN_INFO "Hola Mundo en el modulo 2.\n");

	// Un valor de retorno diferente de 0 significa que 
	// init_module fallo; el modulo no se puede cargar
	return 0;
}

static void __exit funcion_exit(void)
{
	printk(KERN_INFO "Terminando la ejecución del módulo 2.\n");
}

module_init(funcion_inicio);
module_exit(funcion_exit);
