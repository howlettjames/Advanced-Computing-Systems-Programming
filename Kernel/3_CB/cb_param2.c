// cb_param_2.c -   Demostración del uso de parámetros con call back usando una cadena.

#include <linux/module.h>    	// Necesaria para todos los módulos
#include <linux/kernel.h>		// Necesaria para KERN_INFO
#include <linux/init.h>			// Necesaria para las macros
#include <linux/moduleparam.h>  // Necesaria para el manejo de parámetros

#define N 6

MODULE_LICENSE("GPL");								// Tipo de licencia
MODULE_AUTHOR("JAMESWILSON");						// Autor del módulo
MODULE_DESCRIPTION("Uso de parametros");		// Descripción de la funcionalidad del módulo
MODULE_VERSION("1.0");
MODULE_INFO(driver, "PULSO CARDIACO");

static char comando[N];

int set_param(const char *val, const struct kernel_param *kp)
{
	char *parametro = (char *) kp->arg;
	int lon = strlen(val);

	printk(KERN_INFO "Funcion callback de escritura ejecutada...\n");
	printk(KERN_INFO "Parametro val: %s y longitud: %d\n", val, lon);
	printk(KERN_INFO "Parametro kp->arg: %s, comando: %s\n", parametro, comando);
	
	if(lon > N)	
	{
		printk(KERN_ERR "Parametro: (%s) muy largo\n", val);
		return -ENOSPC;
	}
	else if(lon == 1)
	{
		printk(KERN_ERR "Falta parametro\n");
		return -EINVAL;
	}
	
	strcpy(parametro, val);
	printk(KERN_INFO "Parametro kp->arg: %s, comando: %s\n", parametro, comando);

	return 0;
}

// Versión 2, con una cadena
int get_param(char *buffer, const struct kernel_param *kp)
{
	char *cadena = (char *) (kp->arg);
	int ret;

	printk(KERN_INFO "Funcion callback de lectura ejecutada...\n");
	
	strcpy(buffer, cadena);	
	ret = strlen(buffer);

	printk(KERN_INFO "Valor de retorno: %s...\n", buffer);
	
	return ret;
}

// Versión 1, con un número
/*int get_param(char *buffer, const struct kernel_param *kp)
{
	int ret = 0;

	printk(KERN_INFO "Funcion callback de lectura ejecutada...\n");
	printk(KERN_INFO "Buffer antes de conversion: %s, ret: %d\n", buffer, ret);

	// Esta función convierte de entero a cadena, retorna la longitud de la cadena que se copió
	ret = param_get_int(buffer, kp);

	if(ret > 1)
	{
		printk(KERN_INFO "Buffer despues de conversion: %s, ret: %d\n", buffer, ret);
		return ret;	// Debemos devolver el tamaño de la cadena que se copió
	}

	return -EPERM;
}
*/
const struct kernel_param_ops mis_param_ops = 
{
	.set = set_param,
	.get = get_param,
};

module_param_cb(comando, &mis_param_ops, &comando, 0660);
MODULE_PARM_DESC(comando, "Cadena de parametro");


static int __init funcion_inicio(void)
{
	strcpy(comando, "atras");
	printk(KERN_INFO "----------------------------------------");
	printk(KERN_INFO "Iniciando el modulo con funciones callback.\n");
	printk(KERN_INFO "Comando: %s\n", comando);
	
	return 0;
}

static void __exit funcion_exit(void)
{
	printk(KERN_INFO "----------------------------------------");
	printk(KERN_INFO "Terminando la ejecución del modulo con funciones callback.\n");
	printk(KERN_INFO "Comando: %s\n", comando);
	
}

module_init(funcion_inicio);
module_exit(funcion_exit);
