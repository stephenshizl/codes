#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/ioctl.h>

#define DEV_NAME "/dev/bootmode"

#define	HEARTBEAT_IOCTL_BASE	'H'
#define	HEARTBEAT_GET_BOOTFLAG        	_IOR(HEARTBEAT_IOCTL_BASE, 0, int)

int main (int argc , char *argv[])
{

    int fd;
    int ret;
	int bootmode;

    fd = open(DEV_NAME, O_RDWR);
    if (fd < 0) {
        printf("Failed to open %s: %d\n", DEV_NAME, fd);
        return 1;
    }

	ret = ioctl(fd, HEARTBEAT_GET_BOOTFLAG, &bootmode);
	if (ret < 0)
		printf("timeout.\n");
	else 
		printf("bootmode: %d.\n", bootmode);

	return 0;
}
