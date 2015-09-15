/*************************************************************************
    > File Name: drivers/input/bus_basic.c
    > Author: chengdong
    > Mail: zchengdongdong@gmail.com 
    > Created Time: Wed 05 Dec 2012 03:12:56 PM CST
 ************************************************************************/

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/string.h>
#include <linux/pm.h>
#include <linux/bus_basic.h>
#include <linux/cdev.h>
#include <linux/err.h>

MODULE_LICENSE("Dual BSD/GPL");

static int mybus_match(struct device *dev, struct device_driver *driver);
void mybus_dev_release(struct device *dev);
static char *Version = "$Revision:1.0 $";

static int mybus_major = 58;
static int mybus_minor = 0;

static int mybus_legacy_suspend(struct device *dev) {

	struct mybus_device *mybus_dev = container_of(dev, struct mybus_device, dev);
	struct mybus_driver *mybus_drv = container_of(dev->driver, struct mybus_driver, driver);
	int ret;

	if(dev->driver && mybus_drv->suspend)
		ret = mybus_drv->suspend(mybus_dev, PMSG_SUSPEND);

	return ret;
}

static int mybus_legacy_resume(struct device *dev) {

	struct mybus_device *mybus_dev = container_of(dev, struct mybus_device, dev);
	struct mybus_driver *mybus_drv = container_of(dev->driver, struct mybus_driver, driver);
	int ret;

	if(dev->driver && mybus_drv->resume)
		ret = mybus_drv->resume(mybus_dev);

	return ret;
}

static int mybus_suspend(struct device *dev) {
	
	int ret = 0;
	struct device_driver *drv = dev->driver;
	
	if(drv == NULL)
		return 0;

	if(drv->pm) {
		if(drv->pm->suspend)
		ret = drv->pm->suspend(dev);
	} else {
		ret = mybus_legacy_suspend(dev);
	}
	
	return ret;
}

static int mybus_resume(struct device *dev) {
	
	int ret;
	struct device_driver *drv = dev->driver;
	
	printk("func :%s\n", __func__);
	if(drv == NULL)
		return 0;

	if(drv->pm) {
		if(drv->pm->resume)
		ret = drv->pm->resume(dev);
	} else {
		ret = mybus_legacy_resume(dev);
	}
	
	return ret;
}

static int mybus_probe(struct device *dev) {

	int ret;
	struct mybus_device *mybus_dev = container_of(dev, struct mybus_device, dev);
	struct mybus_driver *mybus_drv = container_of(dev->driver, struct mybus_driver, driver);

	if(dev->driver && mybus_drv->probe)
		ret = mybus_drv->probe(mybus_dev);

	return 0;
}


static int poweroff(struct device *dev) {

	return 0;
}

static struct dev_pm_ops mybus_pm_ops = {

	.suspend = mybus_suspend,
	/*.poweroff = mybus_poweroff,*/
	.resume = mybus_resume,
};

static struct mybus my_bus_type = {
	.mybus_type = {
		.name = "my_bus",
		.match = &mybus_match,
		.pm = &mybus_pm_ops,
	},
};

static struct mybus_device mybus_dev = {
		.name	= "mybus_dev",
		.dev	= {
			.parent = NULL,
			.kobj.name	= "mybus_dev",
			.bus	= &my_bus_type,
			/*.class	= &mybus_class,*/
			.release = mybus_dev_release,
		},
};

void mybus_dev_release(struct device *dev)
{
	printk("mybus_dev.name :%s, mybus_dev.dev.kobj.name :%s\n", mybus_dev.name, mybus_dev.dev.kobj.name);
	printk("mybus_dev release\n");
}

static int mybus_match(struct device *dev, struct device_driver *driver)
{
	printk("mybus_match driver->name:%s\n", driver->name);
	return !strncmp(dev->kobj.name, driver->name, strlen(driver->name));
}

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

int mybus_add_device(struct mybus_device **device, unsigned len)
{
	int i, ret = 0;
	for(i = 0; i < len; i++){
		printk("__func__ ,name :%s\n", device[i]->name);
		device_initialize(&device[i]->dev);
		ret = mybus_register_device(device[i]);
		if(ret) {
			while(--i >= 0)
				ret = mybus_unregister_device(device[i]);
			break;
		}
	}

	return ret;
}

int mybus_register_device(struct mybus_device *pdev)
{

	int ret;
	char *dev_name = pdev->name;

	if(!pdev->dev.parent)
		pdev->dev.parent = &mybus_dev.dev;

	pdev->dev.bus = &my_bus_type;
	/*pdev->dev.devt = MKDEV(mybus_major, 0);*/
	/*pdev->dev.class = mybus_class;*/
	pdev->dev.release = mybus_dev_release;
	/*dev_set_drvdata(&pdev->dev, pdev->name);*/
	dev_set_name(&pdev->dev, "%s", pdev->name);

	ret = device_add(&pdev->dev);
	device_create(mybus_class, NULL, MKDEV(mybus_major, 0), "mybus_test" "%s", "mybus_dev1_file");

	return ret;
}

int mybus_register_driver(struct mybus_driver *pdrv)
{
	pdrv->driver.bus = &my_bus_type;
	if(pdrv->probe)
		pdrv->driver.probe = mybus_probe;
	return driver_register(&pdrv->driver);
}


int mybus_unregister_device(struct mybus_device *pdev)
{
	printk("func %s\n", __func__);
	device_unregister(&pdev->dev);
	return 0;
}

int mybus_unregister_driver(struct mybus_driver *pdrv)
{
	driver_unregister(&pdrv->driver);
	return 0;
}
/*模块加载函数*/
int __init my_bus_init(void)
{
	int ret;

	/*注册总线*/
	ret = bus_register(&my_bus_type.mybus_type);
	if(ret)
		return ret;

	/*创建class,供设备类生成设备文件使用*/
	mybus_class = class_create(THIS_MODULE, "mybus_class");
	/*ret = class_register(&mybus_class);*/
	if(ret) 
	{
		printk("Err: failed in creating class.\n");
		return -1; 
	}

	/*创建属性文件*/
	if(bus_create_file(&my_bus_type.mybus_type, &bus_attr_version))
		printk(KERN_NOTICE "Fail to create version attribute!\n");

	device_register(&mybus_dev.dev);
	/*device_create(mybus_class, NULL, MKDEV(hello_major, 0), "hello" "%d", 0 );*/
	return ret;
}
EXPORT_SYMBOL(mybus_class);

static void my_bus_exit(void)
{
	mybus_unregister_device(&mybus_dev);
	bus_unregister(&my_bus_type.mybus_type);
	printk("my_bus_type.mybus_type.name :%s\n", my_bus_type.mybus_type.name);
}

/*module_init(my_bus_init);*/
/*module_exit(my_bus_exit);*/
