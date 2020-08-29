// cb_param1.c -   Demostración del uso de parámetros con funciones callback.

#include <linux/module.h>    	// Necesaria para todos los módulos
#include <linux/kernel.h>		// Necesaria para KERN_INFO
#include <linux/init.h>			// Necesaria para las macros
#include <linux/moduleparam.h>  // Necesaria para el manejo de parámetros

MODULE_LICENSE("GPL");								// Tipo de licencia
MODULE_AUTHOR("JAMESWILSON");						// Autor del módulo
MODULE_DESCRIPTION("Uso de parametros con funciones callback");		// Descripción de la funcionalidad del módulo
MODULE_VERSION("1.0");
MODULE_INFO(driver, "PULSO CARDIACO");

static int irq = 7;

int set_param(const char *val, const struct kernel_param *kp)
{
	int parametro = *(int *) kp->arg;
	int ret;

	printk(KERN_INFO "Funcion callback de escritura ejecutada...\n");
	printk(KERN_INFO "Parametro val: %s\n", val);
	printk(KERN_INFO "Parametro kp->arg: %d, irq: %d\n", parametro, irq);
	
	// Convierte de cadena a entero, 0 en éxito
	ret = param_set_int(val, kp);

	if(ret == 0)
	{
		printk(KERN_INFO "Parametro irq: %d\n", irq);
	}

	return ret;
}

int get_param(char *buffer, const struct kernel_param *kp)
{
	int ret = 0;

	printk(KERN_INFO "Funcion callback de lectura ejecutada...\n");
	printk(KERN_INFO "Buffer antes de conversion: %s, ret: %d\n", buffer, ret);

	// Esta función convierte de entero a cadena, retorna la longitud de la cadena que se copió
	ret = param_get_int(buffer, kp);

	if(ret > 1)
	{
		printk(KERN_INFO "Buffer despues de conversion: %s, ret: %d\n", buffer, ret);
		return ret;
	}

	return -EPERM;
}

const struct kernel_param_ops mis_param_ops = 
{
	.set = set_param,
	.get = get_param,
};

module_param_cb(irq, &mis_param_ops, &irq, 0660);
MODULE_PARM_DESC(irq, "Numero de interrupcion");


static int __init funcion_inicio(void)
{
	printk("--------------------------------------------");
	printk(KERN_INFO "Iniciando el modulo con funciones callback 1.\n");
	printk(KERN_INFO "Interrupcion: %d\n", irq);
	
	return 0;
}

static void __exit funcion_exit(void)
{
	printk("--------------------------------------------");
	printk(KERN_INFO "Terminando la ejecución del modulo con funciones callback 1.\n");
	printk(KERN_INFO "Interrupcion: %d\n", irq);
}

module_init(funcion_inicio);
module_exit(funcion_exit);
