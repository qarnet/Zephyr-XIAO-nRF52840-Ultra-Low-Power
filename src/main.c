#include <zephyr/device.h>
#include <zephyr/pm/pm.h>
#include <zephyr/pm/device.h>
#include <zephyr/pm/policy.h>
#include <zephyr/drivers/spi.h>

static const struct spi_dt_spec spi = SPI_DT_SPEC_GET(DT_ALIAS(spi_flash0), SPI_WORD_SET(8), 0);


void main(void)
{
	int rc;

	printk("\n%s low power demo\n", __TIME__);

	if(!device_is_ready(spi.bus))
	{
		printk("Flash dev not ready\n");
	}

	rc = pm_device_action_run(spi.bus, PM_DEVICE_ACTION_SUSPEND);
	if(rc < 0)
	{
		printk("State set failed (err %d)\n", rc);
	}

	k_sleep(K_FOREVER);
}
