// ddc-4.c - Asignación dinámica de número mayor y menor de un device driver de caracter
//           Creación de la clase para el device driver de caracter, creación del archivo de dispositivo para el ddc.			 
//			 Se agregan operaciones call backs
#include <linux/module.h>    	// Necesaria para todos los módulos
#include <linux/kernel.h>		// Necesaria para KERN_INFO
#include <linux/init.h>			// Necesaria para las macros
#include <linux/moduleparam.h>  // Necesaria para el manejo de parámetros
#include <linux/fs.h>			// Necesaria para las funciones de registro de números de dev
#include <linux/kdev_t.h>		// Necesaria para las macros de registro de números
#include <linux/device.h>		// Necesaria para la creación de clase y dispositivo
#include <linux/cdev.h>			// Necesaria para registrar las operaciones

MODULE_LICENSE("GPL");								// Tipo de licencia
MODULE_AUTHOR("JAIME BASTIDA");						// Autor del módulo
MODULE_DESCRIPTION("Creacion de Device Driver de Caracter");		// Descripción de la funcionalidad del módulo
MODULE_VERSION("1.0");								// Versión del módulo
MODULE_INFO(driver, "Character Device Driver");      // Información personalizada del usuario

dev_t dispositivo = 0;
static struct class *dev_class;
static struct device *dev_file;
static struct cdev dev_cdev;

static int driver_open(struct inode *inode, struct file *file);
static int driver_release(struct inode *inode, struct file *file);
static ssize_t driver_read(struct file *filp, char __user *buf, size_t len, loff_t *off);
static ssize_t driver_write(struct file *filp, const char *buf, size_t len, loff_t *off);

static struct file_operations fops =
{
	.owner = THIS_MODULE,
	.open = driver_open,
	.read = driver_read,
	.write = driver_write,
	.release = driver_release,
};

static int driver_open(struct inode *inode, struct file *file)
{
	printk(KERN_INFO "Llamada a la operacion open del DDC");	

	return 0;
}

static int driver_release(struct inode *inode, struct file *file)
{
	printk(KERN_INFO "Llamada a la operacion release del DDC");	

	return 0;
}

static ssize_t driver_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
	printk(KERN_INFO "Llamada a la operacion read del DDC");	

	return 0;	
}

static ssize_t driver_write(struct file *filp, const char *buf, size_t len, loff_t *off)
{
	printk(KERN_INFO "Llamada a la operacion write del DDC");	

	return len;		
}

static int __init funcion_inicio(void)
{
	int ret;

	printk(KERN_INFO "----------------------------------------");
	printk(KERN_INFO "Iniciando el modulo para asignar numeros dinamicamente en DDC.\n");
	ret = alloc_chrdev_region(&dispositivo, 0, 1, "ESCOM_dev");	

	if(ret < 0)
	{
		printk(KERN_ERR "Error al asignar el numero mayor y menor.\n");
		return ret;
	}
	printk(KERN_INFO "Numeros asignados correctamente, Mayor: %d, Menor: %d.\n", MAJOR(dispositivo), MINOR(dispositivo));

	cdev_init(&dev_cdev, &fops);
	ret = cdev_add(&dev_cdev, dispositivo, 1);
	
	if(ret < 0)
	{
		printk(KERN_ERR "Error al registrar las operaciones.\n");
		unregister_chrdev_region(dispositivo, 1);	
		return ret;
	}	
	printk(KERN_INFO "Operaciones registradas correctamente.\n");

	dev_class = class_create(THIS_MODULE, "ESCOM_class");
	if(IS_ERR(dev_class))
	{
		printk(KERN_ERR "Error al crear la clase de dispositivo.\n");
		unregister_chrdev_region(dispositivo, 1);	
		return PTR_ERR(dev_class);
	}
	printk(KERN_INFO "Clase de dispositivo creada exitosamente.\n");

	dev_file = device_create(dev_class, NULL, dispositivo, NULL, "ESCOM_device");
	if(IS_ERR(dev_file))
	{
		printk(KERN_ERR "Error al crear el archivo de dispositivo.\n");
		unregister_chrdev_region(dispositivo, 1);
		class_destroy(dev_class);

		return PTR_ERR(dev_file);
	}
	printk(KERN_INFO "Archivo de dispositivo creado exitosamente.\n");

	return 0;
}

static void __exit funcion_exit(void)
{
	printk(KERN_INFO "----------------------------------------");
	printk(KERN_INFO "Terminando la ejecución del modulo.\n");

	device_destroy(dev_class, dispositivo);
	class_destroy(dev_class);
	cdev_del(&dev_cdev);
	unregister_chrdev_region(dispositivo, 1);	
}

module_init(funcion_inicio);
module_exit(funcion_exit);
