/**
 * @file motorClass.h
 * @details This file holds the contents for the motor class and interface
 */
#ifndef MOTOR_CLASS_INTF_H_
#define MOTOR_CLASS_INTF_H_

/**
 * @brief Enumeration to list out the supported speed modifier commands
 */
typedef enum{
   /**
    * @brief Command to increase speed of the motor by a single step
    */
   SPEED_INCREASE,
   /**
    * @brief Command to decrease speed of the motor by a single step
    */
   SPEED_DECREASE,
   /**
    * @brief Set the speed to the default value
    */
   SPEED_RESET,
   /**
    * @brief Stop the motor
    */
   SPEED_STOP
}MOTOR_SPEED_MODIF;

/**
 * @brief Enumeration to list the types of motor types by control strategy
 */
typedef enum{
   /**
    * @brief Motor which can be controlled through switching
    */
   MOTOR_SWITCH = 0,
   /**
    * @brief Motor which can be controlled through PWM
    */
   MOTOR_PWM
}MOTOR_TYPE;

/**
 * @brief Class for the motor properties
 */
typedef struct MotorClass
{
    /**
     * @brief The pin which the motor is assigned to
     */
    int pinId;
    /**
     * @brief Type of motor being used
     */
    MOTOR_TYPE motorType;
};

void Motor_new(MotorClass *motorObject, int motorOutputPin, MOTOR_TYPE motorType);
void Motor_delete(MotorClass *motorObject);
void Motor_updateSpeed(MotorClass *motorObject, MOTOR_SPEED_MODIF speedChange);

#endif
