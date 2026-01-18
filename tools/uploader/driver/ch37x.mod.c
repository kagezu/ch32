#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/export-internal.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

#ifdef CONFIG_UNWINDER_ORC
#include <asm/orc_header.h>
ORC_HEADER;
#endif

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



static const char ____versions[]
__used __section("__versions") =
	"\x18\x00\x00\x00\x68\x22\x0b\xcf"
	"usb_bulk_msg\0\0\0\0"
	"\x10\x00\x00\x00\xba\x0c\x7a\x03"
	"kfree\0\0\0"
	"\x1c\x00\x00\x00\xd6\x80\xee\xef"
	"usb_get_descriptor\0\0"
	"\x1c\x00\x00\x00\xcb\xf6\xfd\xf0"
	"__stack_chk_fail\0\0\0\0"
	"\x28\x00\x00\x00\xb3\x1c\xa2\x87"
	"__ubsan_handle_out_of_bounds\0\0\0\0"
	"\x1c\x00\x00\x00\xed\x32\xb4\xd2"
	"usb_register_driver\0"
	"\x10\x00\x00\x00\x7e\x3a\x2c\x12"
	"_printk\0"
	"\x14\x00\x00\x00\x4b\x8d\xfa\x4d"
	"mutex_lock\0\0"
	"\x28\x00\x00\x00\x04\xf3\x7a\x40"
	"usb_wait_anchor_empty_timeout\0\0\0"
	"\x1c\x00\x00\x00\x7b\xcc\x27\x84"
	"_raw_spin_lock_irq\0\0"
	"\x20\x00\x00\x00\x53\x0f\x75\x4b"
	"_raw_spin_unlock_irq\0\0\0\0"
	"\x20\x00\x00\x00\xe1\x8a\x2c\x96"
	"usb_kill_anchored_urbs\0\0"
	"\x18\x00\x00\x00\xd9\x74\xc0\x23"
	"usb_deregister\0\0"
	"\x1c\x00\x00\x00\x2b\xe4\xa0\xc9"
	"usb_find_interface\0\0"
	"\x24\x00\x00\x00\x87\x2f\x84\x07"
	"usb_autopm_get_interface\0\0\0\0"
	"\x20\x00\x00\x00\x5f\x69\x96\x02"
	"refcount_warn_saturate\0\0"
	"\x1c\x00\x00\x00\x63\xa5\x03\x4c"
	"random_kmalloc_seed\0"
	"\x18\x00\x00\x00\x19\x08\xda\x08"
	"kmalloc_caches\0\0"
	"\x18\x00\x00\x00\x4c\x48\xc3\xd0"
	"kmalloc_trace\0\0\0"
	"\x18\x00\x00\x00\x9f\x0c\xfb\xce"
	"__mutex_init\0\0\0\0"
	"\x20\x00\x00\x00\x54\xea\xa5\xd9"
	"__init_waitqueue_head\0\0\0"
	"\x14\x00\x00\x00\x15\xce\x24\xee"
	"usb_get_dev\0"
	"\x1c\x00\x00\x00\xb1\x90\x18\xfd"
	"usb_register_dev\0\0\0\0"
	"\x14\x00\x00\x00\x62\xcf\x79\x60"
	"_dev_info\0\0\0"
	"\x14\x00\x00\x00\xcb\x61\x3a\xa1"
	"usb_put_dev\0"
	"\x24\x00\x00\x00\x08\x0d\x02\x14"
	"usb_autopm_put_interface\0\0\0\0"
	"\x1c\x00\x00\x00\xd5\x39\x63\xff"
	"usb_deregister_dev\0\0"
	"\x14\x00\x00\x00\xbb\x6d\xfb\xbd"
	"__fentry__\0\0"
	"\x1c\x00\x00\x00\xca\x39\x82\x5b"
	"__x86_return_thunk\0\0"
	"\x18\x00\x00\x00\x38\xf0\x13\x32"
	"mutex_unlock\0\0\0\0"
	"\x24\x00\x00\x00\x75\x08\x94\x89"
	"mutex_lock_interruptible\0\0\0\0"
	"\x18\x00\x00\x00\xe1\xbe\x10\x6b"
	"_copy_to_user\0\0\0"
	"\x18\x00\x00\x00\x9d\x7f\x7e\x16"
	"__get_user_1\0\0\0\0"
	"\x18\x00\x00\x00\xdf\xd3\x29\x67"
	"__get_user_4\0\0\0\0"
	"\x14\x00\x00\x00\x45\x3a\x23\xeb"
	"__kmalloc\0\0\0"
	"\x1c\x00\x00\x00\x48\x9f\xdb\x88"
	"__check_object_size\0"
	"\x18\x00\x00\x00\xc2\x9c\xc4\x13"
	"_copy_from_user\0"
	"\x18\x00\x00\x00\xeb\x7b\x33\xe1"
	"module_layout\0\0\0"
	"\x00\x00\x00\x00\x00\x00\x00\x00";

MODULE_INFO(depends, "");

MODULE_ALIAS("usb:v1A86p55E0d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v4348p55E0d*dc*dsc*dp*ic*isc*ip*in*");

MODULE_INFO(srcversion, "CD4520BAD605C9C6B7DEB09");
