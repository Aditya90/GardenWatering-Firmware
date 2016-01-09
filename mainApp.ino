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
// --- PRIVATE VARIABLES ---
// -------------------------

/**
 * @brief Object for the first motor instance
 */
MotorClass plantMotor1;

// -------------------------
// --- PRIVATE FUNCTION DECLARATIONS ---
// -------------------------
static int main_motorSpeedChange(String command);
static void main_rxMsgUiCallback(RX_MSG_STATUS rxMsgStatus);
static void main_initMotorOutputs(MotorClass* motor, int pinNumber, MOTOR_TYPE motorType);

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
   Spark.function(REST_API_MOTOR_CONTROL_FN, main_motorSpeedChange);
}

/**
 * @brief loop function for the application
 */
void loop()
{

}

// -------------------------
// --- PRIVATE FUNCTION DEFINITIONS ---
// -------------------------

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
