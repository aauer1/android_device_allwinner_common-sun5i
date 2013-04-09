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
	{ 0xadf42bd5, "__request_region" },
	{ 0xb58b332d, "cdev_del" },
	{ 0x4b3094f6, "kmalloc_caches" },
	{ 0x12da5bb2, "__kmalloc" },
	{ 0xa4045b03, "cdev_init" },
	{ 0xfbc74f64, "__copy_from_user" },
	{ 0xab455eb0, "up_read" },
	{ 0x528c709d, "simple_read_from_buffer" },
	{ 0x92eb0db4, "debugfs_create_dir" },
	{ 0xa0125406, "mem_map" },
	{ 0xd6ee688f, "vmalloc" },
	{ 0x3ec8886f, "param_ops_int" },
	{ 0x67c2fa54, "__copy_to_user" },
	{ 0x2e5810c6, "__aeabi_unwind_cpp_pr1" },
	{ 0x788fe103, "iomem_resource" },
	{ 0xd8e484f0, "register_chrdev_region" },
	{ 0xbed60566, "sub_preempt_count" },
	{ 0xb0bb9c02, "down_interruptible" },
	{ 0x7f9ae622, "device_destroy" },
	{ 0xb1ad28e0, "__gnu_mcount_nc" },
	{ 0x7485e15e, "unregister_chrdev_region" },
	{ 0x999e8297, "vfree" },
	{ 0x39229ead, "debugfs_create_file" },
	{ 0x4c63c69e, "debugfs_remove_recursive" },
	{ 0x36ffbf67, "__alloc_pages_nodemask" },
	{ 0xc6d533e1, "down_read" },
	{ 0xe707d823, "__aeabi_uidiv" },
	{ 0xfa2a45e, "__memzero" },
	{ 0x5f754e5a, "memset" },
	{ 0x27e1a049, "printk" },
	{ 0xbf8ba54a, "vprintk" },
	{ 0x4c6ff041, "add_preempt_count" },
	{ 0xb3efc72d, "down" },
	{ 0xc1936a5e, "device_create" },
	{ 0x43b0c9c3, "preempt_schedule" },
	{ 0xadb3d81e, "up_write" },
	{ 0xb1b22911, "down_write" },
	{ 0xc18e9ab1, "contig_page_data" },
	{ 0xb4e41ea5, "cdev_add" },
	{ 0xc8e00d0, "__free_pages" },
	{ 0x4ab2789f, "___dma_page_cpu_to_dev" },
	{ 0xbc5196d2, "___dma_page_dev_to_cpu" },
	{ 0x6b2dc060, "dump_stack" },
	{ 0xc27487dd, "__bug" },
	{ 0x9bce482f, "__release_region" },
	{ 0x6bb5f288, "kmem_cache_alloc_trace" },
	{ 0x425456c9, "cpu_cache" },
	{ 0x37a0cba, "kfree" },
	{ 0x73f4421, "remap_pfn_range" },
	{ 0x9d669763, "memcpy" },
	{ 0x8cf51d15, "up" },
	{ 0xd63762d0, "class_destroy" },
	{ 0x701d0ebd, "snprintf" },
	{ 0xc3fe87c8, "param_ops_uint" },
	{ 0xdb2f99f4, "__class_create" },
	{ 0xaa94c694, "__init_rwsem" },
	{ 0x29537c9e, "alloc_chrdev_region" },
	{ 0x81799cee, "vscnprintf" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "60464A1F049A2AB047E1B58");
