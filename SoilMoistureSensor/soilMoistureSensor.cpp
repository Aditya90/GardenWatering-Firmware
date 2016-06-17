
#include "application.h"
#include "stdio.h"
#include "soilMoistureSensor.h"

/**
 * @brief Threshold of the reading below which the soil is considered dry
 */
static const int SOIL_MOISTURE_SENSOR_DRY_THRESHOLD = 250;

/**
 * @brief Threshold of the reading above which the soil is considered wet
 */
 static const int SOIL_MOISTURE_SENSOR_WET_THRESHOLD = 400;

/**
 * @brief Read the sensor value from the moisture sensor
 * @return Reading from the sensor
 */
int SOIL_MOISTURE_SENSOR::SoilMoistureSensor_Read(void)
{
   Serial.println("SoilMoistureSensor_Read called");
   //return (analogRead(_pinId));
}
