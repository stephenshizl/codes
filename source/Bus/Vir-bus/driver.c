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
#include <linux/bus_basic.h>

MODULE_LICENSE("Dual BSD/GPL");

static int my_probe(struct device *dev)
{

	printk("mybus_driver func:%s\n", __func__);
	printk("Driver found device which my driver can handle!\n");
	return 0;
}

static int my_suspend(struct device *dev)
{
	printk("mybus_driver func:%s\n", __func__);
	printk("Driver found device which my driver can handle!\n");
	return 0;
}

static int my_resume(struct device *dev)
{
	printk("mybus_driver func:%s\n", __func__);
	printk("Driver found device which my driver can handle!\n");
	return 0;
}

static struct mybus_driver mybus_drv= {
		.driver = {
			.name = "mybus_dev1"
		},
		.probe = my_probe,
		.suspend = &my_suspend,
		.resume = &my_resume,
};

static int __init my_driver_init(void)
{
	int ret;
	 
	/*注册驱动*/
	/*driver_register(&my_driver);*/
	ret = mybus_register_driver(&mybus_drv);

	return ret;
}

static void my_driver_exit(void)
{
	mybus_unregister_driver(&mybus_drv);
}

module_init(my_driver_init);
module_exit(my_driver_exit);
