#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("JAIME BASTIDA");
MODULE_DESCRIPTION("DEMOSTRACION MACROS DE INICIALIZACION");

static int mivar1 __initdata = 10;
static int mivar2 __initdata = 20;

static int __init funcion_init(void)
{
 	printk(KERN_INFO "Hola mundo en el modulo 4, var1: %d, var2: %d.\n", mivar1, mivar2);

	/*Un valor de retorno diferente de 0 significa que init_module falló*/
	return 0;
}

static void __exit funcion_exit(void)
{
	printk(KERN_INFO "Terminando la ejecucion del modulo 4.\n");
}

module_init(funcion_init);
module_exit(funcion_exit);