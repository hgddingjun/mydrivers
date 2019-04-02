/*****************************************************************
 *
 *  Dingjun Create 2019.04.01
 *
 *  ex01drv.c  --- first driver exercise, Hello kernel
 *
 *  Study linux kernel drivers
 *
 * ****************************************************************/
#include <linux/init.h>
#include <linux/module.h>

static int __init hello_init(void)
{
    printk(KERN_INFO "Hello linux kernel-4.0 --->[ ^INIT^ ]\n");
    return 0;
}

static void __exit hello_exit(void)
{
    printk(KERN_INFO "Hello linux kernel-4.0 [ vEXITv ]\n");
}

module_init(hello_init);
module_exit(hello_exit);
MODULE_AUTHOR("dingjun<28059535@qq.com>");
MODULE_DESCRIPTION("A simple module.");
MODULE_LICENSE("GPL v2");







