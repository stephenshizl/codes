#!/system/bin/sh

#busybox mount -o rw -t ext4 /dev/block/data /data

cd /data/
for file in *
do
	echo $file
	if [ "$file" != "media" ]; then
		echo "try to delete file [$file]"
		busybox rm -fr $file
	fi
done

cd ./media/

for file in *
do
	echo $file
	if [ "$file" != "0" ]; then
		echo "try to delete file [$file]"
		busybox rm -fr $file
	fi
done
