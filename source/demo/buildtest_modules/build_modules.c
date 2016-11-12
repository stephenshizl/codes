/************************************************************************
    > File Name: test.c
    > Author: chengdong
    > Mail: zchengdongdong@gmail.com 
    > Created Time: Sat 12 Nov 2016 03:23:09 PM CST
***********************************************************************/
#include <linux/init.h>  
#include <linux/kernel.h>  
#include <linux/module.h>  
static int funcIn(void)  
{  
    printk("in module");  
    return 0;  
}  
static void funcOut(void)  
{  
    printk("out module");  
    return;  
}  
module_init(funcIn);  
module_exit(funcOut);  
MODULE_LICENSE("GPL");  
MODULE_AUTHOR("chengdong");  

