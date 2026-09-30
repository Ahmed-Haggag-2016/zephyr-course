#define DT_DRV_COMPAT our_led_sensor  // Matches compatible: "our,led-sensor"

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>

/* Config structural layout matching instantiation requirements */
struct led_sensor_config {
    struct gpio_dt_spec led_gpio;
};

/* 1. sample_fetch: Talks to hardware -> Turns the LED ON */
static int led_sensor_sample_fetch(const struct device *dev, enum sensor_channel chan)
{
    const struct led_sensor_config *cfg = dev->config;
    
    // Turn the simulated LED ON (Value = 1)
    int ret = gpio_pin_set_dt(&cfg->led_gpio, 1);
    if (ret == 0) {
        printk("[DRIVER LOG] sensor_sample_fetch executed -> LED Turned ON!\n");
    }
    return ret;
}

/* 2. channel_get: Converts state -> Turns the LED OFF */
static int led_sensor_channel_get(const struct device *dev, enum sensor_channel chan, struct sensor_value *val)
{
    const struct led_sensor_config *cfg = dev->config;

    // Turn the simulated LED OFF (Value = 0)
    int ret = gpio_pin_set_dt(&cfg->led_gpio, 0);
    if (ret == 0) {
        printk("[DRIVER LOG] sensor_channel_get executed  -> LED Turned OFF!\n");
    }
    return ret;
}

/* Bind methods to the standard Zephyr Sensor API footprint */
static DEVICE_API(sensor, led_sensor_api) = {
    .sample_fetch = led_sensor_sample_fetch,
    .channel_get = led_sensor_channel_get,
};

/* Initialization routine to configure the target GPIO framework pin */
static int led_sensor_init(const struct device *dev)
{
    const struct led_sensor_config *cfg = dev->config;

    if (!gpio_is_ready_dt(&cfg->led_gpio)) {
        return -ENODEV;
    }

    // Prepare pin as a standard output path
    return gpio_pin_configure_dt(&cfg->led_gpio, GPIO_OUTPUT_INACTIVE);
}

/* Macro to generate internal device tracking records automatically */
#define LED_SENSOR_DEFINE(inst)                                        \
    static const struct led_sensor_config led_sensor_config_##inst = { \
        .led_gpio = GPIO_DT_SPEC_INST_GET(inst, led_gpios),           \
    };                                                                 \
                                                                       \
    DEVICE_DT_INST_DEFINE(inst,                                        \
                          led_sensor_init,                             \
                          NULL,                                        \
                          NULL,                                        \
                          &led_sensor_config_##inst,                   \
                          POST_KERNEL,                                 \
                          CONFIG_SENSOR_INIT_PRIORITY,                 \
                          &led_sensor_api);

DT_INST_FOREACH_STATUS_OKAY(LED_SENSOR_DEFINE)
