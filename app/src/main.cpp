// #include <zephyr/drivers/gpio.h>
// #include <zephyr/kernel.h>
// #include <zephyr/logging/log.h>

// //#define SLEEP_TIME_MS 1000

// /* The devicetree node identifier for the "led0" alias. */
// //#define LED_NODE DT_ALIAS(led0)
// #define LED_NODE DT_ALIAS(app_led)

// static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);

// LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

// int main(void)
// {
//     bool led_state = true;

//     if (!gpio_is_ready_dt(&led)) return 0;

//     if (gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE) < 0) return 0;

//     while (1) {
//         if (gpio_pin_toggle_dt(&led) < 0) return 0;

//         led_state = !led_state;
//         LOG_INF("LED state: %s", led_state ? "ON" : "OFF");
//         //k_msleep(SLEEP_TIME_MS);
//         //k_msleep(CONFIG_BLINK_SLEEP_TIME_MS);
//         k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS);
//     }
//     return 0;
// }

// #include <stdio.h>
// #include <zephyr/kernel.h>
// #include <zephyr/sys/printk.h>


// int main(void)
// {
// 	printk("Hello World! %s\n", CONFIG_BOARD_TARGET);


// 	return 0;
// }

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include "led_sensor.h"  // Include our custom extension header

int main(void)
{
    const struct device *dev = DEVICE_DT_GET(DT_NODELABEL(my_led_sensor));

    if (!device_is_ready(dev)) {
        printk("Custom LED Sensor driver device not ready!\n");
        return -ENODEV;
    }

    printk("=== Native Sim Extension API Test Started ===\n");

    // 1. Initial run showing the default initialized parameter state (100)
    sensor_sample_fetch(dev);
    k_msleep(500);
    struct sensor_value dummy;
    sensor_channel_get(dev, SENSOR_CHAN_ALL, &dummy);
    k_msleep(500);

    // 2. Call custom extension API function to change the runtime parameter value to 555
    led_sensor_set_custom_parameter(dev, 555);

    // 3. Next run to prove that the parameter inside the driver's structural data block updated successfully
    sensor_sample_fetch(dev);
    k_msleep(500);
    sensor_channel_get(dev, SENSOR_CHAN_ALL, &dummy);

    printk("=== Extension API Test Completed Natively ===\n");
    return 0;
}
