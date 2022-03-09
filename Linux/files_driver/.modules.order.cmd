cmd_/home/mboris/projects/files_driver/modules.order := {   echo /home/mboris/projects/files_driver/files.ko; :; } | awk '!x[$$0]++' - > /home/mboris/projects/files_driver/modules.order
