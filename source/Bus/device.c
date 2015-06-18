/*************************************************************************
    > File Name: drivers/input/device.c
    > Author: chengdong
    > Mail: zchengdongdong@gmail.com 
    > Created Time: Thu 06 Dec 2012 03:06:17 PM CST
 ************************************************************************/

#include <linux/device.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/string.h>

MODULE_LICENSE("Dual BSD/GPL");

extern struct device my_bus;
extern struct bus_type my_bus_type;
extern int mybus_register_device(struct device *dev);
extern int mybus_unregister_device(struct device *dev);

static void my_dev_release(struct device *dev)
{

}

struct device my_dev = {
	.init_name = "my_dev",
	.bus = &my_bus_type,
	.parent = &my_bus,
	.release = my_dev_release,
};

static ssize_t mydev_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%s\n", "This is my device!");
}

static DEVICE_ATTR(dev, S_IRUGO, mydev_show, NULL);

static int __init my_device_init(void)
{
	int ret = 0;
	
	/*注册设备*/
	/*device_register(&my_dev);*/
	mybus_register_device(&my_dev);


	/*创建属性文件*/
	device_create_file(&my_dev, &dev_attr_dev);

	return ret;
}

static void my_device_exit(void)
{
	mybus_unregister_device(&my_dev);
}

module_init(my_device_init);
module_exit(my_device_exit);
