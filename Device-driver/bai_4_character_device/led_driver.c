#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/kdev_t.h>
#include <linux/types.h>
#include <linux/interrupt.h>
#include <linux/ioctl.h>
#include <linux/uaccess.h>
#include <asm/gpio.h>
#include <asm/io.h>
#include <linux/err.h>
#include <linux/miscdevice.h>


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


#define MAGIC_NO 		100
#define MODE_NORMAL 		_IOW(MAGIC_NO,0,int)
#define MODE_BLYNK		_IOW(MAGIC_NO,1,int)


MODULE_AUTHOR("huynhtai291998@gmail.com");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.1");

static volatile unsigned int *gpio_addr;
static volatile unsigned int *gpio_setdataout_addr;
static volatile unsigned int *gpio_cleardataout_addr;
static volatile unsigned int *gpio_oe_addr;
static volatile unsigned int *gpio_dataout_addr;

static dev_t dev;
static struct cdev my_cdev;
static struct device *device_name;
static struct class *class_name;
static int dev_open(struct inode*,struct file *);
static int dev_close(struct inode*,struct file *);
static ssize_t dev_read(struct file*,char __user*,size_t len, loff_t *);
static ssize_t dev_write(struct file*,const char __user*,size_t len, loff_t *);
static long dev_ioctl(struct file *,unsigned int , unsigned long);
void config_gpio_bbb(void);
void _set_value_gpio(unsigned char  gpio_pin, unsigned char value);

char pin =22;
char value =1;


static struct file_operations fops = {
	.open = dev_open,
	.release = dev_close,
	.unlocked_ioctl =dev_ioctl,
	.read = dev_read,
	.write = dev_write,
};


static int dev_open(struct inode *inodep, struct file *filep)
{
	printk("open\n");
	return 0;
}
static int dev_close(struct inode *inodep, struct file *filep)
{
	printk("close\n");
	return 0;
}

static ssize_t dev_read(struct file*filep, char __user *buf, size_t len, loff_t *offset)
{
	printk("read\n");
	return 0;
}

struct data_config
{
	int tan_so;
	int time;
}data_config;


static long dev_ioctl(struct file *filep,unsigned int cmd, unsigned long arg)
{	

	printk("ioctl %d\n",arg);
	struct data_config mydata;
	copy_from_user(&mydata,arg,sizeof(mydata));
	switch(cmd)
	{
	case MODE_NORMAL:
		printk("mode normal");
		_set_value_gpio(22,1);
		break;
		
	case MODE_BLYNK:
		printk("mode blynk: tanso :%d time:  %d",mydata.tan_so,mydata.time);
		_set_value_gpio(22,0);
		break;
	default :
		printk("error\n\r");
	}
	return 0;
}	

char data[100];
static ssize_t dev_write(struct file*filep, const char __user *buf, size_t len, loff_t *offset)
{
	memset(data,0,100);
	copy_from_user(data,buf,len);
	if(data[0] == '0')
	{	printk("led off");
		_set_value_gpio(22,0);
	}
	else if(data[0] == '1')
	{
		printk("led on");
		_set_value_gpio(22,1);

	}
	printk("user write = %d \n",data[0]);
	return -1;
}

void config_gpio_bbb(void)
{

	// Configure Register BBB 
	gpio_addr = (unsigned int*)ioremap(GPIO0_START_ADDR,GPIO0_SIZE);
	gpio_setdataout_addr = (unsigned int *)(gpio_addr + GPIO0_SETDATAOUT/4);
	gpio_oe_addr = (unsigned int *)(gpio_addr + GPIO0_OE/4);
	gpio_cleardataout_addr =(unsigned int *)(gpio_addr + GPIO0_CLEARDATAOUT/4);

}

void _set_value_gpio(unsigned char gpio_pin,unsigned char value)
{
	if(value == 1)
	{
		*gpio_setdataout_addr |= (1 << gpio_pin);
	}
	else if(value == 0)
	{
		*gpio_cleardataout_addr |= (1 << gpio_pin);
	}
}



static int __init exam_init(void)
{
    // Creat Device File
	int ret;
	ret = alloc_chrdev_region(&dev, 0, 1, "led_device");
	if(ret)
	{
		printk("can not register major no\n");
		return ret;
	}
	pr_info(KERN_INFO "register successfully major now is: %d\n", MAJOR(dev));
	
	cdev_init(&my_cdev,&fops);
	my_cdev.owner = THIS_MODULE;
	my_cdev.dev = dev;
	ret = cdev_add(&my_cdev, dev, 1);
	if(ret < 0)
	{
		pr_info("cdev_add error\n");
		return ret;
	}
   	class_name = class_create(THIS_MODULE,"led_device");
	if(IS_ERR(class_name))
	{
		pr_info("create class failed\n");
	}
	else
		pr_info("create successfully class\n");
	device_name = device_create(class_name,NULL,dev,NULL,"led_device");
	if(IS_ERR(device_name))
	{
		pr_info("create device failds\n");
	}
	else
		pr_info("create device success\n");
    
	// Configure Register BBB 
	config_gpio_bbb();
	return 0;
}

static void __exit exam_exit(void)
{
	pr_info("goodbye\n");
	cdev_del(&my_cdev);
	device_destroy(class_name,dev);
	class_destroy(class_name);
	unregister_chrdev_region(dev, 0);
   	iounmap(gpio_addr);
}

module_init(exam_init);
module_exit(exam_exit);

