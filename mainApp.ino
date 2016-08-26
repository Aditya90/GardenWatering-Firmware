/**
 * @file mainApp.ino
 * @details The main application control file
 */

// -------------------------
// --- INCLUDES ---
// -------------------------
#include "stdio.h"
#include "webIntf.h"
#include "hardwareConfig.h"
#include "motorClass.h"
#include "PietteTech_DHT.h"
#include "DhtDriver_Intf.h"
#include "soilMoistureSensor.h"
#include "soilMoistureSensorManager.h"

// -------------------------
// --- PRIVATE DEFINES ---
// -------------------------

// -------------------------
// --- PRIVATE TYPES ---
// -------------------------

/**
 * @brief RX message status list to pass locally in this file
 */
typedef enum
{
   RX_MSG_SUCCESS = 0,
   RX_MSG_FAILURE,
   MAX_NUM_RX_MSG_CODE
}RX_MSG_STATUS;

// -------------------------
// --- PRIVATE FUNCTION DECLARATIONS ---
// -------------------------

static void main_motorsInit(void);
static void main_sensorsInit(void);
static int main_motorSpeedChange(String command);
static void main_rxMsgUiCallback(RX_MSG_STATUS rxMsgStatus);
static void main_initMotorOutputs(MotorClass* motor, int pinNumber, MOTOR_TYPE motorType);
static void main_dhtLoop(void);
static void main_dhtWrapper(void);

// -------------------------
// --- PRIVATE VARIABLES ---
// -------------------------

/**
 * @brief Object for the first motor instance
 */
MotorClass plantMotor1;

/**
 * @brief Instantiate an instance of the DHT
 */
PietteTech_DHT DHT(DHT_PIN, DHTTYPE, main_dhtWrapper);

/**
 * @brief Instantiate a soil moisture sensor
 */
SOIL_MOISTURE_SENSOR *soilMoistureSensor1;

/**
 * @brief Declare an instance of the soil moisture sensor manager
 */
SOIL_MOISTURE_SENSOR_MANAGER *soilMoistureSensorManager;

// -------------------------
// --- PUBLIC VARIABLES ---
// -------------------------

/**
 * @brief Particle cloud variable for temperature value in celsius
 */
int ParticleCloudVar_TemperatureValueFahrenheit;

/**
 * @brief Particle cloud variable for humidity in percentage
 */
int ParticleCloudVar_HumidityPercentage;

/**
 * @brief Particle cloud variable for moisture in percentage
 */
int ParticleCloudVar_MoisturePercentage;

// -------------------------
// --- PUBLIC FUNCTION DEFINITIONS ---
// -------------------------

/**
 * @brief setup function for the application
 */
void setup()
{
   // Code working indication
   pinMode(RX_STATUS_LED_PIN, OUTPUT);
   digitalWrite(RX_STATUS_LED_PIN, HIGH);

   // Init the motors
   main_motorsInit();

   // Init the sensors
   main_sensorsInit();

   // Begin USB serial communication
   Serial.begin(9600);
}

void loop()
{
   main_dhtLoop();
}

// -------------------------
// --- PRIVATE FUNCTION DEFINITIONS ---
// -------------------------

/**
 * @brief Initialize all the sensors in the circuit
 */
static void main_sensorsInit(void)
{
   // Initialize the cloud variables
   ParticleCloudVar_TemperatureValueFahrenheit = 0;
   ParticleCloudVar_HumidityPercentage = 0;
   ParticleCloudVar_MoisturePercentage = 0;

   // Register the temperature and humidity as particle cloud variables
   Particle.variable(REST_API_TEMPERATURE_VAR, &ParticleCloudVar_TemperatureValueFahrenheit, INT);
   Particle.variable(REST_API_HUMIDITY_VAR, &ParticleCloudVar_HumidityPercentage, INT);
   Particle.variable(REST_API_MOISTURE_VAR, &ParticleCloudVar_MoisturePercentage, INT);

   // Initialize the soil moisture sensor 1
   soilMoistureSensor1 = new SOIL_MOISTURE_SENSOR(SOIL_MOISTURE_SENSOR_1_SIGNAL_PIN,
      SOIL_MOISTURE_SENSOR_1_VCC_PIN);

   // Initialize an instance of the soil moisture sensor manager
   soilMoistureSensorManager = new SOIL_MOISTURE_SENSOR_MANAGER();
}

/**
 * @brief Initialize all the motors in the circuit
 */
static void main_motorsInit(void)
{
   // Init Motor Output Pin
   main_initMotorOutputs(&plantMotor1, MOTOR_1_PIN, MOTOR_1_TYPE);

   // Init the web app and point the cloud function with the local motor
   // control function
   Particle.function(REST_API_MOTOR_CONTROL_FN, main_motorSpeedChange);
}

