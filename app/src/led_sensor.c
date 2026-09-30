#define DT_DRV_COMPAT our_led_sensor  // Matches compatible: "our,led-sensor"

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>
#include "led_sensor.h"  // Include our custom API interface header

/* Constant configuration struct (Stored in Flash memory) */
struct led_sensor_config {
    struct gpio_dt_spec led_gpio;
};

/* Dynamic data struct for mutable runtime parameters (Stored in RAM) */
struct led_sensor_data {
    int custom_param;  // The parameter to be modified by our custom extension API
};

/* Implementation of the Custom Extension API Function */
void led_sensor_set_custom_parameter(const struct device *dev, int new_val)
{
    // Retrieve the dynamic runtime data pointer out of the generic device instance
    struct led_sensor_data *data = (struct led_sensor_data *)dev->data;

    // Modify the variable in the dynamic data struct
    data->custom_param = new_val;

    printk("[CUSTOM API LOG] led_sensor_set_custom_parameter executed! New data->custom_param value = %d\n", 
           data->custom_param);
}

/* Standard sample_fetch: Turns the LED ON */
static int led_sensor_sample_fetch(const struct device *dev, enum sensor_channel chan)
{
    const struct led_sensor_config *cfg = dev->config;
    const struct led_sensor_data *data = dev->data; // Read data if needed
    
    int ret = gpio_pin_set_dt(&cfg->led_gpio, 1);
    if (ret == 0) {
        printk("[DRIVER LOG] sample_fetch called (Current param = %d) -> LED ON!\n", data->custom_param);
    }
    return ret;
}

/* Standard channel_get: Turns the LED OFF */
static int led_sensor_channel_get(const struct device *dev, enum sensor_channel chan, struct sensor_value *val)
{
    const struct led_sensor_config *cfg = dev->config;

    int ret = gpio_pin_set_dt(&cfg->led_gpio, 0);
    if (ret == 0) {
        printk("[DRIVER LOG] channel_get called -> LED OFF!\n");
    }
    return ret;
}

static DEVICE_API(sensor, led_sensor_api) = {
    .sample_fetch = led_sensor_sample_fetch,
    .channel_get = led_sensor_channel_get,
};

static int led_sensor_init(const struct device *dev)
{
    const struct led_sensor_config *cfg = dev->config;
    struct led_sensor_data *data = dev->data;

    // Set a baseline starting value inside our dynamic data parameter
    data->custom_param = 100;

    if (!gpio_is_ready_dt(&cfg->led_gpio)) {
        return -ENODEV;
    }

    return gpio_pin_configure_dt(&cfg->led_gpio, GPIO_OUTPUT_INACTIVE);
}

/* Updated Macro: Passes the runtime data structure references into the definition layout */
#define LED_SENSOR_DEFINE(inst)                                        \
    static struct led_sensor_data led_sensor_data_##inst;              \
    static const struct led_sensor_config led_sensor_config_##inst = { \
        .led_gpio = GPIO_DT_SPEC_INST_GET(inst, led_gpios),           \
    };                                                                 \
                                                                       \
    DEVICE_DT_INST_DEFINE(inst,                                        \
                          led_sensor_init,                             \
                          NULL,                                        \
                          &led_sensor_data_##inst,                     \
                          &led_sensor_config_##inst,                   \
                          POST_KERNEL,                                 \
                          CONFIG_SENSOR_INIT_PRIORITY,                 \
                          &led_sensor_api);

DT_INST_FOREACH_STATUS_OKAY(LED_SENSOR_DEFINE)
