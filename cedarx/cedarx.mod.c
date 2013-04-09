#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
 .name = KBUILD_MODNAME,
 .init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
 .exit = cleanup_module,
#endif
 .arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x3ad6e03c, "module_layout" },
	{ 0xb58b332d, "cdev_del" },
	{ 0x4b3094f6, "kmalloc_caches" },
	{ 0x65e75cb6, "__list_del_entry" },
	{ 0xa4045b03, "cdev_init" },
	{ 0xc93c111c, "clk_reset" },
	{ 0xfbc74f64, "__copy_from_user" },
	{ 0x9f8df4c7, "clk_enable" },
	{ 0x4c1371d0, "ve_start" },
	{ 0x3ec8886f, "param_ops_int" },
	{ 0x67c2fa54, "__copy_to_user" },
	{ 0x2e5810c6, "__aeabi_unwind_cpp_pr1" },
	{ 0xd8e484f0, "register_chrdev_region" },
	{ 0xc8b57c27, "autoremove_wake_function" },
	{ 0x4ec2f8f7, "clk_disable" },
	{ 0x22bde45f, "no_llseek" },
	{ 0xbed60566, "sub_preempt_count" },
	{ 0xb0bb9c02, "down_interruptible" },
	{ 0x7f9ae622, "device_destroy" },
	{ 0x25aed440, "clk_put" },
	{ 0x3028e855, "init_timer_key" },
	{ 0xae8e2c8f, "ve_size" },
	{ 0x7485e15e, "unregister_chrdev_region" },
	{ 0x71a375d9, "clk_get_rate" },
	{ 0xc7024530, "nonseekable_open" },
	{ 0x7d11c268, "jiffies" },
	{ 0x343a1a8, "__list_add" },
	{ 0xf6288e02, "__init_waitqueue_head" },
	{ 0xe707d823, "__aeabi_uidiv" },
	{ 0xfa2a45e, "__memzero" },
	{ 0x27e1a049, "printk" },
	{ 0x4c6ff041, "add_preempt_count" },
	{ 0x521445b, "list_del" },
	{ 0xc1936a5e, "device_create" },
	{ 0x1298ac43, "mod_timer" },
	{ 0xd6b8e852, "request_threaded_irq" },
	{ 0x3ed5495e, "platform_driver_register" },
	{ 0x43b0c9c3, "preempt_schedule" },
	{ 0xb4e41ea5, "cdev_add" },
	{ 0xc140cd50, "kmem_cache_alloc" },
	{ 0x76327f51, "platform_device_register" },
	{ 0x3bd1b1f6, "msecs_to_jiffies" },
	{ 0xd62c833f, "schedule_timeout" },
	{ 0xd780a450, "clk_set_parent" },
	{ 0x26256178, "clk_get" },
	{ 0xdb76df02, "clk_set_rate" },
	{ 0xb9e52429, "__wake_up" },
	{ 0x37a0cba, "kfree" },
	{ 0x73f4421, "remap_pfn_range" },
	{ 0x75a17bed, "prepare_to_wait" },
	{ 0xf90439f2, "send_sig_info" },
	{ 0x8cf51d15, "up" },
	{ 0xd63762d0, "class_destroy" },
	{ 0x45a55ec8, "__iounmap" },
	{ 0x8893fa5d, "finish_wait" },
	{ 0x40a6f522, "__arm_ioremap" },
	{ 0xefd6cf06, "__aeabi_unwind_cpp_pr0" },
	{ 0x38dc6bcd, "platform_driver_unregister" },
	{ 0xdb2f99f4, "__class_create" },
	{ 0x29537c9e, "alloc_chrdev_region" },
	{ 0xf20dabd8, "free_irq" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

