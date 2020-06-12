#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/io.h>
#include <linux/slab.h>
#include <linux/mm.h>
#include <linux/reboot.h>
#include <linux/err.h>
#include <linux/platform_device.h>
#include <linux/moduleparam.h>
#include <linux/pm_runtime.h>
#include <linux/watchdog.h>
#include <linux/delay.h>
#define WDT_DEFAULT_TIME 30  
/* Register Map WDT-am335x*/
#define PTV			2
#define GET_WLDR_VAL(secs)      (0xffffffff - ((secs) * (32768/(1<<PTV))) + 1)
#define TIMER_MARGIN_MAX		(24 * 60 * 60)	/* 1 day */
#define TIMER_MARGIN_MIN		1		/* 1 sec */
#define TIMER_MARGIN_DEFAULT    60      /* 60 secs */

#define WDT_WIDR		(0x00)
#define WDT_WDSC		(0x10)
#define WDT_WDST		(0x14)
#define WDT_WISR		(0x18)
#define WDT_WIER		(0x1C)
#define WDT_WCLR		(0x24)
#define WDT_WCRR		(0x28)
#define WDT_WLDR		(0x2C)
#define WDT_WTGR		(0x30)
#define WDT_WWPS		(0x34)
#define WDT_WDLY		(0x44)
#define WDT_WSPR		(0x48)

static bool nowayout = WATCHDOG_NOWAYOUT;
module_param(nowayout, bool, 0000);
MODULE_PARM_DESC(nowayout, "Watchdog cannot be stopped once started (default="
		__MODULE_STRING(WATCHDOG_NOWAYOUT) ")");
static unsigned timeout = WDT_DEFAULT_TIME;
module_param(timeout, uint, 0);
MODULE_PARM_DESC(timeout, "Watchdog timeout in seconds (default="
__MODULE_STRING(WDT_DEFAULT_TIME) ")");



struct my_omap_wdt_dev
{
    struct watchdog_device wdog;
    void __iomem    *base;
    struct device   *dev;
    bool            wdt_active;
    int             wdt_trig;
    struct mutex    lock;
};
static void __wdt_enable(struct my_omap_wdt_dev *wdt)
{
    void __iomem *base = wdt->base;
    iowrite32(0xBBBB,base + WDT_WSPR);
	while ((readl_relaxed(base + WDT_WWPS)) & (1<<4))
		cpu_relax();
    iowrite32(0x4444,base + WDT_WSPR);
    while ((readl_relaxed(base + WDT_WWPS)) & (1<<4))
		cpu_relax();

}
static void __wdt_disable(struct my_omap_wdt_dev *wdt)
{
    void __iomem *base = wdt->base;
    __raw_writel(0xAAAA,base +WDT_WSPR);
    while ((__raw_readl(base + WDT_WWPS)) & (1<<4))
		cpu_relax();
    __raw_writel(0x5555,base + WDT_WSPR);
    while ((__raw_readl(base + WDT_WWPS)) & (1<<4))
		cpu_relax();
}
static void __wdt_set_timeout(struct my_omap_wdt_dev *wdt , unsigned int _timeout)
{
    unsigned int val_load = GET_WLDR_VAL(_timeout);
    void __iomem *base = wdt->base;
    while (readl_relaxed(base + WDT_WWPS) & (1<<2))
		cpu_relax();
    iowrite32(val_load,base + WDT_WLDR);
    while (readl_relaxed(base + WDT_WWPS) & (1 << 2))
		cpu_relax();

}
static void __wdt_reload(struct my_omap_wdt_dev *wdt)
{
    void __iomem *base = wdt->base;
    wdt->wdt_trig = ~wdt->wdt_trig;
	while ((readl_relaxed(base + WDT_WWPS)) & (1 << 3))
		cpu_relax();
	iowrite32(wdt->wdt_trig, (base + WDT_WTGR));
	while ((readl_relaxed(base + WDT_WWPS)) & (1 << 3))
		cpu_relax();    

}
static void __wdt_setup(struct my_omap_wdt_dev *wdt)
{
    void __iomem *base = wdt->base;
    mutex_lock(&wdt->lock);
    __wdt_disable(wdt);
	while (readl_relaxed(base + WDT_WWPS) & 0x01)
		cpu_relax();    
    iowrite32((1<<5)|(1<<2),base + WDT_WCLR);
    __wdt_set_timeout(wdt,wdt->wdog.timeout);
    __wdt_reload(wdt);
    __wdt_enable(wdt);
    wdt->wdt_active = true;
    mutex_unlock(&wdt->lock);    
}

static int bbb_wdt_set_timeout(struct watchdog_device *wdog, unsigned int timeout)
{
    struct my_omap_wdt_dev *wdt = watchdog_get_drvdata(wdog);
    mutex_unlock(&wdt->lock);
    __wdt_disable(wdt);
    __wdt_set_timeout(wdt,timeout);
    __wdt_enable(wdt);
    __wdt_reload(wdt);
    wdog->timeout = timeout;
    mutex_lock(&wdt->lock);  
    pr_info("Set timeout : %d \n",timeout); 
    return 0;
}
static int bbb_wdt_ping(struct watchdog_device *wdog)
{
    struct my_omap_wdt_dev *wdt = watchdog_get_drvdata(wdog);
    mutex_unlock(&wdt->lock);
    __wdt_reload(wdt);
    mutex_lock(&wdt->lock);
    return 0;
}

