#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xb08d359a, "module_layout" },
	{ 0x3380ab48, "cdev_del" },
	{ 0x6bc3fbc0, "__unregister_chrdev" },
	{ 0x2dfa98e1, "class_destroy" },
	{ 0xa9e56e72, "device_destroy" },
	{ 0x718875ae, "cdev_add" },
	{ 0x69f2abf6, "cdev_init" },
	{ 0x192503d2, "device_create" },
	{ 0x4f695e3a, "__class_create" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0x56470118, "__warn_printk" },
	{ 0x6b10bee1, "_copy_to_user" },
	{ 0x13c49cc2, "_copy_from_user" },
	{ 0x88db9f48, "__check_object_size" },
	{ 0xc5850110, "printk" },
	{ 0xbdfb6dbb, "__fentry__" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "F51B2FAEFC41381797E6820");
