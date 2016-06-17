/**
 * @file webIntf.h
 * @details Interface information between the web APIs and the firmware
 */
#ifndef WEB_INTF_H
#define WEB_INTF_H

/**
 * @brief Rest api function used to interact with the web interface
 */
#define REST_API_MOTOR_CONTROL_FN "motorControl"

/**
 * @brief The REST varialbe for temperature in units of Fahrenheit
 */
#define REST_API_TEMPERATURE_VAR "Temperature_Fahrenheit"

/**
 * @brief The REST varialbe for humidity in units of percentage
 */
#define REST_API_HUMIDITY_VAR "Humidity_Percentage"

/**
 * @brief The REST varialbe for moisture in units of percentage
 */
#define REST_API_MOISTURE_VAR "Moisture_Percentage"

/**
 * @brief Command to increase speed of the motor
 */
#define CMD_SPEED_INCREASE "increase"

/**
 * @brief Command to decrease speed of the motor
 */
#define CMD_SPEED_DECREASE "decrease"

/**
 * @brief Command to reset the speed of the motor
 */
#define CMD_SPEED_RESET "reset"

/**
 * @brief Command to stop the motor
 */
#define CMD_SPEED_STOP "stop"

/**
 * @brief List of return values from the firmware to the web interface
 */
typedef enum
{
   RX_MSG_RETVAL_FAILURE = 0,
   RX_MSG_RETVAL_SUCCESS,
   MAX_NUM_RX_MSG_RETVALS
}RX_MSG_RETVALS;

/**
 * @brief Particle cloud variable for temperature value in Fahrenheit
 */
extern int ParticleCloudVar_TemperatureValueFahrenheit;

/**
 * @brief Particle cloud variable for humidity in percentage
 */
extern int ParticleCloudVar_HumidityPercentage;

/**
 * @brief Particle cloud variable for moisture in percentage
 */
extern int ParticleCloudVar_MoisturePercentage;


#endif
