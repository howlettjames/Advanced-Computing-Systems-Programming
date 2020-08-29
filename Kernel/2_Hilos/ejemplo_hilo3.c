// ejemplo_hilo3.c - Hilo de kernel con parámetros.

#include <linux/module.h>    	// Necesaria para todos los módulos
#include <linux/kernel.h>		// Necesaria para KERN_INFO
#include <linux/init.h>			// Necesaria para las macros
#include <linux/kthread.h>      // Necesaria para los hilos
#include <linux/delay.h>        // Necesaria para las rutinas de tiempo
#include <linux/sched.h>		// Necesaria para la función schedule()
#include <linux/moduleparam.h>		// Necesaria para los parámetros

MODULE_LICENSE("GPL");								// Tipo de licencia
MODULE_AUTHOR("JAMESWILSON");						// Autor del módulo
MODULE_DESCRIPTION("Modulo de kernel simple");		// Descripción de la funcionalidad del módulo
MODULE_VERSION("1.0");
MODULE_INFO(driver, "PULSO CARDIACO");

static int irq = 7;
module_param(irq, int, 0660);
MODULE_PARM_DESC(irq, "Numero de interrupcion");

struct task_struct *khilo1, *khilo2;

int hilo_kernel(void *data)
{
	int id = *(int *) data;
	register int contador = 0, cpu;

	printk(KERN_INFO "Hilo en ejecución con id: %d...\n", id);
	while(!kthread_should_stop()) 	// Termina el hilo cuando kthread_stop es llamada
	{
		cpu = get_cpu();
		put_cpu();
		// schedule();					// Bloquea el hilo por un intervalo preestablecido de tiempo
		ssleep(1); 				// Bloquea el hilo por un número de segundos = 2
		// msleep(500);
		// udelay(1000);	
		printk(KERN_INFO "Ejecutando codigo de hilo: %d, cpu: %d...\n", contador++, cpu);	
	}

	return id;
}

static int __init kernel_init(void)
{
	static int id_thread1 = 10, id_thread2 = 20;

	printk(KERN_INFO "\n--------------------------------------------------------\n");
	khilo1 = kthread_create(hilo_kernel, (void *) &id_thread1, "kmi_hilo1");
	if(IS_ERR(khilo1))
	{
		printk(KERN_ERR "Error en la creacion del hilo...\n");
		return PTR_ERR(khilo1);
	}
	kthread_bind(khilo1, 3);			// Vinculamos nuestro hilo al CPU 3
	wake_up_process(khilo1);			// Iniciando el hilo, poniéndolo en la cola de listos para ejecución
	printk(KERN_INFO "Hilo de Kernel creado: %s con PID %d\n", khilo1->comm, khilo1->pid);

	printk(KERN_INFO "\n--------------------------------------------------------\n");
	khilo2 = kthread_create(hilo_kernel, (void *) &id_thread2, "kmi_hilo2");
	if(IS_ERR(khilo2))
	{
		printk(KERN_ERR "Error en la creacion del hilo...\n");
		return PTR_ERR(khilo2);
	}
	kthread_bind(khilo2, 1);			// Vinculamos nuestro hilo al CPU 1
	wake_up_process(khilo2);			// Iniciando el hilo, poniéndolo en la cola de listos para ejecución
	printk(KERN_INFO "Hilo de Kernel creado: %s con PID %d\n", khilo2->comm, khilo2->pid);

	return 0;
}

static void __exit kernel_exit(void)
{
	int ret_hilo_kernel1 = kthread_stop(khilo1);
	int ret_hilo_kernel2 = kthread_stop(khilo2);
	
	if(ret_hilo_kernel1 == -EINTR)
		printk(KERN_INFO "Error en la terminacion del hilo.\n");
	else
		printk(KERN_INFO "Hilo de Kernel con id: %d detenido.\n", ret_hilo_kernel1);

	if(ret_hilo_kernel2 == -EINTR)
		printk(KERN_INFO "Error en la terminacion del hilo.\n");
	else
		printk(KERN_INFO "Hilo de Kernel con id: %d detenido.\n", ret_hilo_kernel2);
}

module_init(kernel_init);
module_exit(kernel_exit);
