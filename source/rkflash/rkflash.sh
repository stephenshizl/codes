#!/bin/bash

# Usage:
# 	./rkflash C60 kernel boot
# 	./rkflash E8 kernel boot


# 烧录system 执行./rkflash.sh system
# 烧录kernel 执行./rkflash.sh kernel


prog=/home/june/work/tools/rkflash

files_C60=(	\
		parameter 	 0x0     	 "../images/parameter"	\
		misc      	 0x2000  	 "../images/misc.img"            	\
	 	kernel    	 0x4000  	 "/home/june/images/kernel.img.C60"                	\
		boot      	 0x8000  	 "../images/boot.img"				\
		recovery  	 0xC000  	 "../images/recovery.img"                   \
		backup    	 0x10000  	 "../images/recovery.img"        	\
		system    	 0x12000 	 "../images/system.img"                    	\
		data         0xD2000     \
		user      	 0x130000 	 \
	)

files_F63=(	\
	 	 parameter 	 0x0     	 "/home/june/work/F63/rk2906-android/tools/parameter"	\
	 	 misc      	 0x2000  	 "/home/june/work/F63/rk2906-android/rockdev/Image/misc.img"            	\
	 	 kernel    	 0x4000  	 "/home/june/images/kernel.img.F63"                	\
	 	 waveform    0x4000  	 "/home/june/work/F63/rk2906-android/tools/OemDataPacket/waveform.img"                	\
	 	 boot    	 0x8000  	 "/home/june/work/F63/rk2906-android/rockdev/Image/boot.img"                	\
	 	 recovery    0x10000  	 "/home/june/work/F63/rk2906-android/rockdev/Image/recovery.img"                	\
	 	 backup    	 0x20000  	 ""                	\
	 	 cache    	 0x98000  	 ""                	\
	 	 userdata    0xd2000  	 ""                	\
	 	 kpanic    	 0x1d2000  	 ""                	\
	 	 system    	 0x1cc000  	 "/home/june/work/F63/rk2906-android/rockdev/Image/system.img"                	\
	)

files_S70=(	\
	 	 parameter 	 0x0     	 "/home/june/work/images4.1/parameter"	\
	 	 misc      	 0x2000  	 "/home/june/work/AK41/rk2918-android-jb/rockdev/Image/misc.img"            	\
	 	 kernel    	 0x4000  	 "/home/june/images/kernel.img.S70"                	\
	 	 boot      	 0x8000  	 "/home/june/work/AK41/rk2918-android-jb/rockdev/Image/boot.img"				\
	 	 recovery  	 0x10000  	 "/home/june/work/AK41/rk2918-android-jb/rockdev/Image/recovery.img"                   \
	 	 user      	 0x670000 	 ""	\
	)

files_D72=(	\
	 	 parameter 	 0x0     	 "/home/june/work/3028/D72-TMI/parameter"	\
	 	 misc      	 0x2000  	 "/home/june/work/3028/D72-TMI/Image/Image/misc.img"            	\
	 	 kernel    	 0x4000  	 "/home/june/images/kernel.img.D72"                	\
	 	 boot    	 0xA000  	 "/home/june/work/3028/D72-TMI/Image/Image/boot.img"                	\
	 	 recovery    0x12000  	 "/home/june/work/3028/D72-TMI/Image/Image/recovery.img"                	\
	 	 backup    	 0x22000  	 ""                	\
	 	 cache    	 0x42000  	 ""                	\
	 	 userdata    0x82000  	 ""                	\
	 	 kpanic    	 0x282000  	 ""                	\
	 	 system    	 0x284000  	 "/home/june/work/3028/D72-TMI/Image/Image/system.img"                	\
	 	 user      	 0x404000 	 ""	\
	)

files_K70=(	\
	 	 kernel    	 0x4000  	 "/home/june/images/kernel.img.K70"                	\
	)

files_Dongle=(	\
	 	 parameter 	 0x0     	 "/home/june/work/dongle/rk2928-dongle-jb-4.2.2/rk_sdk/tools/rockdev/parameter"	\
	 	 kernel    	 0x2000  	 "/home/june/images/kernel.img.S70"                	\
	 	 boot      	 0x6000  	 "/home/june/work/dongle/rk2928-dongle-jb-4.2.2/rockdev/Image/boot.img"				\
	 	 userdata    0x6200  	 "/home/june/work/dongle/rk2928-dongle-jb-4.2.2/rk_sdk/tools/rockdev/data_512K.jffs2"				\
	 	 system      0x6600  	 "/home/june/work/dongle/rk2928-dongle-jb-4.2.2/rockdev/Image/system.img"				\
	 	 backup  	 0x14c00  	 "/home/june/work/dongle/rk2928-dongle-jb-4.2.2/rockdev/Image/update.img"                   \
	 	 user      	 0x14c08 	 ""	\
	)

files_Rootfs=(	\
	 	 boot      	 0xA000  	 "/home/june/Dropbox/tool/busybox/images/boot.img"		\
	 	 system      0x284000  	 "/home/june/Dropbox/tool/busybox/images/system.img"				\
	 	 recovery  	 0x10000  	 "/home/june/work/busybox/crootfs.img"                   \
	)

case $1 in
	c60)
		files=( ${files_C60[@]} )
		shift 1
		;;
	e8)
		files=( ${files_E8[@]} )
		shift 1
		;;
	fs)
		files=( ${files_Rootfs[@]} )
		shift 1
		;;
	f63)
		files=( ${files_F63[@]} )
		shift 1
		;;
	s70)
		files=( ${files_S70[@]} )
		shift 1
		;;
	k70)
		files=( ${files_K70[@]} )
		shift 1
		;;
	d72)
		files=( ${files_D72[@]} )
		shift 1
		;;
	dg)
		files=( ${files_Dongle[@]} )
		shift 1
		;;
	-b)
		$prog -b
		exit
		;;
esac

case $1 in
	-b)
		$prog -b
		exit
		;;
esac

case $1 in 
	e9)
		fastboot flash $2 /home/june/images/zImage.E9
		exit
		;;
esac

targets="kernel boot recovery system"

flash_all() {
	targets="parameter misc kernel boot recovery system backup"
}

if [[ $# -gt 0 ]]; then
	targets=$@
fi

# flash images 
for target in $targets; do
	for i in $(seq 0 ${#files[*]} ); do
			#echo files:${files[i]}
		if [[ $target == ${files[i]} ]]; then
			echo -e "flashing $target: ${files[i+2]}"

			size=$(ls -l ${files[i+2]} | awk '{print $5}')
			$prog -w ${files[i+1]} $(($size/512 + 1)) ${files[i+2]}

			echo
		fi
	done
done
