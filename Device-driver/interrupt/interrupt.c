#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/interrupt.h>
#include <linux/ioctl.h>
#include <asm/gpio.h>
#include <asm/io.h>
#include <linux/err.h>

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


#define GPIO_PIN_INTERRUPT      12

MODULE_LICENSE("GPL");
MODULE_AUTHOR("NHT huynhtai291998@gmail.com");



static volatile unsigned int *gpio_addr;







void config_interrupt_gpio(void)
{
	gpio_addr[GPIO_OE/4] |=(1<<GPIO_PIN_INTERRUT); // config input
//	gpio_addr[GPIO_LEVELDETECT0/4] |=(1<<GPIO_PIN_INTERRUPT); //mode low	
//	gpio_addr[GPIO_LEVELDETECT1/4] |=(1<<GPIO_PIN_INTERRUPT); //mode high
//	gpio_addr[GPIO_FALLINGETECT/4] |=(1<<GPIO_PIN_INTERRUPT); //mode falling
//	gpio_addr[GPIO_RISINGDETECT/4] |=(1<<GPIO_PIN_INTERRUPT); //mode rising
}


int __init external_interrupt_init(void)
{
   	printk("start interrupt_module");
	gpio_addr = (unsigned int*)ioremap(GPIO1_START_ADDR,GPIO_SIZE);
   	gpio_addr[GPIO_OE/4] = 0;
    	gpio_addr[GPIO_SETDATAOUT/4] |= (1<<22);
    return 0;
}

void __exit external_interrupt_exit(void)
{
	printk("exit_module\n");
	gpio_addr[GPIO_CLEARDATAOUT/4] |= (1<<22);
      	iounmap(gpio_addr);
}

module_init(external_interrupt_init);
module_exit(external_interrupt_exit);
