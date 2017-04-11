#!/bin/bash
insmod /lib/modules/4.4.48/extra/conv.ko
#cat /proc/devices | egrep conv  ; to verify if driver is loaded   
#less /var/log/syslog    ; to see printk
major=$( cat /proc/devices | grep char_conv  |  sed 's/[^0-9]*//g' )
mknod -m 666 /dev/conv c $major 0
