#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>

#define MIN(a,b) (((a) <= (b)) ? (a) : (b))
#define BULK_EP_OUT 0x02 //Адрес выходной оконечной точки
#define BULK_EP_IN 0x81 //Адрес выходной оконечной точки
#define MAX_PKT_SIZE 512 //  Максимальный пакет данных

static struct usb_device *device; //объявляем прототипы
static struct usb_class_driver class; //необходимо для файловых операций
static unsigned char bulk_buf[MAX_PKT_SIZE]; //максимальный размер данных

static int pen_open(struct inode *i, struct file *f) //открытие файла
{
    return 0;
}
static int pen_close(struct inode *i, struct file *f) //закрытие файла
{
    return 0;
}
static ssize_t pen_read(struct file *f, char __user *buf, size_t cnt, loff_t *off) //читаем файл
{
    int retval;
    int read_cnt;  //сколько прчитали?

    /* Read the data from the bulk endpoint */ //читаем инфу из поточной оконечной точки
    retval = usb_bulk_msg(device, usb_rcvbulkpipe(device, BULK_EP_IN), //указатель на устройство, фукция, задающая оконечную точку
            bulk_buf, MAX_PKT_SIZE, &read_cnt, 5000);//указатель на массив(куда записываем), размер массива, указатель на запись количества принятых данных, сколько ждём завершения сообщения в тиках
    if (retval) //в случае ошибки
    {
        printk(KERN_ERR "Bulk message returned %d\n", retval);
        return retval;
    }
    if (copy_to_user(buf, bulk_buf, MIN(cnt, read_cnt))) //передача информации из пространства ядра в пространство пользователя
    {
        return -EFAULT;
    }

    return MIN(cnt, read_cnt); //возвращаем количество прочитанных данных
}
static ssize_t pen_write(struct file *f, const char __user *buf, size_t cnt, loff_t *off) //пишем из файла
{
    int retval;
    int wrote_cnt = MIN(cnt, MAX_PKT_SIZE);   //количество записанных байтов

    if (copy_from_user(bulk_buf, buf, MIN(cnt, MAX_PKT_SIZE))) //копируем данные из пространства пользователя в пространство ядра
    {
        return -EFAULT;
    }

    /* Write the data into the bulk endpoint */ //записываем в устройство
    retval = usb_bulk_msg(device, usb_sndbulkpipe(device, BULK_EP_OUT), //указатель на устройство, фукция, задающая оконечную точку
            bulk_buf, MIN(cnt, MAX_PKT_SIZE), &wrote_cnt, 5000); //указатель на массив(откуда записываем), размер массива, указатель на массив записываемых данных, сколько ждём завершения сообщения в тиках
    if (retval) //выводим ошибку, если появлились проблемы
    {
        printk(KERN_ERR "Bulk message returned %d\n", retval);
        return retval;
    }

    return wrote_cnt;
}

static struct file_operations fops = //5) при работе устройства: объявляем файловые операции, которые будем использовать
{
    .open = pen_open, //?????
    .release = pen_close, //?????
    .read = pen_read, //чтение
    .write = pen_write, //запись
};

static int pen_probe(struct usb_interface *interface, const struct usb_device_id *id)  //4) при подключении устройства
{
    int retval;  //объявляем пременную, хранящую значения

    device = interface_to_usbdev(interface); //предоставляем подходящий интерфейс

    class.name = "usb/pen%d"; //называем класс
    class.fops = &fops; //предоставляем доступ для файловых операций
    if ((retval = usb_register_dev(interface, &class)) < 0) //пытаемся зарегать устройство и звписываем в retval результат
    {
        /* Something prevented us from registering this driver */
        err("Not able to get a minor for this device.");
    }
    else
    {
        printk(KERN_INFO "Minor obtained: %d\n", interface->minor); //определяем младшее число
    }

    return retval;
}

static void pen_disconnect(struct usb_interface *interface)//6) удаляем устройство при отключении
{
    usb_deregister_dev(interface, &class);
}

/* Устройства, с которыми работает драйвер*/
static struct usb_device_id pen_table[] =
{
    { USB_DEVICE(0x13fe, 0x4200) },
    {} /* Terminating entry */
};
MODULE_DEVICE_TABLE (usb, pen_table); //внести устройство в таблицу устройств

static struct usb_driver pen_driver = //задаём параметры устройства
{
    .name = "pen_driver", //имя драйвера в системе
    .probe = pen_probe, // 3) Что выполняем при подключении устройства
    .disconnect = pen_disconnect,// 6)Что выполняем при отключении устройства
    .id_table = pen_table, //Собственно, какое устройство обрабатываем-с?
};

static int __init pen_init(void) //1) выполняем при загрузке
{
    int result;

    /* регистрация в субсистеме USB */
    if ((result = usb_register(&pen_driver))) //регистрируем драйвер и присваиваем значение в result и надеемся, что он будет отрицательным
    {
        err("usb_register failed. Error number %d", result); //Если результат не отрицательный, то пишем, что облажались
    }
    return result;
}

static void __exit pen_exit(void)
{
    /* Deregister this driver with the USB subsystem */
    usb_deregister(&pen_driver);
}

module_init(pen_init); //1) загрузка модуля в ядро
module_exit(pen_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Anil Kumar Pugalia <email_at_sarika-pugs_dot_com>");
MODULE_DESCRIPTION("USB Pen Device Driver");
