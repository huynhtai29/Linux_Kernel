#include <linux/init.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/device.h>
#include <linux/gpio/driver.h>
#include <linux/gpio.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/platform_device.h>
#include <linux/pm_runtime.h>
#include <linux/pm.h>
#include <linux/bitops.h>

#define GPIO_REG_OFFS_REVISION				0x00
#define GPIO_REG_OFFS_SYSCONFIG				0x10
#define GPIO_REG_OFFS_EOI					0x20
#define GPIO_REG_OFFS_IRQSTATUS_RAW_0		0x24
#define GPIO_REG_OFFS_IRQSTATUS_RAW_1		0x28
#define GPIO_REG_OFFS_IRQSTATUS_0			0x2C
#define GPIO_REG_OFFS_IRQSTATUS_1			0x30
#define GPIO_REG_OFFS_IRQSTATUS_SET_0		0x34
#define GPIO_REG_OFFS_IRQSTATUS_SET_1		0x38
#define GPIO_REG_OFFS_IRQSTATUS_CLR_0		0x3C
#define GPIO_REG_OFFS_IRQSTATUS_CLR_1		0x40
#define GPIO_REG_OFFS_IRQWAKEN_0			0x44
#define GPIO_REG_OFFS_IRQWAKEN_1			0x48
#define GPIO_REG_OFFS_SYSSTATUS				0x114
#define GPIO_REG_OFFS_CTRL					0x130
#define GPIO_REG_OFFS_OE					0x134
#define GPIO_REG_OFFS_DATAIN				0x138
#define GPIO_REG_OFFS_DATAOUT				0x13C
#define GPIO_REG_OFFS_LEVELDETECT0			0x140
#define GPIO_REG_OFFS_LEVELDETECT1			0x144
#define GPIO_REG_OFFS_RISINGDETECT			0x148
#define GPIO_REG_OFFS_FALLINGDETECT			0x14C
#define GPIO_REG_OFFS_DEBOUNCENABLE			0x150
#define GPIO_REG_OFFS_DEBOUNCINGTIME		0x154
#define GPIO_REG_OFFS_CLEARDATAOUT			0x190
#define GPIO_REG_OFFS_SETDATAOUT			0x194

#define LEVELDETECT0			0x1
#define LEVELDETECT1			0x2
#define RISINGDETECT			0x3
#define FALLINGDETECT			0x4


struct gpio_reg_offs 
{
    u32 revision;
    u32 sysconfig;
    u32 eoi;
    u32 irqstatus_raw0;
    u32 irqstatus_raw1;
    u32 irq_status0;
    u32 irq_status1;
    u32 irq_status_set0;
    u32 irq_status_set1;
    u32 irq_status_clr0;
    u32 irq_status_clr1;
    u32 irq_waken0;
    u32 irq_waken1;
    u32 sysstatus;
    u32 ctrl;
    u32 oe;
    u32 datatin;
    u32 dataout;
    u32 leveldetect0;
    u32 leveldetect1;
    u32 rising;
    u32 falling;
    u32 debounce_enable;
    u32 debonce_time;
    u32 clear_dataout;
    u32 set_dataout;
};


struct my_gpio_bank {
	struct list_head list_node;
	void __iomem *base;
	int irq;
	struct gpio_reg_offs *regs;
	struct gpio_chip chip;
	u32 mod_usage;
	u32 irq_usage;
	u32 pins;
};
static struct my_gpio_bank *to_my_gpio_bank(struct gpio_chip *chip)
{
	struct my_gpio_bank *ret = gpiochip_get_data(chip);

	if (ret == NULL)
		pr_err("Can not get bank by chip\n");

	return ret;
}

/* Funtions for my_gpio_bank->gpio_chip*/
void __enable_modules(struct my_gpio_bank *bank, int is_enable)
{
	void __iomem *base = bank->base;
	if (is_enable)
		iowrite32(0, base + bank->regs->ctrl); /* enable clock gate */
	else
		iowrite32(1, base + bank->regs->ctrl); /* disable clock gate */
}

int __get_direction(struct my_gpio_bank *bank, int pin)
{
	int temp_reg = 0;
	void __iomem *base = bank->base;

	if (bank->regs->oe)
		temp_reg = ioread32(base + bank->regs->oe);
	else
		pr_err("Register not exsit\n");

	temp_reg &= (1 << pin);

	if (temp_reg)
		return 1;
	else
		return 0;
}

void __set_direction(struct my_gpio_bank *bank, int pin, char data)
{
	void __iomem *base = bank->base;
	int temp_reg = ioread32(base + bank->regs->oe);

	if (data)
		temp_reg |= (1 << pin);
	else
		temp_reg &= ~(1 << pin);

	iowrite32(temp_reg, base + bank->regs->oe);
}

