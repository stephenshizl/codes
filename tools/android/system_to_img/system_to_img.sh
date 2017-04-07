#!/bin/bash
########################################################################
# File Name: system_to_img.sh
# Author: chengdong
# mail: zchengdongdong@gmail.com
# Created Time: Wed 03 Dec 2014 02:39:39 PM CST
########################################################################

image=./image/ubuntu.img

ssh root@172.0.0.116 'dd if=/dev/mmcblk0p1' | dd of=$image

e2fsck -yf $image

resize2fs -M $image
