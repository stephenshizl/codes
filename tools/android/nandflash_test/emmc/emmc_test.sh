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

main () {
idat1=/usr/share/nanddata/idat1
idat2=/usr/share/nanddata/idat2
differ_idat1=0
differ_idat2=0
for mtd_dev in $(cat /proc/mtd | awk -F ":" '/mtd/ { print $1 }'); do
	if [ $mtd_dev = "mtd5" ] || [ $mtd_dev = "mtd6" ] || [ $mtd_dev = "mtd7" ]; then 
		info_path="/sys/devices/virtual/block/mtdblock${mtd_dev##mtd}"
		dev_path="/dev/${mtd_dev}"
		name=${mtd_dev}
		size=$(($(cat "${info_path}/size")))
		erasesize=16384
		diff_file=$idat1
		write_file=$idat2
		num=1

		rk_flash -r $(($erasesize*$num/512)) -f $dev_path -s $((($erasesize*$num/512)*0)) | dd of=/tmp/odat bs=$erasesize count=$num 

		diff $idat1 /tmp/odat 2&>/dev/null || differ_idat1=1
		diff $idat2 /tmp/odat 2&>/dev/null || differ_idat2=1

		echo $differ_idat1
		echo $differ_idat2

		if [ $differ_idat1 = 1 ] && [ $differ_idat2 = 1 ];then
			echo "first file differ"
			return
		fi 

		if [ $differ_idat1 = 1 ] && [ $differ_idat2 = 0 ]; then
			diff_file=$idat2
			write_file=$idat1
		fi
		echo $write_file
		echo $diff_file
		for i in $(seq 0 $(($size/($erasesize*$num)-1))); do
			rk_flash -r $(($erasesize*$num/512)) -f $dev_path -s $((($erasesize*$num/512)*i)) | dd of=/tmp/odat bs=$erasesize count=$num 2&>/dev/null || read_failed "$count $name $i"
			sync
			
			error=0
			diff $diff_file /tmp/odat 2&>/dev/null || error=1
			if [ $error = 1 ]; then
				differ "$count $name $i"
				return
			fi
		done

		for j in $(seq 0 $(($size/($erasesize*$num)-1))); do
			dd if=$write_file bs=$erasesize count=$num 2&>/dev/null | rk_flash -w -f $dev_path -s $((($erasesize*$num/512)*i)) || write_failed "$count $name $i"
			sync
		done
		reboot
	fi
done

}
main
