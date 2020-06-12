#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

extern  int a1;
int init_module(void)
{
	printk(KERN_INFO "Hello world 1 a1 = %d .\n",a1);
	return 0;
}
void cleanup_module(void)
{
	printk(KERN_INFO "Goodbye world 1.\n");
}
