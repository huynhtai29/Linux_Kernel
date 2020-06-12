#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
//#include <linux/interrupt.h>
#include <linux/ioctl.h>
#include <asm/gpio.h>
#include <asm/io.h>
//#include <linux/err.h>

#define GPIO0_START_ADDR        0x4804C000 
#define GPIO0_END_ADDR		0X4804DFFF
#define GPIO0_SIZE              (GPIO0_END_ADDR - GPIO0_START_ADDR)
#define GPIO0_REVISION          0x00
#define GPIO0_SYSCONFIG         0x10
#define GPIO0_EOI               0x20
#define GPIO0_IRQSTATUS_RAW_0   0x24
#define GPIO0_IRQSTATUS_RAW_1   0x28
#define GPIO0_IRQSTATUS_0       0x2C
#define GPIO0_IRQSTATUS_1       0x30    
#define GPIO0_IRQSTATUS_SET_0   0x34
#define GPIO0_IRQSTATUS_SET_1   0x38
#define GPIO0_IRQSTATUS_CLR_0   0x3C
#define GPIO0_IRQSTATUS_CLR_1   0x40
#define GPIO0_SYSSTATUS         0x114 
#define GPIO0_CTRL              0x130
#define GPIO0_OE                0x134
#define GPIO0_DATAIN            0x138
#define GPIO0_DATAOUT           0x13C
#define GPIO0_LEVELDETECT0      0x140
#define GPIO0_LEVELDETECT1      0x144
#define GPIO0_RISINGDETECT      0x148
#define GPIO0_FALLINGDETECT     0x14C
#define GPIO0_DEBOUNCENABLE     0x150
#define GPIO0_DEBOUNCINGTIME    0x154
#define GPIO0_CLEARDATAOUT      0x190
#define GPIO0_SETDATAOUT        0x194


static volatile unsigned int *gpio_addr;
static volatile unsigned int *gpio_setdataout_addr;
static volatile unsigned int *gpio_cleardataout_addr;
static volatile unsigned int *gpio_oe_addr;
static volatile unsigned int *gpio_dataout_addr;
static void __iomem *io;

int __init init_module(void)
{
//    	printk("start gpio_modul\n");
	//gpio_addr = (unsigned int*)ioremap(GPIO0_START_ADDR,GPIO0_SIZE);
	io = ioremap(GPIO0_START_ADDR,GPIO0_SIZE);
	//    	if(gpio_addr == NULL)
//   	 {
//    		printk("Mapping gpio_addr fail \n");
//		goto gpio_addr_fail;	
//	 }
//    	gpio_addr[GPIO0_SYSCONFIG/4] =0; 
    //	gpio_addr[GPIO0_CTRL/4] |= (1<<1);
   	//gpio_addr[GPIO0_OE/4] = 0;
    	//gpio_addr[GPIO0_SETDATAOUT/4] |= (1<<22);
	iowrite32(0xC00000,io +GPIO0_SETDATAOUT);
/*	gpio_setdataout_addr = (unsigned int *)(gpio_addr + GPIO0_SETDATAOUT/4);
	gpio_oe_addr = (unsigned int *)(gpio_addr + GPIO0_OE/4);
	gpio_cleardataout_addr =(unsigned int *)(gpio_addr + GPIO0_CLEARDATAOUT/4);
	gpio_dataout_addr =(unsigned int *)(gpio_addr + GPIO0_DATAOUT/4);

*/
    return 0;
}

void __exit cleanup_module(void)
{

//    	printk("end GPIO_modul %x - %x -%x \n",*gpio_addr,*gpio_setdataout_addr,*gpio_cleardataout_addr);
//    gpio_addr[GPIO0_CLEARDATAOUT/4] |= (1<<22);
//  	*gpio_cleardataout_addr |= (1<<22);
	iowrite32(0xc00000,io + GPIO0_CLEARDATAOUT);
      	iounmap(gpio_addr);
}

//module_init(init_module);
//module_exit(cleanup_module);
