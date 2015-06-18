sed /linux/d RKPowerOnAnimation.c > RKPowerOnAnimation_1.c
sed s/__initdata//g RKPowerOnAnimation_1.c > RKPowerOnAnimation_2.c
sed s/__initconst//g RKPowerOnAnimation_2.c > RKPowerOnAnimation.c
rm RKPowerOnAnimation_*
rm array
gcc array2bin.c -o array
./array > animation.bin
