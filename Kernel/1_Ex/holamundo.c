// @Módulo de Kernel simple.

#include <linux/module.h>    	// Necesaria para todos los módulos
#include <linux/kernel.h>		// Necesaria para KERN_INFO

int init_module(void)
{
	printk(KERN_ALERT "Hola Mundo en el modulo 1.\n");

	// Un valor de retorno diferente de 0 significa que 
	// init_module fallo; el modulo no se puede cargar
	return 0;
}

void cleanup_module(void)
{
	printk(KERN_ALERT "Terminando la ejecución del módulo 1.\n");
}