unsigned int __get_datain(struct my_gpio_bank *bank, int pin)
{
	void __iomem *base = bank->base;
	int temp_reg = ioread32(base + bank->regs->datatin);

	return (temp_reg & (1 << pin));
}

unsigned int __get_dataout(struct my_gpio_bank *bank, int pin)
{
	void __iomem *base = bank->base;
	int temp_reg = ioread32(base + bank->regs->dataout);

	return (temp_reg & (1 << pin));
}

void __set_dataout(struct my_gpio_bank *bank, int pin, char data)
{
	void __iomem *base = bank->base;
	int temp_reg = ioread32(base + bank->regs->dataout);

	if (data)
		temp_reg |= 1 << pin;
	else
		temp_reg &= ~(1 << pin);

	iowrite32(temp_reg, base + bank->regs->dataout);
}

int	my_gpio_request(struct gpio_chip *chip, unsigned int pin)
{
	struct my_gpio_bank *bank = to_my_gpio_bank(chip);

	if (!(bank->pins)) {
		pm_runtime_get_sync(bank->chip.parent);
		__enable_modules(bank, 1);
	}
	bank->pins |= (1 << pin);
	return 0;
}

void my_gpio_free(struct gpio_chip *chip, unsigned int pin)
{
	struct my_gpio_bank *bank = to_my_gpio_bank(chip);

	pinctrl_free_gpio(chip->base + pin);
	bank->pins &= ~(1 << pin);

	if (!(bank->pins)) {
		pm_runtime_put(chip->parent);
		__enable_modules(bank, 0);
	}
}

int	my_gpio_get_direction(struct gpio_chip *chip, unsigned int pin)
{
	int ret = 0;
	struct my_gpio_bank *bank = to_my_gpio_bank(chip);

	if (bank == NULL) {
		pr_err("error get bank\n");
		return -1;
	}

	ret = __get_direction(bank, pin);

	return ret;
}

int	my_gpio_direction_input(struct gpio_chip *chip, unsigned int pin)
{
	struct my_gpio_bank *bank = to_my_gpio_bank(chip);

	if (bank == NULL) {
		pr_err("error get bank\n");
		return -1;
	}

	__set_direction(bank, pin, 1);

	return 0;
}

int	my_gpio_direction_output(struct gpio_chip *chip,
		unsigned int pin, int value)
{
	struct my_gpio_bank *bank = to_my_gpio_bank(chip);

	__set_direction(bank, pin, 0);
	__set_dataout(bank, pin, value);

	return 0;
}

int my_gpio_get(struct gpio_chip *chip, unsigned int pin)
{
	struct my_gpio_bank *bank = to_my_gpio_bank(chip);
	int ret = 0;

	if (bank == NULL)
		return -1;

	ret = __get_direction(bank, pin);

	if (ret == 1)
		ret = __get_datain(bank, pin);
	else
		ret = __get_dataout(bank, pin);

	return ret;
}

void my_gpio_set(struct gpio_chip *chip, unsigned int pin, int value)
{
	struct my_gpio_bank *bank = to_my_gpio_bank(chip);

	if (bank == NULL)
		return;

	__set_dataout(bank, pin, value);
}

