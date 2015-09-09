#!/bin/sh

sudo losetup -o `expr 2048 \* 512` /dev/loop1 obj/certikos.img
sudo mount /dev/loop1 /mnt
