/*************************************************************************
    > File Name: include/linux/mybus_driver.h
    > Author: chengdong
    > Mail: zchengdongdong@gmail.com 
    > Created Time: Fri 14 Dec 2012 10:26:37 AM CST
 ************************************************************************/

#include <linux/delay.h>
#include <linux/input.h>
#include <linux/miscdevice.h>

static int mybus_dev1_open(struct inode *inode, struct file *file);
static int mybus_dev1_release(struct inode *inode, struct file *file);
static int mybus_dev1_ioctl(struct inode *inode, struct file *file, unsigned int cmd, unsigned long arg);


struct mybus_driver_info {
	char *name;
	char *dev_file;
	bool		wakeup;
	bool		active_low;
};

struct mybus_driver_data {
	int statu;
	int irq_enable;
	struct input_dev *input_dev;
};

