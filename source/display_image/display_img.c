/*************************************************************************
  > File Name: writefb.c
  > Author: chengdong
  > Mail: zchengdongdong@gmail.com 
  > Created Time: Thu 18 Oct 2012 06:44:09 PM CST
 ************************************************************************/

#include <stdio.h>
#include <linux/fb.h>
#include <string.h>
#include <fcntl.h>

#define DEBUG

#ifdef DEBUG
#define LOG(fmt, arg...) printf("[Display] " fmt, ##arg)
#else
#define LOG(fmt, arg...)
#endif

int write_logo(int fd_fb, const char *logo)
{
	struct stat st;
	int fd_logo, ret;

	fd_logo = open(logo, O_RDONLY);
	if (fd_logo < 0) {
		LOG("failed to open logo.\n");
		return -1;
	}

	if (fstat(fd_logo, &st) < 0) {
		LOG("failed to fstat logo.\n");
		close(fd_logo);
		return -1;
	}

	if (sendfile(fd_fb, fd_logo, NULL, st.st_size) < 0) {
		LOG("failed to sendfile.\n");
		close(fd_logo);
		return -1;
	}

	close(fd_logo);

	return 0;
}

int main (int argc, char *argv[])
{
	struct fb_var_screeninfo var;
	int fd, ret;
	char fb_path[128];
	char display_img[128];
	if (argc <= 1){
		snprintf(fb_path, sizeof(fb_path), "%s", "/dev/fb0");
		snprintf(display_img, sizeof(display_img), "%s", "/images/logo-normal-h");
	}else{
		snprintf(fb_path, sizeof(fb_path), "%s", argv[0]);
		snprintf(display_img, sizeof(display_img), "%s", argv[1]);
	}
	
	fd = open(fb_path, O_RDWR);
	if (fd < 0) {
		fd = open("/dev/fb0", O_RDWR);
		if (fd < 0) {
			LOG("failed to open fb.\n");
			return -1;
		}
	}

	ioctl(fd, FBIOGET_VSCREENINFO, &var);

	//LOG("xres: %d, yres: %d, bpp: %d\n", var.xres, var.yres, var.bits_per_pixel);

	ioctl(fd, FBIOBLANK, FB_BLANK_POWERDOWN);
	ioctl(fd, FBIOBLANK, FB_BLANK_UNBLANK);

	if ((var.xres == 800) && (var.yres == 600)) {
		ret = write_logo(fd, display_img);
	} else {
		ret = write_logo(fd, display_img);
	}

	LOG("%s.\n", ret ? "failed" : "success");
	LOG("fb_path: %s, display_img: %s.\n", fb_path, display_img);

	if (ioctl(fd, FBIOPAN_DISPLAY, &var) < 0) {
		perror("ioctl");
	}

	close(fd);

	return ret;

}

//int display_logo(char *fb, char *img)
//{
//	struct fb_var_screeninfo var;
//	int fd, ret;
//	if (strlen(fb) == 0)
//		LOG("FB_len = %d\n",strlen(fb));
//	fd = open(fb, O_RDWR);
//	if (fd < 0) {
//		fd = open("/dev/fb0", O_RDWR);
//		if (fd < 0) {
//			LOG("failed to open fb.\n");
//			return -1;
//		}
//	}
//
//	ioctl(fd, FBIOGET_VSCREENINFO, &var);
//
//	LOG("xres: %d, yres: %d, bpp: %d\n", var.xres, var.yres, var.bits_per_pixel);
//
//	ioctl(fd, FBIOBLANK, FB_BLANK_POWERDOWN);
//	ioctl(fd, FBIOBLANK, FB_BLANK_UNBLANK);
//
//	if ((var.xres == 800) && (var.yres == 600)) {
//		ret = write_logo(fd, img);
//	} else {
//		ret = write_logo(fd, img);
//	}
//
//	LOG("display logo (%s).\n", ret ? "failed" : "success");
//
//	close(fd);
//
//	return ret;
//}

