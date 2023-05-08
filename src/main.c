#include <zephyr/device.h>
#include <zephyr/pm/pm.h>
#include <zephyr/pm/device.h>
#include <zephyr/pm/policy.h>
#include <zephyr/drivers/spi.h>

static const struct device *const spi_bus = DEVICE_DT_GET(DT_BUS(DT_ALIAS(spi_flash0)));

void main(void)
{
	int rc;

	if(!device_is_ready(spi_bus))
	{
		return;
	}

	rc = pm_device_action_run(spi_bus, PM_DEVICE_ACTION_SUSPEND);
	if(rc < 0)
	{
		return;
	}

	k_sleep(K_FOREVER);
}
