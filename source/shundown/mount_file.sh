#!/system/bin/sh

while true
do
	mount | grep "\/mnt\/sdcard"
	if [ $? -eq 0 ] ; then
		if [ ! -d "/sdcard/iflytek" ] || [ ! -d "/data/skyres" ] ; then
			cp -rf /mnt/skyres/iflytek /mnt/sdcard/
			sync
			mkdir /data/skyres
		fi
		#exit 0
	fi
	ps | grep "com.skyworthauto.launcher"
	if [ $? -eq 0 ] ; then
		sleep 15
		monkey --ignore-crashes --ignore-timeouts --kill-process-after-error --ignore-security-exceptions --throttle 1000 --pct-syskeys 0 -v -v -v -s 5 1000000
		#echo "c" > /proc/sysrq-trigger
	fi
	sleep 2
done
