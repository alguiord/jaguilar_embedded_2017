make -C /lib/modules/$(uname -r)/build M=$PWD modules
sudo insmod ./conv.ko
#cat /proc/devices | egrep conv  ; to verify if driver is loaded   
#less /var/log/syslog    ; to see printk
sudo mknod -m 666 /dev/char_conv c 246 0
#sudo rmmod ex01_simple_module 
