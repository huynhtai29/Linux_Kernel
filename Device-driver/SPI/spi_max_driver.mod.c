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
	{ 0x6bc3fbc0, __VMLINUX_SYMBOL_STR(__unregister_chrdev) },
	{ 0x9f7a3e33, __VMLINUX_SYMBOL_STR(cdev_del) },
	{ 0x1251a9b3, __VMLINUX_SYMBOL_STR(kmalloc_caches) },
	{ 0x217421db, __VMLINUX_SYMBOL_STR(cdev_init) },
	{ 0x2e5810c6, __VMLINUX_SYMBOL_STR(__aeabi_unwind_cpp_pr1) },
	{ 0x91bda9a9, __VMLINUX_SYMBOL_STR(device_destroy) },
	{ 0x6fa7bda3, __VMLINUX_SYMBOL_STR(__spi_register_driver) },
	{ 0xb1ad28e0, __VMLINUX_SYMBOL_STR(__gnu_mcount_nc) },
	{ 0x28cc25db, __VMLINUX_SYMBOL_STR(arm_copy_from_user) },
	{ 0x7485e15e, __VMLINUX_SYMBOL_STR(unregister_chrdev_region) },
	{ 0x5f3936b7, __VMLINUX_SYMBOL_STR(spi_setup) },
	{ 0xfa2a45e, __VMLINUX_SYMBOL_STR(__memzero) },
	{ 0x5f754e5a, __VMLINUX_SYMBOL_STR(memset) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0xe1537255, __VMLINUX_SYMBOL_STR(__list_del_entry_valid) },
	{ 0xa60f790b, __VMLINUX_SYMBOL_STR(driver_unregister) },
	{ 0xb7f96077, __VMLINUX_SYMBOL_STR(device_create) },
	{ 0x68f31cbd, __VMLINUX_SYMBOL_STR(__list_add_valid) },
	{ 0x12699bf5, __VMLINUX_SYMBOL_STR(spi_sync) },
	{ 0x4b1552f8, __VMLINUX_SYMBOL_STR(cdev_add) },
	{ 0xdb7305a1, __VMLINUX_SYMBOL_STR(__stack_chk_fail) },
	{ 0x51521308, __VMLINUX_SYMBOL_STR(kmem_cache_alloc_trace) },
	{ 0x1e047854, __VMLINUX_SYMBOL_STR(warn_slowpath_fmt) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0x9491b702, __VMLINUX_SYMBOL_STR(class_destroy) },
	{ 0x8f678b07, __VMLINUX_SYMBOL_STR(__stack_chk_guard) },
	{ 0x339b7a65, __VMLINUX_SYMBOL_STR(__class_create) },
	{ 0x29537c9e, __VMLINUX_SYMBOL_STR(alloc_chrdev_region) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

MODULE_ALIAS("of:N*T*Chuynhtai,spi1");
MODULE_ALIAS("of:N*T*Chuynhtai,spi1C*");
