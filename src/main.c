/*
 * Copyright (c) 2019 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/init.h>
#include <zephyr/pm/pm.h>
#include <zephyr/pm/device.h>
#include <zephyr/pm/policy.h>
#include <soc.h>
#include <hal/nrf_gpio.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/spi.h>

#include "sensors.h"

#define BUSY_WAIT_S 2U
#define SLEEP_S 2U
static const struct gpio_dt_spec led_red = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);

/* Prevent deep sleep (system off) from being entered on long timeouts
 * or `K_FOREVER` due to the default residency policy.
 *
 * This has to be done before anything tries to sleep, which means
 * before the threading system starts up between PRE_KERNEL_2 and
 * POST_KERNEL.  Do it at the start of PRE_KERNEL_2.
 */
static int disable_ds_1(const struct device *dev)
{
	ARG_UNUSED(dev);

	pm_policy_state_lock_get(PM_STATE_SOFT_OFF, PM_ALL_SUBSTATES);
	return 0;
}

SYS_INIT(disable_ds_1, PRE_KERNEL_2, 0);

void main(void)
{
	int rc;

	uint8_t my_buffer = 0xB9;
	struct spi_buf my_spi_buffer[1];
	my_spi_buffer[0].buf = &my_buffer;
	my_spi_buffer[0].len = 1;
	const struct spi_buf_set tx_buff = {my_spi_buffer, 1};

	const struct spi_dt_spec spi = SPI_DT_SPEC_GET(DT_ALIAS(spi_flash0), SPI_WORD_SET(8), 0);

	if(!spi_is_ready_dt(&spi))
	{
		printk("SPI not ready\n");
	}

	// rc = spi_write_dt(&spi, &tx_buff);
	// if(rc < 0)
	// {
	// 	printk("SPI Write failed (err %d)\n", rc);
	// }

	// const struct device *flash_dev = DEVICE_DT_GET(DT_ALIAS(spi_flash0));

	// if(!device_is_ready(flash_dev))
	// {
	// 	printk("Flash dev not ready\n");
	// }

	// rc = pm_device_action_run(flash_dev, PM_DEVICE_ACTION_TURN_OFF);
	// if(rc < 0)
	// {
	// 	printk("Error pm (err %d)\n", rc);
	// }


	// initSensors();

	gpio_pin_configure_dt(&led_red, GPIO_OUTPUT_INACTIVE);

	printk("\n%s system off demo\n", __TIME__);

	// k_sleep(K_FOREVER);

	gpio_pin_set_dt(&led_red, 1);

	k_msleep(1000);

	gpio_pin_set_dt(&led_red, 0);

	k_msleep(1000);

	NRF_CLOCK->TASKS_HFCLKSTOP;
	NRF_RTC0->TASKS_STOP;
	NRF_RTC1->TASKS_STOP;
	NRF_RTC2->TASKS_STOP;

	/* Above we disabled entry to deep sleep based on duration of
	 * controlled delay.  Here we need to override that, then
	 * force entry to deep sleep on any delay.
	 */
	// pm_state_force(0u, &(struct pm_state_info){PM_STATE_SOFT_OFF, 0, 0});

	/* Now we need to go sleep. This will let the idle thread runs and
	 * the pm subsystem will use the forced state. To confirm that the
	 * forced state is used, lets set the same timeout used previously.
	 */	
	k_sleep(K_SECONDS(SLEEP_S));

	// gpio_pin_set_dt(&led_red, 0);

	printk("ERROR: System off failed\n");
	while (true) {
		/* spin to avoid fall-off behavior */
	}

	
}
