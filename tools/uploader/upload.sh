#!/bin/bash

if [ "$(lsusb -d 4348:55e0)" != "" ] ; then
sudo wch -p /dev/ch37x2 -c $1/tools/uploader/ch32 -o programm -f $1/obj/ch32.hex -r 1

for i in {1..20}
do
sleep 1
if [ "$(lsusb -d 4348:55e0)" != "" ] ; then break
fi
done
fi

if [ "$(lsusb -d 4348:55e0)" != "" ] ; then
sudo wch -p /dev/ch37x2 -c $1/tools/uploader/ch32 -o programm -f $1/obj/ch32.hex -r 0
fi


