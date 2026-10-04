#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "process_monitor"
#define CLASS_NAME "process_monitor_class"

static int major_number;
static struct class *process_class;
static struct device *process_device;

static int device_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "Process Monitor Driver: Device opened\n");
    return 0;
}

static int device_release(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "Process Monitor Driver: Device closed\n");
    return 0;
}

static ssize_t device_read(struct file *file,
                           char __user *buffer,
                           size_t length,
                           loff_t *offset)
{
    char message[] = "Linux Process Monitor Driver is active\n";
    int message_length = strlen(message);

    if (*offset >= message_length)
        return 0;

    if (copy_to_user(buffer, message, message_length))
        return -EFAULT;

    *offset = message_length;

    return message_length;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = device_open,
    .read = device_read,
    .release = device_release,
};

static int __init process_driver_init(void)
{
    printk(KERN_INFO "Process Monitor Driver: Initializing\n");

    major_number = register_chrdev(0, DEVICE_NAME, &fops);

    if (major_number < 0) {
        printk(KERN_ALERT "Failed to register character device\n");
        return major_number;
    }

    process_class = class_create(CLASS_NAME);

    if (IS_ERR(process_class)) {
        unregister_chrdev(major_number, DEVICE_NAME);
        return PTR_ERR(process_class);
    }

    process_device = device_create(
        process_class,
        NULL,
        MKDEV(major_number, 0),
        NULL,
        DEVICE_NAME
    );

    if (IS_ERR(process_device)) {
        class_destroy(process_class);
        unregister_chrdev(major_number, DEVICE_NAME);
        return PTR_ERR(process_device);
    }

    printk(KERN_INFO "Process Monitor Driver: Loaded successfully\n");

    return 0;
}

static void __exit process_driver_exit(void)
{
    device_destroy(process_class, MKDEV(major_number, 0));
    class_destroy(process_class);
    unregister_chrdev(major_number, DEVICE_NAME);

    printk(KERN_INFO "Process Monitor Driver: Unloaded\n");
}

module_init(process_driver_init);
module_exit(process_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Swastik");
MODULE_DESCRIPTION("Linux Character Device Driver for Process Monitoring");
MODULE_VERSION("1.0");
