// ejemplo_hilo.c - Demostración de uso de hilos en kernel.

#include <linux/module.h>    	// Necesaria para todos los módulos
#include <linux/kernel.h>		// Necesaria para KERN_INFO
#include <linux/init.h>			// Necesaria para las macros
#include <linux/kthread.h>      // Necesaria para los hilos
#include <linux/delay.h>        // Necesaria para las rutinas de tiempo
#include <linux/sched.h>		// Necesaria para la función schedule()

MODULE_LICENSE("GPL");								// Tipo de licencia
MODULE_AUTHOR("JAMESWILSON");						// Autor del módulo
MODULE_DESCRIPTION("Modulo de kernel simple");		// Descripción de la funcionalidad del módulo
MODULE_VERSION("1.0");
MODULE_INFO(driver, "PULSO CARDIACO");

struct task_struct *khilo;

int hilo_kernel(void *data)
{
	int id = *(int *) data;
	register int contador = 0;

	printk(KERN_INFO "Hilo en ejecución con id: %d...\n", id);
	while(!kthread_should_stop()) 	// Termina el hilo cuando kthread_stop es llamada
	{
		printk(KERN_INFO "Ejecutando codigo de hilo: %d...\n", contador++);	
		// schedule();				// Bloquea el hilo por un intervalo preestablecido de tiempo
		ssleep(1); 				// Bloquea el hilo por un número de segundos = 2
		// msleep(500);
		// udelay(1000);	
	}

	return id;
}

static int __init kernel_init(void)
{
	static int id_thread = 10;

	printk(KERN_INFO "\n--------------------------------------------------------\n");
	khilo = kthread_create(hilo_kernel, (void *) &id_thread, "kmi_hilo");
	// khilo = kthread_run(hilo_kernel, (void *) &id_thread, "kmi_hilo");

	if(IS_ERR(khilo))
	{
		printk(KERN_ERR "Error en la creacion del hilo...\n");
		return PTR_ERR(khilo);
	}

	wake_up_process(khilo);
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
