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

static char *Version = "$Revision:1.0 $";

static int my_match(struct device *dev, struct device_driver *driver)
{
	return !strncmp(dev->kobj.name, driver->name, strlen(driver->name));
}

/*声明总线*/

struct bus_type my_bus_type = {
	.name = "my_bus",//总线名字
	.match = my_match,//总线match函数指针
};

static int show_bus_version(struct bus_type *bus, char *buf)
{
	return snprintf(buf, PAGE_SIZE, "%s\n", Version);
}

static int store_bus_version(struct bus_type *bus, const char *buf, size_t count)
{
	printk("buf:%s, count:%d \n", buf, count);
	return count;
}
static BUS_ATTR(version, S_IALLUGO, show_bus_version, store_bus_version);

/*模块加载函数*/
static int my_bus_init(void)
{
	int ret;

	/*注册总线*/
	ret = bus_register(&my_bus_type);
	if(ret)
		return ret;
	
	/*创建属性文件*/
	if(bus_create_file(&my_bus_type, &bus_attr_version))
		printk(KERN_NOTICE "Fail to create version attribute!\n");
	
	return ret;
}

static int my_bus_exit(void)
{
	bus_unregister(&my_bus_type);
	return 0;
}

module_init(my_bus_init);
module_exit(my_bus_exit);
