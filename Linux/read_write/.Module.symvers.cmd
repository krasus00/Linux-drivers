cmd_/home/mboris/projects/read_write/Module.symvers := sed 's/\.ko$$/\.o/' /home/mboris/projects/read_write/modules.order | scripts/mod/modpost -m -a  -o /home/mboris/projects/read_write/Module.symvers -e -i Module.symvers   -T -