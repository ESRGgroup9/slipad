#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/slab.h>                 
#include <linux/uaccess.h>             
#include <linux/ioctl.h>   
#include <linux/sched/signal.h> 
#include <linux/proc_fs.h> 
#include <linux/fcntl.h> 
#include <linux/types.h>
#include <linux/gpio.h> 
#include <linux/interrupt.h>
#include "utils.h"

#define DEVICE_NAME "lampf"
#define CLASS_NAME "lampfClass"
#define REG_CURRENT_TASK _IOW('a','a',int32_t*)

#define IOCTL_PID 1
#define GPIO_INPUT 0

MODULE_LICENSE("GPL");

static pid_t pid;

static dev_t dev;
static struct class *dev_class = NULL;
static struct cdev c_dev;  // Character device structure

struct GpioRegisters *s_pGpioRegisters;

static unsigned int pinNum = 26;

static int __init lampf_driver_init(void);
static void __exit lampf_driver_exit(void);
static int lampf_open(struct inode *inode, struct file *file);
static int lampf_close(struct inode *inode, struct file *file);
static ssize_t lampf_read(struct file *filp, char __user *buf, size_t len, loff_t *off);
static ssize_t lampf_write(struct file *filp, const char *buf, size_t len, loff_t *off);
static long lampf_ioctl(struct file *file, unsigned int cmd, unsigned long arg);

static int lampf_open(struct inode* inode, struct file *file)
{
	printk(KERN_INFO "[LampF] Device File Opened\n");
	return 0;
}

static int lampf_close(struct inode *inode, struct file * file)
{
	printk(KERN_INFO "[LampF] Device File Closed\n");
	return 0;
}

static ssize_t lampf_write(struct file *filp, const char __user *buf, size_t len, loff_t *off) 
{	
   	printk(KERN_INFO "[LampF] Write function\n");
   	return 0;
}

static ssize_t lampf_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
	char buffer[2];
	int i = gpio_get_value(pinNum);
	int ret = 0;

	sprintf(buffer, "%d", i);

	ret = copy_to_user(buf, buffer, 1);
	printk(KERN_INFO "[LampF] PIN -> %d\n", i);

	// returns number of bytes that cannot be read
	// in success it must be zero
	return ret;
}

static long lampf_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{	
	if(cmd == IOCTL_PID)
	{
		if(copy_from_user(&pid, (pid_t*)arg, sizeof(pid_t))) 
		{
			printk(KERN_INFO "[LampF] copy_from_user failed\n"); 
			return -1;
		}
		// printk(KERN_INFO "[LampF] PID-> %d\n", pid);
		printk(KERN_INFO "[LampF] Requested by PID %d\n", pid);
	}
	else
	{
		printk(KERN_INFO "[LampF] ioctl failed\n");	
	}
	return 0;
}

static struct file_operations fops = 
{
	.owner = THIS_MODULE,
	.write = lampf_write,
	.read = lampf_read,
	.release = lampf_close,
	.open = lampf_open,
	.unlocked_ioctl = lampf_ioctl,
};

static int __init lampf_driver_init(void) 
{
	if ((alloc_chrdev_region(&dev, 0, 1, DEVICE_NAME)) < 0) 
	{
		printk(KERN_INFO "[LampF] Cannot allocate major number\n");
	    return -1;
	}

	/*Creating cdev structure*/
	cdev_init(&c_dev, &fops);

	/*Adding character device to the system*/
	if((cdev_add(&c_dev,dev,1)) < 0)
	{
		printk(KERN_INFO "[LampF] Cannot add the device to the system\n");
		unregister_chrdev_region(dev,1);
	}	

	/*Creating struct class*/
	if((dev_class = class_create(THIS_MODULE, CLASS_NAME)) == NULL)
	{
		printk(KERN_INFO "[LampF] Cannot create the struct class\n");
		unregister_chrdev_region(dev,1);
	}

	/*Creating device*/
	if((device_create(dev_class, NULL, dev, NULL, DEVICE_NAME)) == NULL)
	{
		printk(KERN_INFO "[LampF] Cannot create the Device\n");
		class_destroy(dev_class);
		unregister_chrdev_region(dev,1);
	}
    
    s_pGpioRegisters = (struct GpioRegisters *)ioremap(GPIO_BASE, sizeof(struct GpioRegisters));
	// s_pGpioRegisters = (struct GpioRegisters *)ioremap_nocache(GPIO_BASE, sizeof(struct GpioRegisters));
	SetGPIOFunction(s_pGpioRegisters, pinNum, GPIO_INPUT);

	return 0;
}

static void __exit lampf_driver_exit(void) 
{
	SetGPIOFunction(s_pGpioRegisters, pinNum, GPIO_INPUT);
	iounmap(s_pGpioRegisters);
	
	device_destroy(dev_class, dev);
	class_destroy(dev_class);
	cdev_del(&c_dev);
	unregister_chrdev_region(dev, 1);
	printk(KERN_INFO "[LampF] Device Driver Remove\n");
}

module_init(lampf_driver_init);
module_exit(lampf_driver_exit);
