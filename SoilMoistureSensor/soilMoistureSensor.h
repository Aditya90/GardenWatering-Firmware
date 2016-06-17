/**
 * @file soilMoistureSensor.h
 * @details This file holds the contents for the soil moisture sensor
 */
#ifndef SOIL_MOISTURE_SENSOR_INTF_H_
#define SOIL_MOISTURE_SENSOR_INTF_H_

class SOIL_MOISTURE_SENSOR
{
private:
   /**
    * @brief The pin which the sensors signal is assigned to
    */
   int _signalPinId;

   /**
    * @brief The pin which the sensors power is assigned to
    */
   int _powerPinId;

public:

   SOIL_MOISTURE_SENSOR(int signalPinIdNew, int powerPinIdNew) :
      _signalPinId(signalPinIdNew), _powerPinId(powerPinIdNew)
   {
      // The signal pin is an input
      pinMode(signalPinIdNew, INPUT);

      // The power pin is an output
      pinMode(powerPinIdNew, OUTPUT);

      // Turn the sensor off on init to increase the lifetime of the sensor
      digitalWrite(powerPinIdNew, LOW);
   }

   virtual ~SOIL_MOISTURE_SENSOR()
   {
      // Turn the sensor off if the object is ever destroyed to increase the
      // lifetime of the sensor
      digitalWrite(_powerPinId, LOW);
   }


   int SoilMoistureSensor_Read(void);
};

#endif
