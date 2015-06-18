/*************************************************************************
    > File Name: /tmp/tp.c
    > Author: chengdong
    > Mail: zchengdongdong@gmail.com 
    > Created Time: Fri 25 Jan 2013 09:31:57 AM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include <string.h>

#define COLOR_BLACK	0
#define COLOR_WHITE	~0
#define FB_DEV	"/dev/fb0"
#define INPUT_DIR "/dev/input"

struct fb_dev {
	int fd;
	int input_fd;
	struct fb_var_screeninfo fb_var;
	void *fbmem;
	int fblen;
	struct input_event event;
};

int tp_fd;
struct input_event event;
int write_fb(struct fb_dev *fb_dev, int row, int col);

int main(int argc, char *argv[])
{
	int i, ret;
	int opt;
	int data[3];
	int bpp;
	int color;
	int infile;
	char buf[1024];
	char *token;
	struct fb_dev fb_dev;
	DIR *dirp;
	struct dirent *dirent;
	char input_dev_name[1024];
	char input_dev_path[1024];

	memset(&fb_dev, 0, sizeof(struct fb_dev));

	if (fb_dev.fd<=0) {
		fb_dev.fd = open(FB_DEV, O_RDWR);
		if (fb_dev.fd < 0)
			perror("open");
	}

	if (ioctl(fb_dev.fd, FBIOGET_VSCREENINFO, &fb_dev.fb_var) < 0) {
		perror("ioctl");
		return -1;
	}

	printf("xres: %d, yres: %d, bpp: %d\n", fb_dev.fb_var.xres, fb_dev.fb_var.yres, fb_dev.fb_var.bits_per_pixel);
	fb_dev.fblen = fb_dev.fb_var.xres * fb_dev.fb_var.yres * fb_dev.fb_var.bits_per_pixel/8;
	fb_dev.fbmem = mmap(NULL, fb_dev.fblen, PROT_READ|PROT_WRITE, MAP_SHARED, fb_dev.fd, 0);
	if (!fb_dev.fbmem) {
		perror("mmap");
		return -1;
	}

	memset(fb_dev.fbmem, COLOR_BLACK, fb_dev.fblen);
	if (ioctl(fb_dev.fd, FBIOPAN_DISPLAY, &fb_dev.fb_var) < 0) {
		perror("ioctl");
	}
	tp_fd = open("/dev/input/event2", O_RDONLY);
	if (tp_fd <= 0)
		return -1;
	ret = ioctl(tp_fd, EVIOCGNAME(64), buf);
	if (ret < 0)
		return -1;
	if (strcmp("cyttsp3", buf))
		return -1;

	fprintf(stdout, "Found: %s \n", "cyttsp3");
	while (1) {
		read(tp_fd, &event, sizeof(struct input_event));
		fprintf(stdout, "Time:%lld Type:%u Code:%u Value:%d\n",
				(int64_t)(event.time.tv_sec * 1000 * 1000 + event.time.tv_usec),
				event.type, event.code, event.value);
		if(event.type == 3 && event.code == 0 && event.value != 0) {
			/*fprintf(stdout, "x = %d\n", event.value);*/
			data[0] = event.value;
		}

		if(event.type == 3 && event.code == 1 && event.value != 0) {
			/*fprintf(stdout, "y = %d\n", event.value);*/
			data[1] = event.value;
		}

		if(event.type == 0 && event.code == 0 && event.value == 0) {
			fprintf(stdout, "data[0] = %d, data[1] = %d\n", data[0], data[1]);
			write_fb(&fb_dev, data[1], data[0]);
			if (ioctl(fb_dev.fd, FBIOPAN_DISPLAY, &fb_dev.fb_var) < 0) {
				perror("ioctl");
			}
		}
	}

	return 0;
}

int write_fb(struct fb_dev *fb_dev, int row, int col)
{
	int i, j, k;
	int offset;
	int color = 0;
	char *buf = fb_dev->fbmem;
	int bpp = fb_dev->fb_var.bits_per_pixel/8;

	offset = fb_dev->fb_var.xres*bpp*row  + col*bpp;
	memset(fb_dev->fbmem + offset, ~color, bpp*5);

	return 0;
}
