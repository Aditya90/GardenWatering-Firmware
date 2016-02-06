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
PietteTech_DHT DHT(DHTPIN, DHTTYPE, main_dhtWrapper);

// -------------------------
// --- PUBLIC VARIABLES ---
// -------------------------

/**
 * @brief Particle cloud variable for temperature value in celsius
 */
int ParticleCloudVar_TemperatureValueCelsius;

/**
 * @brief Particle cloud variable for humidity in percentage
 */
int ParticleCloudVar_HumidityPercentage;

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

   // Init Motor Output Pin
   main_initMotorOutputs(&plantMotor1, MOTOR_1_PIN, MOTOR_1_TYPE);

   // Init the web app and point the cloud function with the local motor
   // control function
   Particle.function(REST_API_MOTOR_CONTROL_FN, main_motorSpeedChange);

   // Initialize the cloud variables
   ParticleCloudVar_TemperatureValueCelsius = 0;
   ParticleCloudVar_HumidityPercentage = 0;

   // Register the temperatue and humidity as particle cloud variables
   Particle.variable("TemperatureValueCelsius", &ParticleCloudVar_TemperatureValueCelsius, INT);
   Particle.variable("HumidityPercentage", &ParticleCloudVar_HumidityPercentage, INT);

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
   static int DHT_SampleCounter = 0;  // counter
   static unsigned int DHT_NextSampleTime = 0;  // Start the first sample immediately

  // Check if we need to start the next sample
  if (millis() > DHT_NextSampleTime)
  {
      if (!DHT_HasStarted)
      {
          Serial.print("\n");
          Serial.print(DHT_SampleCounter);
          Serial.print(": Retrieving information from sensor: ");
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

          ParticleCloudVar_TemperatureValueCelsius = DHT.getFahrenheit();
          char temperatueString[20];
          sprintf(temperatueString, "%d", ParticleCloudVar_TemperatureValueCelsius);

          Particle.publish("Humidity_Percentage", humidityString);
          Particle.publish("Temperature_Celsius", temperatueString);

          DHT_SampleCounter++;  // increment counter
          DHT_HasStarted = false;  // reset the sample flag so we can take another
          DHT_NextSampleTime = millis() + DHT_SAMPLE_INTERVAL;  // set the time for next sample
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
