/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdlib.h>

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/regulator.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/sensor/npm1300_charger.h>
#include <zephyr/drivers/led.h>
#include <zephyr/dt-bindings/regulator/npm1300.h>
#include <zephyr/drivers/mfd/npm1300.h>
#include <zephyr/sys/printk.h>
#include <getopt.h>

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   1000

/* The devicetree node identifier for the "led0" alias. */
#define LED0_NODE DT_ALIAS(led0)

/*
 * A build error on this line means your board is unsupported.
 * See the sample documentation for information on how to fix this.
 */
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
static const struct device *ldo1 = DEVICE_DT_GET(DT_NODELABEL(npm1300_ldo1));

int main(void)
{
	int ret;

	if (!gpio_is_ready_dt(&led)) {
		return 0;
	}

	ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);

	if (ret < 0) {
		return 0;
	}

	if (!device_is_ready(ldo1)) {
		printk("Error: Regulator device is not ready\n");
		return 0;
	}

	size_t count=0;
	regulator_set_voltage(ldo1, 3000000, 3000000);
	regulator_enable(ldo1);
	while (1) {
		ret = gpio_pin_toggle_dt(&led);
		if(count%2){
			regulator_enable(ldo1);
		}else{
			regulator_disable(ldo1);
		}

		count++;
		if (ret < 0) {
			return 0;
		}
		k_msleep(SLEEP_TIME_MS);
	}
	return 0;
}
