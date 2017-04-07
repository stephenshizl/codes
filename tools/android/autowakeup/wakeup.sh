########################################################################
# File Name: wakeup.sh
# Author: chengdong
# mail: zchengdongdong@gmail.com
# Created Time: Fri 10 Oct 2014 11:32:09 AM CST
########################################################################
#!/bin/bash

#在shell 脚本中嵌入二进制可执行程序 
# --->导入二进制程序busybox添加到shell脚本末尾
#cat /bin/busybox  >>  wakeup.sh
#if [ ! -e /system/bin/busybox ]; then
	## --->导出二进制程序
	#sed "1,/^### END OF THE SCRIPT###/d" "$0" >  /system/bin/busybox
#fi

if [ ! -e /system/bin/busybox ]; then
	mount -o remount,rw /system
	[ -f /sdcard/busybox ] && cp /sdcard/busybox /system/bin/busybox
	[ -f ./busybox ] && cp busybox /system/bin/busybox
	chmod 777 /system/bin/busybox
fi

[ ! -e /system/bin/rtcwake ] && ln -s /system/bin/busybox /system/bin/rtcwake
[ ! -e /system/bin/usleep ] && ln -s /system/bin/busybox /system/bin/usleep

while true; do

	rtcwake -s 5
	echo mem > /sys/power/autosleep

	while read line
	do
		for word in $line; do
			echo $word > /sys/power/wake_unlock
		done
	done < /sys/power/wake_lock

	usleep 50000

	echo "user.wakelock" > /sys/power/wake_lock
	echo off > /sys/power/autosleep

	input keyevent 26
	sleep 3

done
