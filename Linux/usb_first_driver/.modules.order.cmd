cmd_/home/mboris/projects/usb_first_driver/modules.order := {   echo /home/mboris/projects/usb_first_driver/usb1.ko; :; } | awk '!x[$$0]++' - > /home/mboris/projects/usb_first_driver/modules.order
