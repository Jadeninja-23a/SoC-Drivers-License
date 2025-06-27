#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

MODULE_AUTHOR("Videep Reddy Jalapally");
MODULE_DESCRIPTION("Prints Hello Linux, my love >3");
MODULE_LICENSE("Dual MIT/GPL");
MODULE_VERSION("0.1");

static int __init my_first_module_init(void)
{
	printk(KERN_INFO "Hello Linux, my love >3");
	return 0;
}

static void  __exit my_first_module_exit(void)
{
}

module_init(my_first_module_init);
module_exit(my_first_module_exit);
