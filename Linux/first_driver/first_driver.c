#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("Dual BSD/GPL");

static int hello_init(void)

{
    printk(KERN_ALERT "Hello, world\n"); //printf с приоритетом KERNEL_ALERT
    return 0;
}

static void hello_exit(void)
{
    printk(KERN_ALERT "Goodbye, cruel world\n"); //printf с приоритетом KERNEL_ALERT
}

module_init(hello_init); //вызывается, когда модуль загружается в ядро
module_exit(hello_exit);//вызывается, когда модуль удаляется
