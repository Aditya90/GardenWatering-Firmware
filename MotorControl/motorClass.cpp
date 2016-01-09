/**
 * @file motorClass.cpp
 * @details This file implements the motor class
 */
 // -------------------------
 // --- INCLUDES ---
 // -------------------------
#include "application.h"
#include "stdio.h"
#include "motorClass.h"

// -------------------------
// --- PRIVATE DEFINES ---
// -------------------------
/**
 * @brief Default motor speed which the output should be set to
 */
#define DEFAULT_MOTOR_SPEED (MOTOR_SPEED_STOP)

/**
 * @brief Middle motor speed which the output should be set to
 */
#define MOTOR_SPEED_MID (120)

/**
 * @brief Speed of the motor to reach a standstill
 */
#define MOTOR_SPEED_STOP (0)

/**
 * @brief Maximum speed of the motor
 */
#define MOTOR_SPEED_MAX (200)

/**
 * @brief Minimum speed of the motor
 */
#define MOTOR_SPEED_MIN (3)

// -------------------------
// --- PRIVATE TYPES ---
// -------------------------

// -------------------------
// --- PRIVATE FUNCTION DECLARATIONS ---
// -------------------------
static void updateMotorSpeed(MotorClass *motorObject, int newSpeed);

// -------------------------
// --- PUBLIC FUNCTION DEFINITIONS ---
// -------------------------
/**
 * @brief Create an instance of a motor
 * @param motorObject - Address at which we should create the motor class instance
 * @param id - Id of the new motor object being created
 */
void Motor_new(MotorClass *motorObject, int motorOutputPin, MOTOR_TYPE motorTypeIp)
{
  motorObject->pinId = motorOutputPin;
  motorObject->motorType = motorTypeIp;

  // Init the motor pin as output
  pinMode(motorObject->pinId, OUTPUT);

  // Set the motor type and update the speed to default
  updateMotorSpeed(motorObject, DEFAULT_MOTOR_SPEED);
}

/**
 * @brief Delete an instance of a motor
 * @param this - Address at which to delete the motor class
 */
void Motor_delete(MotorClass *motorObject)
{
  // Set the motor speed to OFF
  updateMotorSpeed(motorObject, 0);

  // Set Pin ID to 0, though that could be valid :/
  motorObject->pinId = 0;
  motorObject->motorType = MOTOR_SWITCH;
}

/**
 * @brief Update the motor speed based on the input argument
 * @param Motor to Update
 * @param Speed upate to the motor
 */
void Motor_updateSpeed(MotorClass *motorObject, MOTOR_SPEED_MODIF speedChange)
{
   switch(speedChange)
   {
      case SPEED_RESET:
      {
         updateMotorSpeed(motorObject, MOTOR_SPEED_MAX);
         break;
      }
      case SPEED_INCREASE:
      {
         updateMotorSpeed(motorObject, MOTOR_SPEED_MAX);
         break;
      }
      case SPEED_DECREASE:
      {
         updateMotorSpeed(motorObject, MOTOR_SPEED_MIN);
         break;
      }
      case SPEED_STOP:
      {
         updateMotorSpeed(motorObject, MOTOR_SPEED_STOP);
         break;
      }
      default:
      {
         // Set the motor output to 0% PWM
         updateMotorSpeed(motorObject, DEFAULT_MOTOR_SPEED);
         break;
      }
   }
}

// -------------------------
// --- PRIVATE FUNCTION DEFINITIONS ---
// -------------------------

/**
 * @brief Update the motor speed based on the type of motor
 * @param motorObject Motor to Update
 * @param newSpeed Speed upate to the motor
 */
static void updateMotorSpeed(MotorClass *motorObject, int newSpeed)
{

   if (motorObject->motorType == MOTOR_PWM)
   {
      if (newSpeed > MOTOR_SPEED_MAX)
      {
         analogWrite(motorObject->pinId, MOTOR_SPEED_MAX);
      }
      else if (newSpeed < MOTOR_SPEED_MIN)
      {
         analogWrite(motorObject->pinId, MOTOR_SPEED_STOP);
      }
      else
      {
         analogWrite(motorObject->pinId, newSpeed);
      }
   }
   else
   {
      // If speed is greater than stopping motor speed, switch on the load
      if (newSpeed > MOTOR_SPEED_STOP)
      {
         digitalWrite(motorObject->pinId, HIGH);
      }
      else
      {
         digitalWrite(motorObject->pinId, LOW);
      }
   }
}
