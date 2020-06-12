#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/ioctl.h>
#include <linux/kmod.h>
#include <linux/slab.h>
#include <linux/timer.h>
#include <linux/jiffies.h>
#include <asm/param.h>
#include <asm/gpio.h>
#include <asm/io.h>
#include <linux/err.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("NHT huynhtai291998@gmail.com");




#define GPIO1_START_ADDR        0x4804C000 
#define GPIO1_END_ADDR		0X4804DFFF
#define GPIO_SIZE              (GPIO1_END_ADDR - GPIO1_START_ADDR)
#define GPIO_REVISION          0x00
#define GPIO_SYSCONFIG         0x10
#define GPIO_EOI               0x20
#define GPIO_IRQSTATUS_RAW_0   0x24
#define GPIO_IRQSTATUS_RAW_1   0x28
#define GPIO_IRQSTATUS_0       0x2C
#define GPIO_IRQSTATUS_1       0x30    
#define GPIO_IRQSTATUS_SET_0   0x34
#define GPIO_IRQSTATUS_SET_1   0x38
#define GPIO_IRQSTATUS_CLR_0   0x3C
#define GPIO_IRQSTATUS_CLR_1   0x40
#define GPIO_SYSSTATUS         0x114 
#define GPIO_CTRL              0x130
#define GPIO_OE                0x134
#define GPIO_DATAIN            0x138
#define GPIO_DATAOUT           0x13C
#define GPIO_LEVELDETECT0      0x140
#define GPIO_LEVELDETECT1      0x144
#define GPIO_RISINGDETECT      0x148
#define GPIO_FALLINGDETECT     0x14C
#define GPIO_DEBOUNCENABLE     0x150
#define GPIO_DEBOUNCINGTIME    0x154
#define GPIO_CLEARDATAOUT      0x190
#define GPIO_SETDATAOUT        0x194

static volatile unsigned int *gpio_addr;


struct timer_list exp_timer;

static void do_something(unsigned long data)
{
	
	if((gpio_addr[GPIO_DATAOUT/4]>>22) == 1)
	{
		gpio_addr[GPIO_CLEARDATAOUT/4] |=(1<<22);
	}
	else
	{
		gpio_addr[GPIO_SETDATAOUT/4] |=(1<<22);
	}
	
	printk("Your timer expired and app has been called\n");
	//gpio_addr[GPIO_SETDATAOUT/4] |=(1<<22);
	exp_timer.expires  = jiffies +10;
	add_timer(&exp_timer);
}

static int __init tst_init(void)
{
	int delay = 10;
	gpio_addr = (unsigned int*)ioremap(GPIO1_START_ADDR,GPIO_SIZE);
	printk(KERN_INFO "Init called\n");

	init_timer_on_stack(&exp_timer);

	exp_timer.expires = jiffies + delay;
	exp_timer.data = 0;
	exp_timer.function = do_something;
	
	add_timer(&exp_timer);
	gpio_addr[GPIO_OE/4] &= ~(1<<22);
	gpio_addr[GPIO_OE/4] |= (1<<15);
	gpio_addr[GPIO_DEBOUNCENABLE/4] |= (1<<15);
	gpio_addr[GPIO_DEBOUNCINGTIME/4] = 50;

	return 0;
}

static void __exit tst_exit(void)
{
	gpio_addr[GPIO_CLEARDATAOUT/4]|=(1<<22);
	iounmap(gpio_addr);
	del_timer(&exp_timer);  
	printk(KERN_INFO "Exit called\n");
}

module_init(tst_init);
module_exit(tst_exit);
