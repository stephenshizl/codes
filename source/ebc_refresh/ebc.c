#include <stdio.h>
#include <stdlib.h>
#include <linux/fb.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>
#include "ebc.h"

#define GET_EBC_BUFFER 0x7000
#define SET_EBC_SEND_BUFFER 0x7001
#define GET_EBC_BUFFER_INFO 0x7003

#define EBC_DEV "/dev/ebc"
#define COLOR_BLACK	0
#define COLOR_WHITE	~0

#define RGB2Luma_4bit(r,g,b)        ((77*r+150*g+29*b)>>12)

void rgb565_convert_line(unsigned short *luma, unsigned short *src,
        unsigned int width)
{
    unsigned int i;
    unsigned int r, g, b, g0, g1, g2, g3;
    unsigned int rgb_data;

    for (i = 0; i < width; i += 4)
    {
		rgb_data = *src++;
		r = (rgb_data & 0xf800)>>8;
		g = (rgb_data & 0x07e0)>>3;
		b = (rgb_data & 0x1f)<<3;
        g0 = RGB2Luma_4bit(r, g, b);

		rgb_data = *src++;
		r = (rgb_data & 0xf800)>>8;
		g = (rgb_data & 0x07e0)>>3;
		b = (rgb_data & 0x1f)<<3;
		g1 = RGB2Luma_4bit(r, g, b);

		rgb_data = *src++;
		r = (rgb_data & 0xf800)>>8;
		g = (rgb_data & 0x07e0)>>3;
		b = (rgb_data & 0x1f)<<3;
		g2 = RGB2Luma_4bit(r, g, b);

		rgb_data = *src++;
		r = (rgb_data & 0xf800)>>8;
		g = (rgb_data & 0x07e0)>>3;
		b = (rgb_data & 0x1f)<<3;
		g3 = RGB2Luma_4bit(r, g, b);
		*luma++ = (g0 & 0x0F) | ((g1 & 0xF)<<4) | ((g2 & 0xF)<<8)| ((g3 & 0xF)<<12);
    }
}

void rgb565_to_gray(void *gray, void *rgb, int height, int width, int panel_w)
{
    int i;

    for (i = 0; i < height; i++) {
        rgb565_convert_line(gray, rgb, width);
        rgb += width*2;
        gray += panel_w/2;
    }
}
void RGB565_2_Luma_0(unsigned short *luma, unsigned short *Src, unsigned int width)
{
	unsigned int i;
	unsigned int r, g, b, g0, g1, g2, g3;
	unsigned int rgb_data;

	for (i=0; i<width; i++)
	{
		rgb_data = *Src++;
		r = (rgb_data & 0xf800)>>8;
		g = (rgb_data & 0x07e0)>>3;
		b = (rgb_data & 0x1f)<<3;       
		g0 = RGB2Luma_4bit(r, g, b);

		rgb_data = *Src++;		
		r = (rgb_data & 0xf800)>>8;
		g = (rgb_data & 0x07e0)>>3;
		b = (rgb_data & 0x1f)<<3;       		
		g1 = RGB2Luma_4bit(r, g, b);

		rgb_data = *Src++;		
		r = (rgb_data & 0xf800)>>8;
		g = (rgb_data & 0x07e0)>>3;
		b = (rgb_data & 0x1f)<<3;       		
		g2 = RGB2Luma_4bit(r, g, b);

		rgb_data = *Src++;		
		r = (rgb_data & 0xf800)>>8;
		g = (rgb_data & 0x07e0)>>3;
		b = (rgb_data & 0x1f)<<3;       
		g3 = RGB2Luma_4bit(r, g, b);
		*luma++ = (g0 & 0x0F) | ((g1 & 0xF)<<4) | ((g2 & 0xF)<<8)| ((g3 & 0xF)<<12);
	}
}

void RGB565_2_Luma_0_rotate(unsigned short *gray, unsigned short *rgb,int h,int w, int panel_w)
{
	int j;
	unsigned short *rgb_buf = (unsigned short *)rgb;
	for (j=0;j<h;j++)
	{
		RGB565_2_Luma_0(gray, rgb_buf, w>>2);
		rgb_buf += w;
		gray += panel_w>>2;
	}
}

void rgb888_convert_line(unsigned short *luma, unsigned char *src,
        unsigned int width)
{
    unsigned int i;
    unsigned int r, g, b, g0, g1, g2, g3;

    for (i = 0; i < width; i += 4) {
        r = *src++;
        g = *src++;
        b = *src++;
        g0 = RGB2Luma_4bit(r, g, b);

        r = *src++;
        g = *src++;
        b = *src++;
        g1 = RGB2Luma_4bit(r, g, b);

        r = *src++;
        g = *src++;
        b = *src++;
        g2 = RGB2Luma_4bit(r, g, b);

        r = *src++;
        g = *src++;
        b = *src++;
        g3 = RGB2Luma_4bit(r, g, b);

        *luma++ = (g0 & 0x0F) | ((g1 & 0xF)<<4) |
            ((g2 & 0xF)<<8)| ((g3 & 0xF)<<12);
    }
}

