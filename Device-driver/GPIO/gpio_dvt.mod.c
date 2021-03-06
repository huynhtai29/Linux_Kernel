#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x7ef3190f, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x641b7b9c, __VMLINUX_SYMBOL_STR(platform_driver_unregister) },
	{ 0x7952ec47, __VMLINUX_SYMBOL_STR(__platform_driver_register) },
	{ 0xf54cbf82, __VMLINUX_SYMBOL_STR(devm_kfree) },
	{ 0x62697525, __VMLINUX_SYMBOL_STR(pm_runtime_enable) },
	{ 0xd80c3e93, __VMLINUX_SYMBOL_STR(devm_kmalloc) },
	{ 0x7d78e2b3, __VMLINUX_SYMBOL_STR(of_match_device) },
	{ 0xada832f5, __VMLINUX_SYMBOL_STR(dev_err) },
	{ 0x370706ed, __VMLINUX_SYMBOL_STR(gpiochip_add_data) },
	{ 0x62581107, __VMLINUX_SYMBOL_STR(devm_ioremap_resource) },
	{ 0x8d04b6, __VMLINUX_SYMBOL_STR(platform_get_resource) },
	{ 0x6206b84e, __VMLINUX_SYMBOL_STR(__pm_runtime_disable) },
	{ 0x73072c10, __VMLINUX_SYMBOL_STR(gpiochip_remove) },
	{ 0xe261727c, __VMLINUX_SYMBOL_STR(__pm_runtime_idle) },
	{ 0x6d549e6, __VMLINUX_SYMBOL_STR(pinctrl_free_gpio) },
	{ 0x2e5810c6, __VMLINUX_SYMBOL_STR(__aeabi_unwind_cpp_pr1) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x822137e2, __VMLINUX_SYMBOL_STR(arm_heavy_mb) },
	{ 0x5828c9d0, __VMLINUX_SYMBOL_STR(__pm_runtime_resume) },
	{ 0x90734311, __VMLINUX_SYMBOL_STR(gpiochip_get_data) },
	{ 0xb1ad28e0, __VMLINUX_SYMBOL_STR(__gnu_mcount_nc) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

MODULE_ALIAS("of:N*T*Chuynhtai,omap4-gpio");
MODULE_ALIAS("of:N*T*Chuynhtai,omap4-gpioC*");