/**
 * @brief Wrapper required for instantiating the DHT object
 */
static void main_dhtWrapper(void)
{
    DHT.isrCallback();
}

/**
 * @brief The main dht loop
 */
static void main_dhtLoop(void)
{
   static bool DHT_HasStarted = FALSE; // flag to indicate we started acquisition
   static unsigned long DHT_NextSampleTime = 0;  // Start the first sample immediately

  // Check if we need to start the next sample
  if (millis() > DHT_NextSampleTime)
  {
      if (!DHT_HasStarted)
      {
          DHT.acquire();
          DHT_HasStarted = true;
      }

       if (!DHT.acquiring())
       {
          // get DHT status
          int result = DHT.getStatus();

          switch (result)
          {
            case DHTLIB_OK:
            {
               // Don't print anything if the reading was valid
               break;
            }
            default:
            {
               Serial.println("Unknown error : ");
               Serial.print(result);
               Serial.print("\n");
               break;
            }
          }

          ParticleCloudVar_HumidityPercentage = DHT.getHumidity();
          char humidityString[20];
          sprintf(humidityString, "%d", ParticleCloudVar_HumidityPercentage);

          ParticleCloudVar_TemperatureValueFahrenheit = DHT.getFahrenheit();
          char temperatureString[20];
          sprintf(temperatureString, "%d", ParticleCloudVar_TemperatureValueFahrenheit);

          Particle.publish(REST_API_HUMIDITY_VAR, humidityString);
          Particle.publish(REST_API_TEMPERATURE_VAR, temperatureString);

          // reset the sample flag so we can take another
          DHT_HasStarted = false;
          // set the time for next sample - This will eventually wrap around
          DHT_NextSampleTime = millis() + DHT_SAMPLE_INTERVAL_MS;
      }
   }
}

/**
 * @brief Init the motor outputs
 * @param motor Motor to initialize
 * @param pinNumber The pin number to tie the motor class to
 * @param motorType Type of motor to use - Switch or PWM
 */
static void main_initMotorOutputs(MotorClass* motor, int pinNumber, MOTOR_TYPE motorType)
{
  // Setup pinNumber to connect to a motor of motorType
  Motor_new(motor, pinNumber, motorType);
}

/**
 * @brief Change the speed of the motor
 * @param command Command sent to the motor from the cloud
 */
static int main_motorSpeedChange(String command)
{
   int retVal;

  if (command == CMD_SPEED_INCREASE)
  {
    Motor_updateSpeed(&plantMotor1, SPEED_INCREASE);
    main_rxMsgUiCallback(RX_MSG_SUCCESS);
    retVal = RX_MSG_RETVAL_SUCCESS;
  }
  else if (command == CMD_SPEED_DECREASE)
  {
    Motor_updateSpeed(&plantMotor1, SPEED_DECREASE);
    main_rxMsgUiCallback(RX_MSG_SUCCESS);
    retVal = RX_MSG_RETVAL_SUCCESS;
  }
  else if (command == CMD_SPEED_STOP)
  {
    Motor_updateSpeed(&plantMotor1, SPEED_STOP);
    main_rxMsgUiCallback(RX_MSG_SUCCESS);
    retVal = RX_MSG_RETVAL_SUCCESS;
  }
  else if (command == CMD_SPEED_RESET)
  {
    // Reset the speed of the motor to default
    Motor_updateSpeed(&plantMotor1, SPEED_RESET);
    main_rxMsgUiCallback(RX_MSG_SUCCESS);
    retVal = RX_MSG_RETVAL_SUCCESS;
  }
  else
  {
    // IF we cannot recognize the command, reset the speed to default
    Motor_updateSpeed(&plantMotor1, SPEED_RESET);
    main_rxMsgUiCallback(RX_MSG_FAILURE);
    retVal = RX_MSG_RETVAL_FAILURE;
  }

  return retVal;
}

/**
 * @brief UI indication for RX of web message
 * @param Status of the recieved message
 */
static void main_rxMsgUiCallback(RX_MSG_STATUS rxMsgStatus)
{
   switch(rxMsgStatus)
   {
      case RX_MSG_SUCCESS:
      {
         // Flash LED 5 times
         for (int i=0;i<5;i++)
         {
            digitalWrite(RX_STATUS_LED_PIN, LOW);
            delay(100);
            digitalWrite(RX_STATUS_LED_PIN, HIGH);
            delay(100);
         }
         break;
      }
      case RX_MSG_FAILURE:
      default:
      {
         // Keep LED off for 5 sec
         digitalWrite(RX_STATUS_LED_PIN, LOW);
         delay(5000);
         digitalWrite(RX_STATUS_LED_PIN, HIGH);
         break;
      }
   }

}
