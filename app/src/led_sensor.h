#ifndef OUR_LED_SENSOR_H_
#define OUR_LED_SENSOR_H_

#include <zephyr/device.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Custom driver extension API to update a dynamic runtime parameter.
 * 
 * @param dev Pointer to the custom sensor device instance.
 * @param new_val The new integer configuration value to save in the data struct.
 */
void led_sensor_set_custom_parameter(const struct device *dev, int new_val);

#ifdef __cplusplus
}
#endif

#endif /* OUR_LED_SENSOR_H_ */
