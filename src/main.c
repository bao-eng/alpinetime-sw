/*
 * Copyright (c) 2023 Fabian Blatz <fabianblatz@gmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/drivers/display.h>
#include <lvgl.h>
#include <lv_demos.h>

#include <zephyr/drivers/regulator.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/sensor/npm1300_charger.h>
#include <zephyr/drivers/led.h>
#include <zephyr/dt-bindings/regulator/npm1300.h>
#include <zephyr/drivers/mfd/npm1300.h>

#define LOG_LEVEL CONFIG_LOG_DEFAULT_LEVEL
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(app);

static const struct device *ldo1 = DEVICE_DT_GET(DT_NODELABEL(npm1300_ldo1));

int main(void)
{
	
	if (!device_is_ready(ldo1)) {
		printk("Error: Regulator device is not ready\n");
		return 0;
	}

	regulator_set_voltage(ldo1, 3000000, 3000000);
	regulator_enable(ldo1);
	
	const struct device *display_dev;

	display_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
	if (!device_is_ready(display_dev)) {
		LOG_ERR("Device not ready, aborting test");
		return 0;
	}

#if defined(CONFIG_LV_USE_DEMO_MUSIC)
	lv_demo_music();
#elif defined(CONFIG_LV_USE_DEMO_BENCHMARK)
	lv_demo_benchmark();
#elif defined(CONFIG_LV_USE_DEMO_STRESS)
	lv_demo_stress();
#elif defined(CONFIG_LV_USE_DEMO_WIDGETS)
	lv_demo_widgets();
#else
#error Enable one of the demos CONFIG_LV_USE_DEMO_MUSIC, CONFIG_LV_USE_DEMO_BENCHMARK ,\
	CONFIG_LV_USE_DEMO_STRESS, or CONFIG_LV_USE_DEMO_WIDGETS
#endif

	lv_task_handler();
	display_blanking_off(display_dev);

	while (1) {
		k_msleep(lv_task_handler());
	}

	return 0;
}
