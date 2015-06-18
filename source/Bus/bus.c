/*************************************************************************
    > File Name: drivers/input/bus_basic.c
    > Author: chengdong
    > Mail: zchengdongdong@gmail.com 
    > Created Time: Wed 05 Dec 2012 03:12:56 PM CST
 ************************************************************************/

#include <linux/device.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/string.h>

MODULE_LICENSE("Dual BSD/GPL");

extern struct device parent_dev;
static char *Version = "$Revision:1.0 $";

static int my_match(struct device *dev, struct device_driver *driver)
{
	printk("my_match :%s\n", driver->name);
	return !strncmp(dev->kobj.name, driver->name, strlen(driver->name));
}

static void my_bus_release(struct device *dev)
{
	printk("my bus release\n");
}

/*声明总线*/

struct bus_type my_bus_type = {
	.name = "my_bus",//总线名字
	.match = my_match,//总线match函数指针
};


struct device my_bus = {
	/*.init_name = "my_dev",*/
	.kobj.name = "my_bus0",
	/*.parent	= &parent_dev,*/
	/*.bus =	&my_bus_type,*/
	.release = my_bus_release
};

EXPORT_SYMBOL(my_bus);//导出my_bus
EXPORT_SYMBOL(my_bus_type);//导出my_bus_type


static int show_bus_version(struct bus_type *bus, char *buf)
{
	return snprintf(buf, PAGE_SIZE, "%s\n", Version);
}

static int store_bus_version(struct bus_type *bus, const char *buf, size_t count)
{
	printk("buf:%s, count:%d \n", buf, count);
	return count;
}
static ssize_t mydev_show(struct device *dev,struct device_attribute *attr, char *buf)
{
		        return sprintf(buf, "%s\n", "This is my device!");
}
static BUS_ATTR(version, S_IALLUGO, show_bus_version, store_bus_version);
static DEVICE_ATTR(device, S_IRUGO, mydev_show, NULL);
/*模块加载函数*/
static int __init my_bus_init(void)
{
	int ret;

	/*注册总线*/
	ret = bus_register(&my_bus_type);
	if(ret)
		return ret;
	
	/*创建属性文件*/
	if(bus_create_file(&my_bus_type, &bus_attr_version))
		printk(KERN_NOTICE "Fail to create version attribute!\n");
	
	ret = device_register(&my_bus);
	if(ret)
		printk(KERN_NOTICE "Fail to register device:my_bus!\n");

	/*创建属性文件*/
	device_create_file(&my_bus, &dev_attr_device);
	return ret;
}

int mybus_register_device(struct device *dev)
{
	device_register(dev);
	return 0;
}

int mybus_register_driver(struct device_driver *drv)
{
	driver_register(drv);
	return 0;
}

int mybus_unregister_device(struct device *dev)
{
	device_unregister(dev);
	return 0;
}

int mybus_unregister_driver(struct device_driver *drv)
{
	driver_unregister(drv);
	return 0;
}

EXPORT_SYMBOL(mybus_register_device);
EXPORT_SYMBOL(mybus_register_driver);

EXPORT_SYMBOL(mybus_unregister_device);
EXPORT_SYMBOL(mybus_unregister_driver);

static void my_bus_exit(void)
{
	bus_unregister(&my_bus_type);
	device_unregister(&my_bus);
}

module_init(my_bus_init);
module_exit(my_bus_exit);


