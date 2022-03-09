/*Чтобы понять работу программы читай снизу вверх*/

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>

static struct usb_device *device;

static int pen_probe(struct usb_interface *interface, const struct usb_device_id *id) //4) система определяет интерфейс для данного устройства
{
    struct usb_host_interface *iface_desc; //создаём указатели на структуры, в которые занесём данные, определённые системой в структуре usb_interface
    struct usb_endpoint_descriptor *endpoint;//создаём указатели на структуры, в которые занесём данные, определённые системой в структуре usb_interface
    int i;

    iface_desc = interface->cur_altsetting; //присваиваем значение указателю на использующийся интерфейс
    printk(KERN_INFO "Pen i/f %d now probed: (%04X:%04X)\n",
            iface_desc->desc.bInterfaceNumber, id->idVendor, id->idProduct);//обращаемся к desc (тип usb_interface_descriptor)  берём даные
    printk(KERN_INFO "ID->bNumEndpoints: %02X\n",
            iface_desc->desc.bNumEndpoints); // количество оконечных точек
    printk(KERN_INFO "ID->bInterfaceClass: %02X\n",
            iface_desc->desc.bInterfaceClass); //тип интерфейса

    for (i = 0; i < iface_desc->desc.bNumEndpoints; i++) //начинаем цикл и проходим по всем оконечным точкам
    {
        endpoint = &iface_desc->endpoint[i].desc; //записываем в наш указатель адрес описания i-й оконечной точки

        printk(KERN_INFO "ED[%d]->bEndpointAddress: 0x%02X\n",
                i, endpoint->bEndpointAddress); //выводим адрес оконечной точки
        printk(KERN_INFO "ED[%d]->bmAttributes: 0x%02X\n",
                i, endpoint->bmAttributes);//выводим параметры
        printk(KERN_INFO "ED[%d]->wMaxPacketSize: 0x%04X (%d)\n",
                i, endpoint->wMaxPacketSize, endpoint->wMaxPacketSize); //максимальный размер посылки
    }

    device = interface_to_usbdev(interface); //предоставляем интерфейс
    return 0;
}

static void pen_disconnect(struct usb_interface *interface)
{
    printk(KERN_INFO "Pen i/f %d now disconnected\n", //5) при отключении устройства
            interface->cur_altsetting->desc.bInterfaceNumber);
}

static struct usb_device_id pen_table[] = //Определяем уникальные номера устройства
{
    { USB_DEVICE(0x13fe, 0x4200) },
    {} /* Terminating entry */
};

MODULE_DEVICE_TABLE (usb, pen_table);  //Вносим наше устройство в реестр устройств, так сказатб

static struct usb_driver pen_driver = // 3) Какое устройство регистрируем?
{
    .name = "pen_driver", //имя драрйвера в ядре
    .probe = pen_probe, // 4) Что делать при обнаружении устройства
    .disconnect = pen_disconnect, // 5) Что делать при выключении устройства
    .id_table = pen_table, //Так какое всё-таки устройство? (определение старшего и младшего числа)
};

static int __init pen_init(void) //2) регистрация устройства
{
    return usb_register(&pen_driver);
}

static void __exit pen_exit(void)
{
    usb_deregister(&pen_driver);
}

module_init(pen_init); //1) что делаем при загрузке в ядро
module_exit(pen_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Anil Kumar Pugalia <email@sarika-pugs.com>");
MODULE_DESCRIPTION("USB Pen Info Driver");
