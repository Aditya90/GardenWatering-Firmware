/**
 * @file soilMoistureSensorManager.h
 * @details This module manages all the soil moisture sensors connected to the
 *    device
 */
#ifndef SOIL_MOISTURE_SENSOR_MANAGER_H_
#define SOIL_MOISTURE_SENSOR_MANAGER_H_

#include "application.h"

class SOIL_MOISTURE_SENSOR_MANAGER
{
   private:
      static const int POLLING_PERIOD_MILLISEC_DEFAULT = 5000;
      int _pollingPeriodMilliSec;
      Timer* _timerHandler;

   public:
      SOIL_MOISTURE_SENSOR_MANAGER():_pollingPeriodMilliSec(POLLING_PERIOD_MILLISEC_DEFAULT)
      {
         _timerHandler = new Timer(_pollingPeriodMilliSec,
            this->SoilMoistureSensorManager_TaskHandler);
         _timerHandler.start();
      }

      virtual ~SOIL_MOISTURE_SENSOR_MANAGER()
      {
         _timerHandler = nullptr;
      }

      void SoilMoistureSensorManager_TaskHandler(void);
};

#endif
