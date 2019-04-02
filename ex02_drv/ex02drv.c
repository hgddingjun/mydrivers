/*****************************************************************
 *
 *  Dingjun Create 2019.04.01
 *
 *  ex02drv.c  --- second driver exercise: GPIO control LED.
 *
 *  Study linux kernel drivers
 *
 * ****************************************************************/
#include <linux/init.h>
#include <linux/module.h>
#include <linux/err.h>
#include <linux/gpio.h>
#include <linux/gpio/consumer.h>
#include <linux/kernel.h>
#include <linux/leds.h>
#include <linux/platform_device.h>
#include <linux/property.h>
#include <linux/slab.h>
#include <linux/workqueue.h>

#define LIGHT_MAJOR 170

struct light_dev {
    struct cdev cdev;     /* char device 'cdev' struct*/
    unsigned char value;  /* 1:light on, 0:light off*/
};


struct light_dev *light_devp;
int light_major = LIGHT_MAJOR;

static int light_open(struct inode *inode, struct file *filp)
{
    struct light_dev *dev;
    dev = container_of(inode->i_cdev, struct light_dev, cdev);
    filp->private_data = dev;
    return 0;
}

static int light_release(struct inode *inode, struct file *filp)
{
    return 0;
}


static int __init light_init(void)
{
    return 0;
}

static void __exit light_exit(void)
{
}

module_init(light_init);
module_exit(light_exit);

MODULE_AUTHOR("dingjun<28059535@qq.com>");
MODULE_DESCRIPTION("A simple module.");
MODULE_LICENSE("GPL v2");
MODULE_ALIAS("leds:gpio");

