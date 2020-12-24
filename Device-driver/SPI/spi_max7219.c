#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/io.h>
#include <linux/slab.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/ioctl.h>
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/spi/spi.h>
#include <linux/string.h>
#include <linux/of_gpio.h>
#include "max7219.h"

static dev_t dev; // major minor
static struct class *class_name;
static LIST_HEAD(device_list);

/* Create funtion into character device file   */

struct spi_max7219
{
    struct spi_device *spi;
    dev_t num_dev;
    struct list_head device_entry;    
    struct cdev c_dev;
    struct device *device_p;
};
static int max7219_open(struct inode *inodep,struct file *filep)
{
	struct spi_max7219 *spidev = NULL;
	unsigned char status = -ENXIO;
	list_for_each_entry(spidev,&device_list,device_entry) 
	{
		if (spidev->num_dev == inodep->i_rdev) {
			status = 0;
			break;
		}
	}
	if(status)
	{
		pr_info("not find device \n");
		return status;
	}
	filep->private_data = spidev;
	return 0;

}
static int max7219_release(struct inode *inodep,struct file *filep)
{
	filep->private_data = NULL;
	pr_info("device file closed \n");
	return 0;
}
static int max7219_write(struct file *filep,const char __user *buf,size_t len,loff_t *offset)
{
	struct spi_max7219 *spidev = filep->private_data;
	unsigned char buff[20];
	int res;
	res = copy_from_user(buff,buf,len);
	if(res < 0)
		pr_info("Error copy from user \n");
	pr_info("User send buff : %s, len : %d \n",buff,len);
	max7219_display(spidev->spi,buff,len);
	return 0;

}
static long max7219_ioctl(struct file *filep,unsigned int cmd,unsigned long arg)
{
	struct spi_max7219 *spidev = filep->private_data;
	struct val_res_max7219 _data;
	copy_from_user(&_data,arg,sizeof(struct val_res_max7219));
	
	switch(cmd){
	case IOCTL_CONFIG:
		max7219_config(spidev->spi);
		break;
	case IOCTL_SHUTDOWN:
		max7219_shutdown(spidev->spi);
		break;
	case IOCTL_INTENSITY:
		max7219_intensity(spidev->spi,5);
		break;
	case IOCTL_DECODE:
		max7219_decode(spidev->spi,0xff);
		break;
	case IOCTL_SCAN_LIMIT:
		max7219_scan_limit(spidev->spi,7);
		break;
	default:
		break;
	}	

}

static struct file_operations fops = {
    .open = max7219_open,
    .write = max7219_write,
    .release = max7219_release,
    .unlocked_ioctl = max7219_ioctl,
};


static int my_probe(struct spi_device *spi)
{

    struct spi_max7219 *spidev = NULL;
    int res = 0;
    int minor = spi->chip_select;
    int major = MAJOR(dev);

    spidev = kzalloc(sizeof(*spidev),GFP_KERNEL);
	if (!spidev)
		return -ENOMEM;
    spidev->spi = spi;
    spi_set_drvdata(spi , spidev);
    spi->bits_per_word = 16;
    res = spi_setup(spi);
    pr_info("chip_select = %d\n", spi->chip_select);
    pr_info("clock = %d\n", spi->max_speed_hz);
    pr_info("cs = %d\n", spi->cs_gpio);
    /* Creat Device File */
    spidev->num_dev = MKDEV(major,minor);
    spidev->device_p =  device_create(class_name,&spi->dev,spidev->num_dev,spidev,"spidev1_%d",spi->chip_select);
    if(spidev == NULL)
    {
        printk("can not creat device file\n");

    }
    /* Register fos into device file */
    cdev_init(&spidev->c_dev,&fops);
    spidev->c_dev.owner = THIS_MODULE;
    spidev->c_dev.dev = spidev->num_dev;
    res = cdev_add(&spidev->c_dev,spidev->num_dev,1);
    if(res)
    {
        pr_err("error occur when add properties for struct cdev\n");

    }
    INIT_LIST_HEAD(&spidev->device_entry);
    list_add(&spidev->device_entry,&device_list);
    max7219_config(spi);

}
static int my_remove(struct spi_device *spi)
{
    struct spi_max7219 *spidev = spi_get_drvdata(spi);
    list_del(&spidev->device_entry);
    cdev_del(&spidev->c_dev);
    device_destroy(class_name,spidev->num_dev);
    unregister_chrdev_region(spidev->num_dev, 1);
    kfree(spidev);
}

struct of_device_id huynhtai_of_match[] = {
    {
        .compatible = "huynhtai,spi1",
    },
    {},
};

MODULE_DEVICE_TABLE(of,huynhtai_of_match);

static struct spi_driver my_spi_driver = {
    .probe = my_probe,
    .remove = my_remove,
    .driver = {
        .name = "max7219",
        .owner = THIS_MODULE,
        .of_match_table = huynhtai_of_match,
    }
};


static int __init max7219_init(void)
{
    int res = 0;
    res = alloc_chrdev_region(&dev,0,2,"spi_max7219"); // create struct proposal to kernel allocate majoi and minor
    if(res < 0)
	{
		printk("can not register major \n");
		return res;
	}
	pr_info(KERN_INFO "register successfully major now is: %d\n", MAJOR(dev));
    class_name = class_create(THIS_MODULE,"spi_max7219");
    if(class_name == NULL)
    {
    	pr_info("create class failed\n");
        unregister_chrdev(dev,0);
    }
    res = spi_register_driver(&my_spi_driver);
    if(res <0)
    {
        printk("can not register spi_driver \n");
        class_destroy(class_name);
    }
    
    return res;
}
static void __exit max7219_exit(void)
{
    spi_unregister_driver(&my_spi_driver);
    class_destroy(class_name);
    unregister_chrdev_region(dev,2);
    

}

module_init(max7219_init);
module_exit(max7219_exit);

MODULE_AUTHOR("Huynh Tai");
MODULE_LICENSE("GPL");
