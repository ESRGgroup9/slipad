#!/bin/sh

echo "Inserting i2c-tools..."
modprobe i2c-bcm2835
modprobe i2c-dev


echo "Inserting pir DDriver..."
insmod /etc/slipad/pir.ko
echo "Inserting lampf DDriver..."
insmod /etc/slipad/lampf.ko

/etc/slipad/dSensors.elf &

/etc/slipad/localSys.elf