static int bbb_wdt_start(struct watchdog_device *wdog)
{
    struct my_omap_wdt_dev *wdt = watchdog_get_drvdata(wdog);    
    pm_runtime_get_sync(wdt->dev);
    if(true == wdt->wdt_active)
    {
        bbb_wdt_set_timeout(wdog,wdog->timeout);
	pr_info("set time out wdt\n");
    }
     else
        __wdt_setup(wdt);
    pr_info("Start Watchdog Timer with value timer out : %d\n",wdog->timeout);
    return bbb_wdt_ping(wdog);   
}
static int bbb_wdt_stop(struct watchdog_device *wdog)
{
    struct my_omap_wdt_dev *wdt = watchdog_get_drvdata(wdog);
    mutex_lock(&wdt->lock);
    bbb_wdt_ping(wdog);
    wdt->wdt_active = false;
    __wdt_disable(wdt);
    mutex_unlock(&wdt->lock);
    pr_info("Stop Watchdog Timer \n");
    return 0;
}
static const struct watchdog_info omap_wdt_info = {
	.options = WDIOF_SETTIMEOUT | WDIOF_MAGICCLOSE | WDIOF_KEEPALIVEPING,
	.identity = "BBB Watchdog",
};

static const struct watchdog_ops bbb_wdt_ops = {
	.owner          = THIS_MODULE,
	.start          = bbb_wdt_start,
	.stop           = bbb_wdt_stop,
	.ping           = bbb_wdt_ping,
	.set_timeout    = bbb_wdt_set_timeout,
};
static int bbb_wdt_probe(struct platform_device *pdev)
{

    struct resource *res;
    struct my_omap_wdt_dev *wdt = NULL;
    int ret = 0;    
    wdt = devm_kzalloc(&pdev->dev,sizeof(*wdt),GFP_KERNEL);
    if (!wdt)
    {
        return -ENOMEM;     
    }
    wdt->dev = &pdev->dev;
    wdt->wdt_trig = 0x2901;
    wdt->wdt_active = false;
    mutex_init(&wdt->lock);
    res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
    if (res == NULL)
    {
        pr_emerg("error get resource \n");
    }
    wdt->base = devm_ioremap_resource(&pdev->dev,res);
    if (IS_ERR(wdt->base))
	return PTR_ERR(wdt->base);
    wdt->wdog.info = &omap_wdt_info;
    wdt->wdog.ops = &bbb_wdt_ops;
    wdt->wdog.min_timeout = TIMER_MARGIN_MIN;
    wdt->wdog.max_timeout = TIMER_MARGIN_MAX;
    wdt->wdog.parent = &pdev->dev;

    if (watchdog_init_timeout(&wdt->wdog,timeout,&pdev->dev)<0)           
    {
        wdt->wdog.timeout = TIMER_MARGIN_DEFAULT;
    }
    watchdog_set_nowayout(&wdt->wdog, nowayout);
    pm_runtime_enable(wdt->dev);
    pm_runtime_get_sync(wdt->dev);
    platform_set_drvdata(pdev,wdt);
    watchdog_set_drvdata(&wdt->wdog,wdt);
    ret = watchdog_register_device(&wdt->wdog);
    if (ret)
    {
        pm_runtime_disable(wdt->dev);
	pr_info("error unregister watchdog timer");
        return ret; 
    }
    pr_info("BBB Watchdog Timer Rev 0x%x: initial timeout %d sec\n",readl_relaxed(wdt->base + WDT_WIDR) & 0xFF,wdt->wdog.timeout);
    pr_info("Watchdog timer registered \n");
    __wdt_disable(wdt);
    pm_runtime_put(wdt->dev);
    return 0;
}
static int bbb_wdt_remove(struct platform_device *pdev)
{
    struct my_omap_wdt_dev *wdt = platform_get_drvdata(pdev);
    __raw_writel(0xAAAA,wdt->base + WDT_WSPR);
    udelay(10);
    __raw_writel(0x5555,wdt->base + WDT_WSPR);
    pm_runtime_disable(wdt->dev);
    watchdog_unregister_device(&wdt->wdog);
    pr_info("Watchdog timer unregistered \n");
    return 0;
}

static const struct of_device_id bbb_wdt_of_match[] = {
    {.compatible = "huynhtai,omap3-wdt",},
    {},
};
MODULE_DEVICE_TABLE(of, bbb_wdt_of_match);
static struct platform_driver bbb_wdt_driver = {
    .probe  = bbb_wdt_probe,
    .remove = bbb_wdt_remove,
    .driver ={
        .name   = "bbb_watchdog",
        .of_match_table = bbb_wdt_of_match,
    },     
};
module_platform_driver(bbb_wdt_driver);
MODULE_AUTHOR("Huynh Tai");
MODULE_LICENSE("GPL");
