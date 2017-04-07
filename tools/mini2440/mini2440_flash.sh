#!/bin/bash
########################################################################
# File Name: /home/june/mini2440_cdrom/transplant/ssflash.sh
# Author: chengdong
# mail: zchengdongdong@gmail.com
# Created Time: Sat 30 Apr 2016 10:02:29 PM CST
########################################################################
cd images/linux

sudo chmod 777 /dev/secbulk0
sudo insmod ~/codes/init.d/kofile/secbulk.ko

# Enter your selection: x
# format NAND FLASH for Linux

# Enter your selection: v
dnw -a 0x0 supervivi-128M 

# Enter your selection: k
dnw -a 0x00060000 zImage_T35 

# Enter your selection: y
dnw -a 0x560000 root_qtopia-128M.img 

