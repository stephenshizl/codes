/*************************************************************************
    > File Name: include/linux/bus.h
    > Author: chengdong
    > Mail: zchengdongdong@gmail.com 
    > Created Time: Mon 10 Dec 2012 09:55:16 AM CST
 ************************************************************************/

#include <linux/device.h>

struct mybus_driver;
struct mybus_device;

struct mybus {
	struct bus_type mybus_type;
};

static struct class *mybus_class;
//struct class mybus_class = {
	//.name		= "mybus_class",
//};

extern int mybus_register_device(struct mybus_device *device);
extern int mybus_add_device(struct mybus_device **device, unsigned len);
extern int mybus_unregister_device(struct mybus_device *device);

extern int mybus_register_driver(struct mybus_driver *drv);
extern int mybus_unregister_driver(struct mybus_driver *drv);

struct mybus_driver {

	struct device_driver driver;
	struct bus_device_id *id_table;
	int (*probe)(struct mybus_device *dev);
	int (*remove)(struct mybus_device *dev);
	int (*suspend)(struct mybus_device *dev, pm_message_t mesg);
	int (*resume)(struct mybus_device *dev);

};

struct mybus_device {

	char *name;
	int irq;
	unsigned short flage;
	unsigned short addr;
	struct device dev;
};

static inline void mybus_set_clientdata(struct mybus_device *pdev, void *data)
{
	dev_set_drvdata(&pdev->dev,data);
}

static inline void *mybus_get_clientdata(struct mybus_device *dev)
{
	return dev_get_drvdata(&dev->dev);
}
