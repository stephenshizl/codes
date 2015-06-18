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
#include <linux/mybus_driver.h>
#include <linux/cdev.h>

#include <linux/fs.h>

MODULE_LICENSE("Dual BSD/GPL");

extern struct class *mybus_class;
static struct cdev cdev;
static dev_t dev = 2;

static int mybus_major = 58;
static int mybus_minor = 0;
static int number_of_devices = 1;

struct file_operations mybus_dev1_ops = {
	.owner = THIS_MODULE,
	.open = mybus_dev1_open,
	.release = mybus_dev1_release,
	.ioctl = mybus_dev1_ioctl,
};

struct miscdevice mybus_char_device = {
		
	.minor = MISC_DYNAMIC_MINOR,
	.name = "mybus_dev1_file",
	.fops = &mybus_dev1_ops,
};

static int mybus_dev1_open(struct inode *inode, struct file *file)
{
	struct mybus_driver_data *mybus_drv_data;
	struct mybus_device *mybus_dev1;

	printk("mybus_driver func:%s\n", __func__);

	mybus_dev1 = container_of(mybus_char_device.parent, struct mybus_device, dev);
	printk("mybus_driver func:%s\n", __func__);

	mybus_drv_data = mybus_get_clientdata(mybus_dev1);
	printk("mybus_driver func:%s\n", __func__);
	printk("mybus_data statu:%d\n", mybus_drv_data->statu);
	return 0;
}

static int mybus_dev1_release(struct inode *inode, struct file *file)
{
	printk("mybus_driver func:%s\n", __func__);
	return 0;
}

static int mybus_dev1_ioctl(struct inode *inode, struct file *file, unsigned int cmd, unsigned long arg)
{
	printk("mybus_driver func:%s\n", __func__);
	return 0;
}

static void char_reg_setup_cdev (void)
{
		int error, devno = MKDEV(mybus_major, mybus_minor);
		cdev_init(&cdev, &mybus_dev1_ops);
		cdev.owner = THIS_MODULE;
		/*cdev.ops = &hello_fops;*/
		error = cdev_add(&cdev, devno , 1);
		if (error)
			printk(KERN_NOTICE "Error %d adding char_reg_setup_cdev", error);

}

struct class *my_class;
static int my_probe(struct mybus_device *pdev)
{
	const struct mybus_driver_info *mybus_drv;
	struct mybus_driver_data *mybus_drv_data;
	struct input_dev *input_dev;
	int ret;

	printk("mybus_driver func:%s\n", __func__);

	mybus_drv = pdev->dev.platform_data;

	mybus_drv_data = kmalloc(sizeof(struct mybus_driver_data), GFP_KERNEL);
	mybus_drv_data->statu = 1;

	/*创建misc设备*/
	mybus_char_device.parent = &pdev->dev;
	/*ret = misc_register(&mybus_char_device);*/
	mybus_set_clientdata(pdev, mybus_drv_data);
	
	/*创建字符设备*/
	dev = MKDEV(mybus_major, mybus_minor);
	ret = register_chrdev_region(dev, number_of_devices, "mybus_chrdev");
	cdev_init(&cdev, &mybus_dev1_ops);
	cdev.owner = THIS_MODULE;
	ret = cdev_add(&cdev, dev, 1);
	if(ret)
		printk("cdev_add err ret :%d\n", ret);

	/*参照input创建字符设备*/
	/*ret = register_chrdev(MYBUS_MAJOR, "mybus_chrdev", &mybus_dev1_ops);*/

	/*dev = MKDEV(mybus_major, mybus_minor);*/
	/*ret = register_chrdev_region(dev, number_of_devices, "mybus_chrdev");*/
	/*if(ret) {*/
		/*printk(KERN_ERR "mybus : unable to register char major %d", MYBUS_MAJOR);*/
	/*}*/
	/*char_reg_setup_cdev();*/
	/*my_class = class_create(THIS_MODULE, "my_class");*/
	/*device_create(my_class, NULL, MKDEV(mybus_major, 0), "mybus_dev6" "%s", "mybus_dev1_class_devfile");*/
	
	printk("mybus_driver mybus_drv.name = %s, mybus_drv.dev_file = %s\n", pdev->name, mybus_drv->dev_file);

	return 0;
}

static int my_suspend(struct mybus_device *dev)
{
	printk("mybus_driver func:%s\n", __func__);
	return 0;
}

static int my_resume(struct mybus_device *dev)
{
	printk("mybus_driver func:%s\n", __func__);
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
