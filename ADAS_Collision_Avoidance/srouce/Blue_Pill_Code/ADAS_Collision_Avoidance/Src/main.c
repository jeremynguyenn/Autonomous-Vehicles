/*
 ******************************************************************************
 * @file           : main.c
 * @author         : Auto-generated by STM32CubeIDE
 * @brief          : Main program body
 ******************************************************************************
 */




#include <stdint.h>

#include "Stm32F103xx.h"

#include "RCC_Interface.h"

#include "GPIO_INTERFACE.h"

#include "SYSTICK_interface.h"

#include "NVIC_Interface.h"

#include "USART_Interface.h"

#include "MotorDriver_Interface.h"

#include "Project_Defines.h"

#include "Project_Init.h"

#include "AEB_System.h"

#include "ACC_System.h"

#include "SELF_PARKING_System.h"

#include "Ultrasonic_Interface.h"

#include "Servo_Core.h"

#include "Buzzer_Interface.h"


uint8_t G_u8DataFromBluetooth  = 0 ; // using UART3  (Rx)
uint8_t G_u8DataFromRaspberrypi  = 0 ; // using UART1  (Rx)

uint8_t Move = MOVE_FORWARD;
uint8_t Current_Speed = 0;
uint8_t Current_System = AEB_SYSTEM;

uint8_t Buzzer_ON = 0;

uint8_t switched = 0;

extern Buzzer_t Buzzer1;

extern Ultrasonic_t Ultrasonic_BackConf;
extern Ultrasonic_t Ultrasonic_FrontConf;


int main(void)
{
	init();

	while(1)
	{

		switch(G_u8DataFromBluetooth)
		{
		case ACC_SYSTEM:
		case SELF_PARKING_SYSTEM:
			Current_System = G_u8DataFromBluetooth;
			G_u8DataFromBluetooth = 0 ;
			break;

		case MOVE_FORWARD:
			MOTORS_Forward(Current_Speed);
			Move = MOVE_FORWARD;
			switched = 1;
			break;

		case MOVE_BACKWARD:
			MOTORS_Backward(Current_Speed);
			Move = MOVE_BACKWARD;
			switched = 1;
			break;

		case MOVE_RIGHT_FORWARD:
			MOTORS_Forward_Right(Current_Speed);
			Move = MOVE_RIGHT_FORWARD;
			switched = 1;
			break;

		case MOVE_LEFT_FORWARD:
			MOTORS_Forward_Left(Current_Speed);
			Move = MOVE_LEFT_FORWARD;
			switched = 1;
			break;

		case MOVE_RIGHT_BACKWARD:
			MOTORS_Backward_Right(Current_Speed);
			Move = MOVE_RIGHT_BACKWARD;
			switched = 1;
			break;

		case MOVE_LEFT_BACKWARD:
			MOTORS_Backward_Left(Current_Speed);
			Move = MOVE_LEFT_BACKWARD;
			switched = 1;
			break;

		case STOP_CAR:
			MOTORS_Stop();
			Current_Speed = 0;
			switched = 1;
			break;

		case SPEED_10 ... SPEED_90:
		    Current_Speed = (G_u8DataFromBluetooth - '0') * 10;
		    switch(Move)
		    {
		    case MOVE_FORWARD:
			   MOTORS_Forward(Current_Speed);
			   break;

		    case MOVE_BACKWARD:
			   MOTORS_Backward(Current_Speed);
			   break;

		    case MOVE_RIGHT_FORWARD:
			   MOTORS_Forward_Right(Current_Speed);
			   break;

		    case MOVE_LEFT_FORWARD:
			   MOTORS_Forward_Left(Current_Speed);
			   break;

		    case MOVE_RIGHT_BACKWARD:
			   MOTORS_Backward_Right(Current_Speed);
			   break;

		    case MOVE_LEFT_BACKWARD:
			   MOTORS_Backward_Left(Current_Speed);
			   break;
		    }
		    switched = 1;
		    break;

		default:
			break;
		}


		if(switched)
		{
			switched = 0;
			Current_System = AEB_SYSTEM;
			G_u8DataFromBluetooth  = 0;
		}


		switch(Current_System)
		{
		case AEB_SYSTEM:
			AEB();
			break;

		case ACC_SYSTEM:
			ACC();
			break;

		case SELF_PARKING_SYSTEM:
			SELF_PARKING();
			Current_System = AEB_SYSTEM;
			break;

		default:
			break;
		}


		//Transmit your current speed to Raspberrypi
		USART_TX_Data(USART1 , Current_Speed);
	}
}
