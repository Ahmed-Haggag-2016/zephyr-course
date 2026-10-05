#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/shell/shell.h>

/* Helper to safely retrieve our device instance inside shell callbacks */
static const struct device *get_sensor_device(void)
{
    return DEVICE_DT_GET(DT_NODELABEL(my_led_sensor));
}

/* 1. Subcommand: fetch */
static int cmd_fetch(const struct shell *sh, size_t argc, char **argv)
{
    const struct device *dev = get_sensor_device();

    if (!device_is_ready(dev)) {
        shell_error(sh, "Sensor device not ready!");
        return -ENODEV;
    }

    shell_print(sh, "Executing sensor_sample_fetch() via shell...");
    int ret = sensor_sample_fetch(dev);
    if (ret == 0) {
        shell_print(sh, "Fetch success!");
    } else {
        shell_error(sh, "Fetch failed with error: %d", ret);
    }
    return ret;
}

/* 2. Subcommand: read */
static int cmd_read(const struct shell *sh, size_t argc, char **argv)
{
    const struct device *dev = get_sensor_device();
    struct sensor_value val = {0};

    if (!device_is_ready(dev)) {
        shell_error(sh, "Sensor device not ready!");
        return -ENODEV;
    }

    shell_print(sh, "Executing sensor_channel_get() via shell...");
    int ret = sensor_channel_get(dev, SENSOR_CHAN_ALL, &val);
    if (ret == 0) {
        shell_print(sh, "Read Success! LED turned OFF.");
    } else {
        shell_error(sh, "Read failed with error: %d", ret);
    }
    return ret;
}

/* 3. Subcommand: info */
static int cmd_info(const struct shell *sh, size_t argc, char **argv)
{
    const struct device *dev = get_sensor_device();
    bool ready = device_is_ready(dev);

    shell_print(sh, "Device Name:  %s", dev->name);
    shell_print(sh, "Ready State:  %s", ready ? "READY" : "NOT READY");
    return 0;
}

/* Create the static subcommand array tree structure */
SHELL_STATIC_SUBCMD_SET_CREATE(sub_sensor,
    SHELL_CMD(fetch, NULL, "Trigger sensor_sample_fetch() -> Turn LED ON", cmd_fetch),
    SHELL_CMD(read,  NULL, "Trigger sensor_channel_get()  -> Turn LED OFF", cmd_read),
    SHELL_CMD(info,  NULL, "Print hardware device name and ready status", cmd_info),
    SHELL_SUBCMD_SET_END
);

/* Register the main root level command 'sensor' linked to your subcommands */
SHELL_CMD_REGISTER(sensor, &sub_sensor, "Custom LED Sensor driver control panel", NULL);
