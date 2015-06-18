/*************************************************************************
    > File Name: disfb.c
    > Author: chengdong
    > Mail: zchengdongdong@gmail.com 
    > Created Time: Mon 15 Apr 2013 09:34:13 AM CST
 ************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <linux/fb.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <linux/input.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>
#include "ebc.h"

#include <time.h>

#define GET_EBC_BUFFER 0x7000
#define SET_EBC_SEND_BUFFER 0x7001
#define GET_EBC_BUFFER_INFO 0x7003

#define FB_DEV	"/dev/fb0"
#define EBC_DEV "/dev/ebc"

#define COLOR_BLACK	0
#define COLOR_WHITE	~0

#define RGB2Luma_4bit(r,g,b)        ((77*r+150*g+29*b)>>12)

#define ROTATE_0     0
#define ROTATE_90   1
#define ROTATE_270 2 

#if defined(ROTATE270)
#define ROTATE    ROTATE_0
#elif defined(ROTATE90)
#define ROTATE    ROTATE_90
#else 
#define ROTATE    ROTATE_0
#endif
enum GGLPixelFormat {
    // these constants need to match those
    // in graphics/PixelFormat.java, ui/PixelFormat.h, BlitHardware.h
    GGL_PIXEL_FORMAT_UNKNOWN    =   0,
    GGL_PIXEL_FORMAT_NONE       =   0,

    GGL_PIXEL_FORMAT_RGBA_8888   =   1,  // 4x8-bit ARGB
    GGL_PIXEL_FORMAT_RGBX_8888   =   2,  // 3x8-bit RGB stored in 32-bit chunks
    GGL_PIXEL_FORMAT_RGB_888     =   3,  // 3x8-bit RGB
    GGL_PIXEL_FORMAT_RGB_565     =   4,  // 16-bit RGB
    GGL_PIXEL_FORMAT_BGRA_8888   =   5,  // 4x8-bit BGRA
    GGL_PIXEL_FORMAT_RGBA_5551   =   6,  // 16-bit RGBA
    GGL_PIXEL_FORMAT_RGBA_4444   =   7,  // 16-bit RGBA

    GGL_PIXEL_FORMAT_A_8         =   8,  // 8-bit A
    GGL_PIXEL_FORMAT_L_8         =   9,  // 8-bit L (R=G=B = L)
    GGL_PIXEL_FORMAT_LA_88       = 0xA,  // 16-bit LA
    GGL_PIXEL_FORMAT_RGB_332     = 0xB,  // 8-bit RGB (non paletted)

    // reserved range. don't use.
    GGL_PIXEL_FORMAT_RESERVED_10 = 0x10,
    GGL_PIXEL_FORMAT_RESERVED_11 = 0x11,
    GGL_PIXEL_FORMAT_RESERVED_12 = 0x12,
    GGL_PIXEL_FORMAT_RESERVED_13 = 0x13,
    GGL_PIXEL_FORMAT_RESERVED_14 = 0x14,
    GGL_PIXEL_FORMAT_RESERVED_15 = 0x15,
    GGL_PIXEL_FORMAT_RESERVED_16 = 0x16,
    GGL_PIXEL_FORMAT_RESERVED_17 = 0x17,

    // reserved/special formats
    GGL_PIXEL_FORMAT_Z_16       =  0x18,
    GGL_PIXEL_FORMAT_S_8        =  0x19,
    GGL_PIXEL_FORMAT_SZ_24      =  0x1A,
    GGL_PIXEL_FORMAT_SZ_8       =  0x1B,

    // reserved range. don't use.
    GGL_PIXEL_FORMAT_RESERVED_20 = 0x20,
    GGL_PIXEL_FORMAT_RESERVED_21 = 0x21,
};
static int gr_fb_fd = -1;
static int gr_vt_fd = -1;
int recovery_ebc_fd = -1;
int recovery_ebc_buffer_base;
int recovery_ebc_buffer_len;
static unsigned gr_active_fb = 0;
static int text_col = 0, text_row = 0, text_top = 0;
static struct fb_var_screeninfo vi;

typedef struct {
    ssize_t version;    // always set to sizeof(GGLSurface)
    uint32_t width;      // width in pixels
    uint32_t height;     // height in pixels
    int32_t stride;     // stride in pixels
    uint8_t*   data;       // pointer to the bits
    uint8_t    format;     // pixel format
    uint8_t    rfu[3];     // must be zero
    // these values are dependent on the used format
    union {
       int32_t  compressedFormat;
       int32_t  vstride;
    };
    void*       reserved;
}GGLSurface;

static GGLSurface gr_mem_surface;
static GGLSurface gr_framebuffer[2];

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

static int get_framebuffer(GGLSurface *fb)
{
    int fd; struct fb_fix_screeninfo fi;
    void *bits;

    fd = open("/dev/fb0", O_RDWR);
    if (fd < 0) {
        perror("cannot open fb0");
        return -1;
    }

    if (ioctl(fd, FBIOGET_FSCREENINFO, &fi) < 0) {
        perror("failed to get fb0 info");
        close(fd);
        return -1;
    }

    if (ioctl(fd, FBIOGET_VSCREENINFO, &vi) < 0) {
        perror("failed to get fb0 info");
        close(fd);
        return -1;
    }

    bits = mmap(0, fi.smem_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (bits == MAP_FAILED) {
        perror("failed to mmap framebuffer");
        close(fd);
        return -1;
    }

	int offset;
	int row, col;
	int bpp = vi.bits_per_pixel/8;
	row = 300;
	col = 400;
	offset = vi.xres*bpp*row + col*bpp;
    memset(bits + offset, ~0, bpp * 10);

    fb->version = sizeof(*fb);
    fb->width = vi.xres;
    fb->height = vi.yres;
    fb->stride = vi.xres;
    fb->data = bits;
    fb->format = GGL_PIXEL_FORMAT_RGB_565;
    memset(fb->data, 0, vi.yres * vi.xres * 2);

	printf("%s, vi.xres:%d ,vi.yres:%d\n", __func__, vi.xres, vi.yres);
    /*fb++;*/

    /*fb->version = sizeof(*fb);*/
    /*fb->width = vi.xres;*/
    /*fb->height = vi.yres;*/
    /*fb->stride = vi.xres;*/
    /*fb->data = (void*) (((unsigned) bits) + vi.yres * vi.xres * 2);*/
    /*fb->format = GGL_PIXEL_FORMAT_RGB_565;*/
    /*memset(fb->data, 0, vi.yres * vi.xres * 2);*/

	recovery_ebc_fd = open("/dev/ebc", O_RDWR,0);
	if (recovery_ebc_fd < 0){
	   perror("open /dev/ebc failed");
       	return -1;
   	}
	
	struct ebc_buf_info buf_info;
	void* vaddr = NULL;
	if(ioctl(recovery_ebc_fd, GET_EBC_BUFFER_INFO,&buf_info)!=0){
		perror("GET_EBC_BUFFER failed");
	}
	printf("%s, buf_info.vir_width:%d ,buf_info.vir_height:%d\n", __func__, buf_info.vir_width, buf_info.vir_height);
	recovery_ebc_buffer_len = buf_info.vir_width*buf_info.vir_height*2;
	vaddr = mmap(0, buf_info.vir_width*buf_info.vir_height*2, PROT_READ|PROT_WRITE, MAP_SHARED, recovery_ebc_fd, 0);
	if (vaddr == MAP_FAILED) {
	    perror("Error mapping the ebc buffer ");
	    return -1;
	}
	recovery_ebc_buffer_base = vaddr;

    return fd;
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
static void set_active_framebuffer(unsigned n)
{
    if (n > 1) return;
    vi.yres_virtual = vi.yres * 2;
    vi.yoffset = n * vi.yres;
    vi.bits_per_pixel = 16;
    if (ioctl(gr_fb_fd, FBIOPUT_VSCREENINFO, &vi) < 0) {
        perror("active fb swap failed");
    }

	{
		printf("in set_active_framebuffer\n");
		int temp_addr;
		struct ebc_buf_info buf_info;
		if(ioctl(recovery_ebc_fd, GET_EBC_BUFFER,&buf_info)!=0){
			perror("GET_EBC_BUFFER failed");
		}
		temp_addr = recovery_ebc_buffer_base + buf_info.offset;

		printf("%s, buf_info.offset:%d \n", __func__, buf_info.offset);
		printf("%s, buf_info.vir_height:%d ,buf_info.vir_width:%d, buf_info.width:%d\n", __func__, buf_info.vir_height, buf_info.vir_width, buf_info.width);
		RGB565_2_Luma_0_rotate((unsigned short*)temp_addr,(unsigned short*)gr_framebuffer[n].data,buf_info.vir_height,buf_info.vir_width,buf_info.width);
		
		buf_info.epd_mode = 1;

		if(ioctl(recovery_ebc_fd, SET_EBC_SEND_BUFFER,&buf_info)!=0){
			perror("SET_EBC_SEND_BUFFER failed");
		}
		
	}
	
}

