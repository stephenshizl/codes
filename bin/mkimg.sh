#!/bin/bash
########################################################################
# File Name: mkimg.sh
# Author: chengdong
# mail: zchengdongdong@gmail.com
# Created Time: Fri 25 Dec 2015 02:26:45 PM CST
########################################################################
toot_dir=out/host/linux-x86/bin
out_dir=out/target/product/wing-k70

system=
boot=
ramdisk=
recovery=

ARGS="$*"

for option in $ARGS
do 
	#echo $option
	case $option in
	system)
		system=1
		;;
	boot)
		boot=1
		;;
	recovery)
		recovery=1
		;;
	ramdisk)
		ramdisk=1
		;;
	esac
done

if [ ! -z $ramdisk ] || [ ! -z $boot ]; then
	echo 
	echo "create ramdisk... "
	$toot_dir/mkbootfs $out_dir/root | $toot_dir/minigzip > $out_dir/ramdisk.img
	echo "ramdisk: $out_dir/ramdisk.img"
fi

if [ ! -z $boot ]; then
	echo 
	echo "create boot... "
	$toot_dir/mkbootimg --kernel $out_dir/kernel --ramdisk $out_dir/ramdisk.img --base 0x40000000 --output $out_dir/boot.img
	echo "boot   : $out_dir/boot.img"
fi

if [ ! -z $recovery ]; then
	echo 
	echo "create recovery... "
fi

if [ ! -z $system ]; then
	echo 
	echo "create system... "
fi
