#!/bin/sh

ulimit -n 1024000

#set -x
#set -e

read_failed_count=0
write_failed_count=0
differ_count=0
total=600

read_failed () {
    echo "[Read Failed] $@"
    read_failed_count=$(($read_failed_count+1))
}

write_failed () {
    echo "[Write Failed] $@"
    write_failed_count=$(($write_failed_count+1))
}

differ () {
	echo "[Differ] $@"
    differ_count=$(($differ_count+1))
}

summary () {
    echo "## summary ##"
    echo "total: $total"
    echo "read failed: $read_failed_count"
    echo "write failed: $write_failed_count"
    echo "differ: $differ_count"
}

count=1

    for mtd_dev in $(cat /proc/mtd | awk -F ":" '/mtd/ { print $1 }'); do

		if [ $mtd_dev = "mtd5" ]; then
        	info_path="/sys/devices/virtual/block/mtdblock${mtd_dev##mtd}"
			idat1="/usr/share/nanddata/idat2"
        	dev_path="/dev/${mtd_dev}"
        	name=${mtd_dev}
        	size=$(($(cat "${info_path}/size")))*512
			erasesize=16384

			num=256

			for i in $(seq 0 $(($size/($erasesize*$num)-1))); do
				dd if=$idat1 bs=$erasesize count=$num 2&>/dev/null | rk_flash -w -f $dev_path -s $((($erasesize*$num/512)*i)) || write_failed "$count $name $i"
				sync
			done
		fi

    done

summary

