#ifndef __MAX_7219
#define __MAX_7219

#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/spi/spi.h>
#define MAGIC_NO 		100
#define RES_SHUT_DOWN 0x0C
#define RES_MODE_DECODE 0x09
#define RES_INTENSITY 0x0A
#define RES_SCAN_LIMIT 0x0B
#define RES_D1 0x01
#define RES_D2 0x02
#define RES_D3 0x03
#define RES_D4 0x04
#define RES_D5 0x05
#define RES_D6 0x06
#define RES_D7 0x07
#define RES_D8 0x08
#define IOCTL_SHUTDOWN      _IOW(MAGIC_NO,0,int)
#define IOCTL_INTENSITY     _IOW(MAGIC_NO,1,int)
#define IOCTL_DECODE        _IOW(MAGIC_NO,2,int)
#define IOCTL_SCAN_LIMIT    _IOW(MAGIC_NO,3,int)
#define IOCTL_CONFIG        _IOW(MAGIC_NO,4,int)
#define SHUT_DOWN_MODE  0x00
#define NORMAL_MODE     0x01

struct val_res_max7219{
    unsigned char val_res_shutdown;
    unsigned char val_res_decode;
    unsigned char val_res_scan_limit;
    unsigned char val_res_intensity; 
};

void max7219_send(struct spi_device *spi, unsigned char _addr,unsigned char _val);
void max7219_config(struct spi_device *spi);
void max7219_display(struct spi_device *spi,unsigned char buff[],unsigned char _len);
void max7219_shutdown(struct spi_device *spi);
void max7219_decode(struct spi_device *spi,unsigned char _number);
void max7219_intensity(struct spi_device *spi,unsigned char _val);
void max7219_scan_limit(struct spi_device *spi,unsigned char _number);

#endif