#ifndef __EBC_H__
#define __EBC_H__

struct ebc_buf_info{
	int offset;
	int epd_mode;
	int height;
	int width;
	int vir_height;
	int vir_width;
};

struct ebc_dev {
    int ebc_fd;
    int ebc_len;
    void *ebc_mem;
    void *rgb_buf;
    struct ebc_buf_info info;
    int rotate;
};

//int ebc_init(struct ebc_dev *dev);
//int ebc_exit(struct ebc_dev *dev);
//int ebc_refresh(struct ebc_dev *dev, void *rgb_buf, int bpp, int mode);

#endif
