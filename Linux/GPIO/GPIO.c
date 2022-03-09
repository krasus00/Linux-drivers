#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>

MODULE_LICENSE("Dual BSD/GPL");
MODULE_DESCRIPTION("Rgister a device nr . and implement some callback functions");

// buffer for data
static char buffer [255];
static int buffer_pointer;

//Variables for device and device claa
static dev_t my_device_nr;
static struct class *my_class;
static struct cdev my_device;


#define DRIVER_NAME  "my_gpio_driver"
#define DRIVER_CLASS "MyModuleClass"


// Read data out of the buffer

static ssize_t driver_read (struct file *File, char *user_buffer, size_t count, loff_t *offs){
  int to_copy, not_copied, delta;
  char tmp[3] = "\n";
  //Get amount of data to copy
  to_copy = min(count, sizeof(tmp));
  //Read value of button
  printk("Value of button: %d \n", gpio_get_value(17));
  tmp[0] = gpio_get_value(17) + '0';

  //Copy data to user return the amount of data which wasn't copied
  not_copied = copy_to_user(user_buffer, &tmp, to_copy);

  //calculate data - number of bytes it has copied
  delta = to_copy - not_copied;
  return delta;
}

//writing
static ssize_t driver_write (struct file *File, const char *user_buffer, size_t count, loff_t *offs){
  int to_copy, not_copied, delta;
  char value;
  //Get amount of data to copy
  to_copy = min (count, sizeof(buffer));

  //Copy data to user return the amount of data which wasn't copied
  not_copied = copy_from_user(&value, user_buffer, to_copy);
  //setting the LED
  switch(value){
      case '0':
        gpio_set_value(4,0);
        break;
      case '1':
        gpio_set_value(4,1);
        break;
      default:
        printk("Invalid Input! \n");
        break;
  }
  //calculate data - number of bytes it has copied
  delta = to_copy - not_copied;

  return delta;
}

 


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
    .release = driver_close,
    .read = driver_read,
    .write = driver_write
  };



static int __init ModuleInit(void)
{
    int retval;
    printk(KERN_ALERT "Hello, world\n"); //printf с приоритетом KERNEL_ALERT
    //allocate device nr
    if (alloc_chrdev_region(&my_device_nr, 0, 1, DRIVER_NAME) <0) {
        printk("Device Nr. could not be alocated!\n");
        return -1;
    }
    printk("read_write - Device Nr. Major: %d, Minor: %d was registered! \n", my_device_nr >> 20, my_device_nr && 0xffff);
    //create a device class
    if ((my_class = class_create(THIS_MODULE, DRIVER_CLASS)) == NULL){
        printk("Device class can not be created! \n");
        goto ClassError;
    }

    //GPIO 4 Initialize
    if(gpio_request(4, "rpi-gpio-4")){
      prink("Can not allocate GPIO 4 \n");
      goto AddError;
    }

    // Set GPIO direction
    if (gpio_direction_output(4,0)) {
      printk ("Can not set GPIO 4 to output! \n");
      goto Gpio4Error;
    }

    //GPIO 17 Initialize
    if(gpio_request(17, "rpi-gpio-17")){
      prink("Can not allocate GPIO 17 \n");
      goto AddError;
    }

    // Set GPIO direction
    if (gpio_direction_output(17,0)) {
      printk ("Can not set GPIO 17 to input! \n");
      goto Gpio17Error;
    }


    //create a device file
    if (device_create(my_class, NULL, my_device_nr, NULL, DRIVER_NAME) == NULL){
        printk ("can not create device file! \n");
        goto FileError;
    }



    //Initialize device File
    cdev_init(&my_device, &fops);

    //Registering device to Kernel
    if(cdev_add(&my_device, my_device_nr, 1) == -1){
        printk("Registering of device to kernel failed! \n");
        goto AddError;
    }

    return 0;

Gpio17Error:
  gpio_free(17);
Gpio4Error:
  gpio_free(4);
AddError:
    device_destroy(my_class, my_device_nr);
FileError:
    class_destroy(my_class);
ClassError:
    unregister_chrdev(my_device_nr, DRIVER_NAME);
    return -1;
}

static void __exit ModuleExit(void)
{
    gpio_set_value(4,0);
    gpio_free(17);
    gpio_free(4);
    cdev_del(&my_device);
    device_destroy(my_class, my_device_nr);
    class_destroy(my_class);
    unregister_chrdev(my_device_nr, DRIVER_NAME);
    printk(KERN_ALERT "Goodbye, cruel world\n"); //printf с приоритетом KERNEL_ALERT
}

module_init(ModuleInit); //вызывается, когда модуль загружается в ядро
module_exit(ModuleExit);//вызывается, когда модуль удаляется
