/*
 * wifi_power.c
 *
 * Yongle Lai @ Rockchip Fuzhou @ 20100303.
 *
 * Power control for WIFI module.
 *
 * There are Power supply and Power Up/Down controls for WIFI typically.
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/jiffies.h>
#include <linux/regulator/machine.h>
#include <linux/err.h>

#include "wifi_power.h"

#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,25)
#include <asm/arch/gpio.h>
#include <asm/arch/iomux.h>
#else
#include <mach/gpio.h>
#include <mach/iomux.h>
#endif


#if (WIFI_GPIO_POWER_CONTROL == 1)

extern struct wifi_power power_gpio;
extern struct wifi_power power_save_gpio;
extern struct wifi_power power_reset_gpio;

#define OS_IOMUX(name, value) rk29_mux_api_set((name), (value));

int wifi_gpio_operate(struct wifi_power *gpio, int flag)
{
	int sensitive;
	
	if (gpio->use_gpio == POWER_NOT_USE_GPIO)
		return 0;
	
	if (gpio->gpio_iomux == POWER_GPIO_IOMUX)
	{
		OS_IOMUX(gpio->iomux_name, gpio->iomux_value);
	}
	
	if (flag == GPIO_SWITCH_ON)
		sensitive = gpio->sensi_level;
	else
		sensitive = 1 - gpio->sensi_level;

	printk(KERN_ERR "@@@@@@@@@@@@@ %s sensitive :%d\n",__FUNCTION__,sensitive);

	if (gpio->use_gpio == POWER_USE_EXT_GPIO)
	{
		printk(KERN_ERR "@@@@@@@@@@@@@ %s ***********\n",__FUNCTION__);
		wifi_extgpio_operation(gpio->gpio_id, sensitive);
	}
	else
	{
		int ret;

		ret = gpio_request(gpio->gpio_id, NULL);
		if (ret != 0)
			printk("Request GPIO for WIFI POWER error!\n");

		gpio_direction_output(gpio->gpio_id, sensitive);
		gpio_set_value(gpio->gpio_id, sensitive);

		gpio_free(gpio->gpio_id);
	}

	return 0;
}

/*
 * WiFi power up sequence
 */
int wifi_turn_on_rtl8192c_card(void)
{
	/*int ret;*/
	/*ret = gpio_request(RK29_PIN5_PD6, NULL);*/
	/*if (ret != 0)*/
	/*printk("Request GPIO for WIFI POWER error!\n");*/

	/*gpio_direction_output(RK29_PIN5_PD6, 0);*/
	/*msleep(1000);*/
	/*//gpio_direction_output(RK29_PIN5_PD6, 1);*/
	/*gpio_set_value(RK29_PIN5_PD6, 1);*/

	//wifi_gpio_operate(&power_gpio, GPIO_SWITCH_OFF);
	//msleep(1000);
	wifi_gpio_operate(&power_gpio, GPIO_SWITCH_ON);
	if (power_gpio.use_gpio != POWER_NOT_USE_GPIO)
			msleep(1000);

	return 0;
}

int wifi_turn_on_card(int module)
{
	struct regulator *wifi_regulator;
	int ret;

	wifi_turn_on_callback();

	wifi_regulator = regulator_get(NULL, "vmmc");
	if (IS_ERR_OR_NULL(wifi_regulator)) {
		printk(KERN_ERR "Failed to get wifi regulator.\n");
		return -1;
	}
	ret = regulator_set_voltage(wifi_regulator, 3300000, 3300000);
	if (ret < 0)
		printk(KERN_ERR "Failed to set wifi regulator voltage.\n");

	regulator_force_disable(wifi_regulator);
	msleep(100);
	ret = regulator_enable(wifi_regulator);
	if (ret < 0)
		printk(KERN_ERR "Failed to enable wifi regulator.\n");
	regulator_put(wifi_regulator);

	wifi_turn_on_rtl8192c_card();
	msleep(4000);
	
	return 0;
}

int wifi_turn_off_card(void)
{
	struct regulator *wifi_regulator;
	int ret;

	wifi_gpio_operate(&power_gpio, GPIO_SWITCH_OFF);

	wifi_regulator = regulator_get(NULL, "vmmc");
	if (IS_ERR_OR_NULL(wifi_regulator)) {
		printk(KERN_ERR "Failed to get wifi regulator.\n");
		return -1;
	}
	ret = regulator_disable(wifi_regulator);
	if (ret < 0)
		printk(KERN_ERR "Failed to disable wifi regulator.\n");
	regulator_put(wifi_regulator);

	msleep(5);

	wifi_turn_off_callback();
	
	return 0;
}

void rockchip_wifi_shutdown(void)
{
	printk("rockchip_wifi_shutdown....\n");

	wifi_turn_off_card();
}
EXPORT_SYMBOL(rockchip_wifi_shutdown);

#endif /* WIFI_GPIO_POWER_CONTROL */

