/**
 * @file DhtDriver_Intf.h
 * @details Holds the interface required for the DHT Driver
 */
 #ifndef DHTDRIVER_INTF_H_
 #define DHTDRIVER_INTF_H_

/**
 * @brief Sensor type DHT11/21/22/AM2301/AM2302
 */
 #define DHTTYPE  (DHT22)

/**
 * @brief Sample time for the DHT in minutes
 */
#define DHT_SAMPLE_INTERVAL_MIN   (5)

 /**
  * @brief Sample time for the DHT in seconds
  */
 #define DHT_SAMPLE_INTERVAL_SEC   (DHT_SAMPLE_INTERVAL_MIN * 60)

 /**
  * @brief Sample time for the DHT in milli-seconds
  */
 #define DHT_SAMPLE_INTERVAL_MS   (DHT_SAMPLE_INTERVAL_SEC * 1000)

#endif
