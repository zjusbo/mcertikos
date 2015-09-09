#!/bin/sh

echo install MBR
sudo dd if=obj/boot/boot0 of=$1 bs=446 count=1
echo install VBR
sudo dd if=obj/boot/boot1 of=$1 bs=512 count=1 seek=64
echo install sector 2 - sector 63
sudo dd if=obj/boot/boot2 of=$1 bs=512 count=62 seek=1
echo install loader
sudo mount $2 /mnt/
sudo mkdir -p /mnt/boot
sudo cp obj/boot/loader /mnt/boot/
sudo umount /mnt/
echo all done!
