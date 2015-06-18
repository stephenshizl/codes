/*************************************************************************
    > File Name: drivers/input/driver.c
    > Author: chengdong
    > Mail: zchengdongdong@gmail.com 
    > Created Time: Thu 06 Dec 2012 03:43:04 PM CST
 ************************************************************************/

#include <linux/device.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/string.h>

MODULE_LICENSE("Dual BSD/GPL");

extern struct bus_type my_bus_type;
extern int mybus_register_driver(struct device_driver *drv);
extern int mybus_unregister_driver(struct device_driver *drv);

static int my_probe(struct device *dev)
{
	printk("Driver found device which my driver can handle!\n");
	return 0;
}

static int my_remove(struct device *dev)
{
	printk("Driver found device unpluged!\n");
	return 0;
}

struct device_driver my_driver = {
		.name = "my_dev",
		.bus = &my_bus_type,
		.probe = my_probe,
		.remove = my_remove,
};

static ssize_t mydriver_show(struct device_driver *driver, char *buf)
{
		return sprintf(buf, "%s\n", "This is my driver!");
}

static DRIVER_ATTR(drv, S_IRUGO, mydriver_show, NULL);

static int __init my_driver_init(void)
{
	int ret = 0;
	 
	/*注册驱动*/
	/*driver_register(&my_driver);*/
	mybus_register_driver(&my_driver);

	/*创建属性文件*/
	driver_create_file(&my_driver, &driver_attr_drv);

	return ret;
}

static void my_driver_exit(void)
{
	mybus_unregister_driver(&my_driver);
}

module_init(my_driver_init);
module_exit(my_driver_exit);
