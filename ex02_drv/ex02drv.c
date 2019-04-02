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
#include <linux/cdev.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/ioctl.h>

#define LIGHT_MAJOR 170

#define LIGHT_MAGIC ';'

#define LIGHT_ON     _IOWR(LIGHT_MAGIC, 0, unsigned char)
#define LIGHT_OFF    _IOWR(LIGHT_MAGIC, 1, unsigned char)


/* struct of device */
struct light_dev {
    struct cdev cdev;     /* char device 'cdev' struct*/
    unsigned char value;  /* 1:light on, 0:light off*/
};


struct light_dev *light_devp;

int light_major = LIGHT_MAJOR;

static void light_on(void)
{
    printk(KERN_INFO "Light --> On\n");
}

static void light_off(void)
{
    printk(KERN_INFO "Light --> Off\n");
}


static void light_gpio_init( void )
{
    printk(KERN_INFO "light GPIO initialize...\n");
}

static int light_open(struct inode *inode, struct file *filp)
{
    struct light_dev *dev;

    /* get device struct pointer */
    dev = container_of(inode->i_cdev, struct light_dev, cdev);
    filp->private_data = dev;

    return 0;
}

static int light_release(struct inode *inode, struct file *filp)
{
    return 0;
}


/* Read device */
static ssize_t light_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
    struct light_dev *dev = filp->private_data; /*get device struct*/

    if( copy_to_user(buf, &(dev->value), 1) )
        return -EFAULT;

    return 1;
}


/* Write device */
static ssize_t light_write(struct file *flip, const char __user *buf, size_t count, loff_t *f_pos)
{
    struct light_dev *dev = flip->private_data;

    if( copy_from_user(&(dev->value), buf, 1) )
        return -EFAULT;
    
    if( dev->value == 1 )
        light_on();
    else
        light_off();

    return 1;
}

/* ioctl funciton */
static long light_unlocked_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    struct light_dev *dev = filp->private_data;

    switch(cmd) {
    case LIGHT_ON:
        dev->value = 1;
        break;
    case LIGHT_OFF:
        dev->value = 0;
        break;
    default: /* Not support command */
        return -ENOTTY;
    }

    return 0;
}


static struct file_operations light_fops = {
    .owner          = THIS_MODULE,
    .read           = light_read,
    .write          = light_write,
    .unlocked_ioctl = light_unlocked_ioctl,
    .open           = light_open,
    .release        = light_release,
};


/* setup character device struct */
static void light_setup_cdev(struct light_dev *dev, int index)
{
    int err, devno = MKDEV(light_major, index);
    cdev_init(&dev->cdev, &light_fops);
    dev->cdev.owner = THIS_MODULE;
    dev->cdev.ops   = &light_fops;
    err = cdev_add(&dev->cdev, devno, 1);
    if( err )
        printk(KERN_NOTICE "Error %d adding LED%d\n", err, index);
}


/* kernel module load */
static int __init light_init(void)
{
    int result;

    dev_t dev = MKDEV(light_major, 0);

    if( light_major )
        result = register_chrdev_region(dev, 1, "LED");
    else {
        result = alloc_chrdev_region(&dev, 0, 1, "LED");
	light_major = MAJOR(dev);
    }

    if(result <0 )
        return result;

    light_devp = kmalloc(sizeof(struct light_dev), GFP_KERNEL);
    if( light_devp == NULL ) {
        result = -ENOMEM;
	goto fail_malloc;
    }

    memset(light_devp, 0, sizeof(struct light_dev));
    light_setup_cdev(light_devp, 0);
    light_gpio_init();

    return 0;

fail_malloc:
    unregister_chrdev_region(dev, 1);
    return result;
}


/* kernel module unload */
static void __exit light_exit(void)
{
    cdev_del(&light_devp->cdev);
    kfree(light_devp);
    unregister_chrdev_region( MKDEV(light_major, 0), 1 );
}

module_init(light_init);
module_exit(light_exit);

MODULE_AUTHOR("dingjun<28059535@qq.com>");
MODULE_DESCRIPTION("light driver example.");
MODULE_LICENSE("GPL v2");
MODULE_ALIAS("leds:gpio");

