#include "max7219.h"
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




void max7219_send(struct spi_device *spi, unsigned char _addr,unsigned char _val)
{   
    unsigned int temp;
    temp = (_addr<<8)|_val;
    spi_write(spi,&temp,2);
}
void max7219_config(struct spi_device *spi)
{
    unsigned char i;
    max7219_send(spi,RES_SHUT_DOWN,1);
    max7219_send(spi,RES_INTENSITY,5);
    max7219_send(spi,RES_SCAN_LIMIT,7);
    max7219_send(spi,RES_MODE_DECODE,0xFF);
    for(i = 1;i<9;i++)
    {
        max7219_send(spi,i,0x0A);
    }
}
void max7219_display(struct spi_device *spi,unsigned char buff[],unsigned char _len)
{
    unsigned char i;
    unsigned char temp;
    max7219_send(spi,RES_SCAN_LIMIT,_len-1);
    for(i = 0;i<=_len;i++)
    {
        if(buff[i] == '-')
        {
            max7219_send(spi,i+1,0x0A);
        }
        else
        {
            temp = buff[i] - 0x30;
            max7219_send(spi,i+1,temp);
        }
        
    }
}
void max7219_shutdown(struct spi_device *spi)
{

    max7219_send(spi,RES_SHUT_DOWN,0);
}
void max7219_decode(struct spi_device *spi,unsigned char _number)
{
    max7219_send(spi,RES_MODE_DECODE,_number);
}
void max7219_intensity(struct spi_device *spi,unsigned char _val)
{
    if((_val < 0) || (_val > 0x0f))
    {
        max7219_send(spi,RES_INTENSITY,0x0f);
    }
    else
    {
        max7219_send(spi,RES_INTENSITY,_val);
    }
}
void max7219_scan_limit(struct spi_device *spi, unsigned char _number)
{
    max7219_send(spi, RES_SCAN_LIMIT,_number);
}