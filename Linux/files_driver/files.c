#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>

MODULE_LICENSE("Dual BSD/GPL");
MODULE_DESCRIPTION("Rgister a device nr . and implement some callback functions");


// When the device file is opened
static int driver_open (struct inode *device_file, struct file *instance)
{
   printk ("dev_nr - open was called! \n");
   return 0;
}

// When the device file is closed
static int driver_close (struct inode *device_file, struct file *instance)
{
   printk ("dev_nr - close was called! \n");
   return 0;
}

  static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = driver_open,
    .release = driver_close
  };

#define MYMAJOR 90

static int __init ModuleInit(void)
{
    int retval;
     printk(KERN_ALERT "Hello, world\n"); //printf с приоритетом KERNEL_ALERT
    //register device number
    retval = register_chrdev(MYMAJOR, "my_dev_nr", &fops);
    if (retval == 0) {
      printk("dev_nr - registered Device number Major: %d, Minor: %d \n", MYMAJOR, 0);
    }
    if (retval > 0) {
      printk("dev_nr - registered Device number Major: %d, Minor: %d \n", retval >> 20, retval&0xfffff);
    }
    else{
      printk ("Could not register device number! \n");
      return -1;
    }

    return 0;
}

static void __exit ModuleExit(void)
{
    unregister_chrdev(MYMAJOR, "my_dev_nr");
    printk(KERN_ALERT "Goodbye, cruel world\n"); //printf с приоритетом KERNEL_ALERT
}

module_init(ModuleInit); //вызывается, когда модуль загружается в ядро
module_exit(ModuleExit);//вызывается, когда модуль удаляется