int my_gpio_set_config(struct gpio_chip *chip,
		unsigned int pin, unsigned long config)
{
	struct my_gpio_bank *bank = to_my_gpio_bank(chip);

	__set_direction(bank, pin, 0);
	__set_dataout(bank, pin, 1);

	return 0;
}
int init_gpio_chip(struct platform_device *pdev)
{
	static int gpio = 64;
	int ret = 0;
	struct resource *res;
	struct device *dev = &pdev->dev;
	struct my_gpio_bank *bank = platform_get_drvdata(pdev);
	if(bank == NULL)
		dev_err(dev,"can't get drvdata\n");
	res = platform_get_resource(pdev,IORESOURCE_MEM,0);
	bank->base = devm_ioremap_resource(dev,res);
	if (bank->base == NULL) {
		dev_err(dev, "can't get resource resource\n");
		return PTR_ERR(bank->base);
	}
	bank->chip.parent = dev;
	bank->chip.owner = THIS_MODULE;
	bank->chip.of_node = dev->of_node;
	bank->chip.base = gpio;
	bank->chip.ngpio = 32;
	bank->chip.label = "gpio";

	bank->chip.request = my_gpio_request;
	bank->chip.free = my_gpio_free;
	bank->chip.get_direction = my_gpio_get_direction;
	bank->chip.direction_input = my_gpio_direction_input;
	bank->chip.direction_output = my_gpio_direction_output;
	bank->chip.get = my_gpio_get;
	bank->chip.set = my_gpio_set;
	bank->chip. set_config = my_gpio_set_config;
	ret = gpiochip_add_data(&bank->chip, bank);
	if (ret) {
		dev_err(bank->chip.parent, "Could not register gpio chip\n");
		return ret;
	}		
}
static const struct of_device_id my_of_match[];
static int my_gpio_probe(struct platform_device *pdev)
{

	int ret =0;
	struct device *dev = &pdev->dev;
	struct my_gpio_bank *bank = NULL;
	const struct gpio_reg_offs *pdata;
	const struct of_device_id *match;
	match = of_match_device(of_match_ptr(my_of_match),dev);
	pdata = match ? match->data : dev_get_platdata(dev);
	printk(KERN_INFO "hello \n");
	if(!pdata)
		return -EINVAL;
	bank = devm_kzalloc(dev, sizeof(struct my_gpio_bank), GFP_KERNEL);
	if (!bank)
	{
		pr_err("Error allocate memmory\n");
		return -ENOMEM;
	}
	pm_runtime_enable(dev);
	pm_runtime_get_sync(dev);
	bank->regs = pdata;
	platform_set_drvdata(pdev, bank);
	ret = init_gpio_chip(pdev);
	if(ret){
		devm_kfree(&pdev->dev, bank);
		pr_err("Error init gpio_chip");
		return ret;				
	}
	pm_runtime_put(bank->chip.parent);
	printk(KERN_INFO "hello \n");
    return 0;
}
	
static int my_gpio_remove(struct platform_device *pdev)
{
	struct my_gpio_bank *bank = platform_get_drvdata(pdev);
	gpiochip_remove(&bank->chip);
	pm_runtime_disable(&pdev->dev);
	return 0;
}
struct gpio_reg_offs reg_offset = 
{
	.revision        = GPIO_REG_OFFS_REVISION,
	.sysconfig       = GPIO_REG_OFFS_SYSCONFIG,
	.eoi             = GPIO_REG_OFFS_EOI,
	.irqstatus_raw0  = GPIO_REG_OFFS_IRQSTATUS_RAW_0,
	.irqstatus_raw1  = GPIO_REG_OFFS_IRQSTATUS_RAW_1,
	.irq_status0     = GPIO_REG_OFFS_IRQSTATUS_0,
	.irq_status1     = GPIO_REG_OFFS_IRQSTATUS_1,
	.irq_status_set0 = GPIO_REG_OFFS_IRQSTATUS_SET_0,
	.irq_status_set1 = GPIO_REG_OFFS_IRQSTATUS_SET_1,
	.irq_status_clr0 = GPIO_REG_OFFS_IRQSTATUS_CLR_0,
	.irq_status_clr1 = GPIO_REG_OFFS_IRQSTATUS_CLR_1,
	.irq_waken0 	 = GPIO_REG_OFFS_IRQWAKEN_0,
	.irq_waken1 	 = GPIO_REG_OFFS_IRQWAKEN_1,
	.sysstatus 		 = GPIO_REG_OFFS_SYSSTATUS,
	.ctrl            = GPIO_REG_OFFS_CTRL,
	.oe              = GPIO_REG_OFFS_OE,
	.datatin 		 = GPIO_REG_OFFS_DATAIN,
	.dataout 		 = GPIO_REG_OFFS_DATAOUT,
	.leveldetect0    = GPIO_REG_OFFS_LEVELDETECT0,
	.leveldetect1    = GPIO_REG_OFFS_LEVELDETECT1,
	.rising          = GPIO_REG_OFFS_RISINGDETECT,
	.falling         = GPIO_REG_OFFS_FALLINGDETECT,
	.debounce_enable = GPIO_REG_OFFS_DEBOUNCENABLE,
	.debonce_time    = GPIO_REG_OFFS_DEBOUNCINGTIME,
	.clear_dataout   = GPIO_REG_OFFS_CLEARDATAOUT,
	.set_dataout     = GPIO_REG_OFFS_SETDATAOUT,
};

static const struct of_device_id my_of_match[] = {
	{
		.compatible = "huynhtai,omap4-gpio",
		.data = &reg_offset,
	},
	{},
};
MODULE_DEVICE_TABLE(of, my_of_match);	
static struct platform_driver my_platform_driver = 
{
    .probe = my_gpio_probe,
    .remove	= my_gpio_remove,
    .driver		= {
    	.name	= "my_bbb_gpio",
    	.owner = THIS_MODULE,
    	.of_match_table = my_of_match,
    }
};
module_platform_driver(my_platform_driver);
MODULE_AUTHOR("HUYNH TAI");
MODULE_LICENSE("GPL");
    
