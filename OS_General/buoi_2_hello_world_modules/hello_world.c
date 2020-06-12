#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>


int a1;

EXPORT_SYMBOL(a1);



int init_module(void)
{
	a1 = 10;
	printk(KERN_INFO "Hello world 1.\n");
	return 0;
}
void cleanup_module(void)
{
	printk(KERN_INFO "Goodbye world 1.\n");
}