void rgb888_to_gray(void *gray, void *rgb, int height, int width, int panel_w)
{
    int i;

    for (i = 0; i < height; i++) {
        rgb888_convert_line(gray, rgb, width);
        rgb += width*3;
        gray += panel_w/2;
    }
}

int ebc_init(struct ebc_dev *dev)
{
    struct ebc_buf_info *info = &dev->info;

    dev->ebc_fd = open(EBC_DEV, O_RDWR);
    if (dev->ebc_fd < 0) {
		printf("no ebc.\n");
        return -1;
    }

    if (ioctl(dev->ebc_fd, GET_EBC_BUFFER_INFO, info) != 0) {
        perror("ioctl");
        return -1;
    }

    dev->ebc_len = info->vir_width * info->vir_height * 2;
    dev->ebc_mem = mmap(0, dev->ebc_len, PROT_READ | PROT_WRITE, MAP_SHARED,
            dev->ebc_fd, 0);
    if (dev->ebc_mem == MAP_FAILED) {
        perror("mmap");
        return -1;
    }

    printf("ebc_init: vwidth=%d vheight=%d\n", info->vir_width, info->vir_height);

    return 0;
}

int ebc_exit(struct ebc_dev *dev)
{
    if (dev->ebc_fd > 0) {
        munmap(dev->ebc_mem, dev->ebc_len);
        close(dev->ebc_fd);
    }
    return 0;
}

int ebc_refresh(struct ebc_dev *dev, void *rgb_buf, int bpp, int mode)
{
    struct ebc_buf_info *info = &dev->info;

    if (dev->ebc_fd <= 0)
        return -1;

    if (ioctl(dev->ebc_fd, GET_EBC_BUFFER, info) != 0) {
        perror("ioctl GET_EBC_BUFFER");
        return -1;
    }

 	info->epd_mode = mode;

    switch (bpp) {
	       case 24:
	               rgb888_to_gray(dev->ebc_mem + info->offset, rgb_buf,
	                               info->vir_height, info->vir_width, info->width);
	               break;
	       case 16:
	            RGB565_2_Luma_0_rotate(dev->ebc_mem + info->offset, rgb_buf,
	                               info->vir_height, info->vir_width, info->width);
	               break;
	       default:
	               fprintf(stderr, "bpp:%d not support yet!\n", bpp);
	               break;
	       }

    if (ioctl(dev->ebc_fd, SET_EBC_SEND_BUFFER, info) != 0) {
        perror("ioctl SET_EBC_SEND_BUFFER");
        return -1;
    }

    return 0;
}

#ifdef CONFIG_LIBPNG
#include <png.h>
int ebc_show_png(struct ebc_dev *dev, const char *file)
{
    struct ebc_buf_info *info = &dev->info;
    png_structp png_ptr;
    png_infop info_ptr;
    int height, pixel_size, i;
    int row_size;
    png_bytepp row_pointers;
    FILE *fp;

    if (dev->ebc_fd <= 0)
        return -1;

    fp = fopen(file, "rb");
    if (!fp) {
        fprintf(stderr, "open %s error", file);
        return -1;
    }

    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    info_ptr = png_create_info_struct(png_ptr);
    png_init_io(png_ptr, fp);

    png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);

    height = png_get_image_height(png_ptr, info_ptr);
    row_size = png_get_rowbytes(png_ptr, info_ptr);

    if (!(png_get_bit_depth(png_ptr, info_ptr) == 8 &&
                (png_get_channels(png_ptr, info_ptr)== 3 &&
                 png_get_color_type(png_ptr, info_ptr) == PNG_COLOR_TYPE_RGB))) {
        fprintf(stderr, "only 8bits RGB support.\n");
        goto exit;
    }

    row_pointers = png_get_rows(png_ptr, info_ptr);

    if (ioctl(dev->ebc_fd, GET_EBC_BUFFER, info) != 0) {
        perror("ioctl GET_EBC_BUFFER");
        return -1;
    }
    info->epd_mode = 1;

    for (i = 0; i < height; i++) {
        rgb888_convert_line(
                dev->ebc_mem + info->offset + (info->width * 2 / 4)*i,
                row_pointers[i], info->vir_width);
    }

    if (ioctl(dev->ebc_fd, SET_EBC_SEND_BUFFER, info) != 0) {
        perror("ioctl SET_EBC_SEND_BUFFER");
        return -1;
    }

exit:
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(fp);
    return 0;
}
#endif