static void get_memory_surface(GGLSurface* ms) {
  ms->version = sizeof(*ms);

  if(ROTATE == ROTATE_0){
	  ms->width = vi.xres;
	  ms->height = vi.yres;
	  ms->stride = vi.xres;
  }
  else{
	  ms->width = vi.yres;
	  ms->height = vi.xres;
	  ms->stride = vi.yres;
  }
  ms->data = malloc(vi.xres * vi.yres * 2);
  ms->format = GGL_PIXEL_FORMAT_RGB_565;
}

int main(int argc, char *argv[])
{
    time_t start = time(NULL);

    printf("Starting recovery on %s", ctime(&start));

    gr_fb_fd = get_framebuffer(gr_framebuffer);
    if (gr_fb_fd < 0) {
        return -1;
    }

    get_memory_surface(&gr_mem_surface);

	 typedef unsigned short pixel;
	 pixel* dst = gr_framebuffer[gr_active_fb].data;
	 pixel* src = gr_mem_surface.data;
	 int i,j;
	switch(ROTATE){
		case ROTATE_90:
					for(i = 0;i < vi.yres;i++) {
						for(j = 0;j < vi.xres;j++) {
							dst[i * vi.xres + vi.xres - 1 - j] = src[i + j * vi.yres];
						}
					}
			break;
		case  ROTATE_270:
			for(i = 0;i < vi.yres;i++) {
				for(j = 0;j < vi.xres;j++) {
					dst[(vi.yres -1 - i) * vi.xres + j] = src[i + j * vi.yres];
				}
			}
			break;
		default://0
			memcpy(gr_framebuffer[gr_active_fb].data, gr_mem_surface.data,
				  vi.xres * vi.yres * 2);
			break;
	}
	set_active_framebuffer(0);
	ioctl(gr_fb_fd, FBIOPAN_DISPLAY, &vi);

exit:
	munmap(gr_framebuffer[0].data, vi.yres * vi.xres * 2);
	munmap(gr_framebuffer[1].data, vi.yres * vi.xres * 2);
	munmap(recovery_ebc_buffer_base, recovery_ebc_buffer_len);
	close(gr_fb_fd);
	close(recovery_ebc_fd);
	return 0;
}

