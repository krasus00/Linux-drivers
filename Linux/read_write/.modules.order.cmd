cmd_/home/mboris/projects/read_write/modules.order := {   echo /home/mboris/projects/read_write/read_write.ko; :; } | awk '!x[$$0]++' - > /home/mboris/projects/read_write/modules.order
