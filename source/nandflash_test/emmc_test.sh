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
while [ $count -le $total ]; do
	echo -e ${count},

    for mtd_dev in $(cat /proc/mtd | awk -F ":" '/mtd/ { print $1 }'); do

        info_path="/sys/devices/virtual/block/mtdblock${mtd_dev##mtd}"
        dev_path="/dev/${mtd_dev}"
        name=${mtd_dev}
        size=$(($(cat "${info_path}/size")))
        #erasesize=$(($(cat "${info_path}/erasesize")))
		erasesize=16384

		num=1
		dd if=/dev/urandom of=/tmp/idat bs=$erasesize count=$num 2&>/dev/null

		for i in $(seq 0 $(($size/($erasesize*$num)-1))); do
			dd if=/tmp/idat bs=$erasesize count=$num 2&>/dev/null | rk_flash -w -f $dev_path -s $((($erasesize*$num/512)*i)) || write_failed "$count $name $i"
			sync
			rk_flash -r $(($erasesize*$num/512)) -f $dev_path -s $((($erasesize*$num/512)*i)) | dd of=/tmp/odat bs=$erasesize count=$num 2&>/dev/null || read_failed "$count $name $i"
			sync
			diff /tmp/idat /tmp/odat 2&>/dev/null || differ "$count $name $i"
		done

    done

    count=$(($count+1))

done

summary

