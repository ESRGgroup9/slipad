#!/bin/sh

echo "Inserting i2c-tools..."
modprobe i2c-bcm2835
modprobe i2c-dev


echo "Inserting pir DDriver..."
insmod pir.ko
echo "Inserting lampf DDriver..."
insmod lampf.ko
