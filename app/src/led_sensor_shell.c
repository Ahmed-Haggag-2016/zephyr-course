#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/shell/shell.h>
#include "led_sensor.h"  // Include our custom API interface header

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

/* 4. New Subcommand with Argument Validation: set <value> */
static int cmd_set(const struct shell *sh, size_t argc, char **argv)
{
    const struct device *dev = get_sensor_device();

    if (!device_is_ready(dev)) {
        shell_error(sh, "Sensor device not ready!");
        return -ENODEV;
    }

    // Convert string parameter to integer
    int value = atoi(argv[1]);

    // Validate if the input value falls out of a specified safe range (e.g., 0 to 1000)
    if (value < 0 || value > 1000) {
        shell_error(sh, "Error: Value %d is out of range! (Allowed range: 0 - 1000)", value);
        return -EINVAL;
    }

    shell_print(sh, "Setting custom parameter to: %d", value);
    
    // Invoking my custom Extension API from L06 Task 2
    led_sensor_set_custom_parameter(dev, value);
    
    shell_print(sh, "Parameter successfully updated in dynamic data struct.");
    return 0;
}

/* Create the static subcommand array tree structure */
SHELL_STATIC_SUBCMD_SET_CREATE(sub_sensor,
    SHELL_CMD(fetch, NULL, "Trigger sensor_sample_fetch() -> Turn LED ON", cmd_fetch),
    SHELL_CMD(read,  NULL, "Trigger sensor_channel_get()  -> Turn LED OFF", cmd_read),
    SHELL_CMD(info,  NULL, "Print hardware device name and ready status", cmd_info),


    /* Enforce argument counts using SHELL_CMD_ARG:
     * mandatory arguments = 2 (the keyword "set" + the value parameter)
     * optional arguments  = 0
     */
    SHELL_CMD_ARG(set, NULL, "Set dynamic data parameter <value> (Range: 0-1000)", cmd_set, 2, 0),

    SHELL_SUBCMD_SET_END
);

/* Register the main root level command 'sensor' linked to your subcommands */
SHELL_CMD_REGISTER(sensor, &sub_sensor, "Custom LED Sensor driver control panel", NULL);
