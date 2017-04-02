#!/bin/bash
make -C /lib/modules/$(uname -r)/build M=$PWD modules
sudo rm /dev/conv
sudo rmmod conv 
sudo insmod ./conv.ko
#cat /proc/devices | egrep conv  ; to verify if driver is loaded   
#less /var/log/syslog    ; to see printk
major=$( cat /proc/devices | grep char_conv  |  sed 's/[^0-9]*//g' )
sudo mknod -m 666 /dev/conv c $major 0
