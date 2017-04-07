/*************************************************************************
    > File Name: init.c
    > Author: chengdong
    > Mail: zchengdongdong@gmail.com 
    > Created Time: Tue 29 Jan 2013 11:01:45 AM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "RKPowerOnAnimation.c"

int main(int argc, char *argv[])
{
	write(STDOUT_FILENO, gRKAniData, sizeof(gRKAniData));
	fprintf(stderr, "%d\n", sizeof(gRKAniData));
	return 0;
}
