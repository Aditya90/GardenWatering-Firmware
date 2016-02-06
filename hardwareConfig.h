/**
 * @file hardwareConfig.h
 * @details Holds details about pin configurations and hardware connections
 */
 #ifndef HARDWARE_CONFIG_H
 #define HARDWARE_CONFIG_H

/**
 * @ brief The pin onto which the RX status LED is connected
 */
#define RX_STATUS_LED_PIN  (D7)

/**
 * @ brief The pin onto which the 1st motor is connected
 */
#define MOTOR_1_PIN  (D1)

/**
 * @ brief The type of control for motor 1
 */
#define MOTOR_1_TYPE  (MOTOR_SWITCH)

 /**
  * @brief Digital pin for communications with the DHT22
  */
 #define DHT_PIN   D2

#endif
